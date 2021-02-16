/*
Dennis Canar
11 - 15 - 20
CST - 405 - TOS101
Nasser Tadayon
*/

#include<string.h>  // to use strcpy()
#include<stdio.h>   // printf()
#include<stdlib.h>  // free()
#include<ctype.h>   // isdigit()


#include "compilerStructs.h" // Contains all the structs used
#include "Lexer.h"          // lexer
#include "SemanticAnalyzer.h" // Functions for semantic checking / creating AST
#include "SyntaxAnalyzer.h" //Syntax analyzer / grammar / LL1 table & algorithms
#include "IntermediateCode.h" // three adress code & code optimization 

int main(int argc, char *argv[])
{



    /////////////////////////////////////////////////////////////////
    ////////////////////////// Lexical Check ////////////////////////
    /////////////////////////////////////////////////////////////////



    // only if were passed some text file we want to change the lexer output
    if (argc > 1)
    {    
        //Create lexer table variable to use 
        LexerTable LT;
        char * programFile =  argv[1];//"testFile.gcupl";  //File where tokens come from 

        readWords(&LT, programFile);    //Get tokens from file
        removeNewline(&LT);      //remove new line chars
        checkTokens(&LT);        //Give types to the tokens
        //printLexTable(&LT);         //Show lexer table
        // need to add some func to print out any unidentified tokens
        if(printUID(&LT))
        {
            printf("Fix the syntax errors above\n");
            return 0;
        }
        printToFile(&LT, "LexerOutput.txt"); //creates our out file
    }


    /////////////////////////////////////////////////////////////////
    ////////////////////////// Syntax Check /////////////////////////
    /////////////////////////////////////////////////////////////////



    // Call to fill our grammar object with rules and create first follow sets
    initialize();
    fixGrammarErrors();

    //Uncomment below if you would like to see the first and follows sets
    //printSets();
    
    initTable();        //Clear table 
    
    loadTable();        //Takes first follow sets and adds to the ll1 table
    
    //used to show table uncomment if you would like to see (warning very wide table)
    //printTable();
    
    //openning file that will give input to the parser
    FILE * file;
    const char * filename = "LexerOutput.txt";
    file = fopen(filename, "r");

    
    // containers to track our input and stack
    char stack[500];
    char input[500];        // will work like a queue

    //Make sure arrays are clear and initialize stack starting symbol       
    memset(stack, '\0', sizeof(stack));
    push(stack, '$', 500);
    push(stack, C_minus.rules[0].LHS, 500);
    memset(input, '\0', sizeof(input));

    //Call parse funcition and pass file we are parsing
    getInput(&file, input);

    struct Node * root = newNode(C_minus.rules[0].LHS); //Starting Node 
    struct Node * curr = root;                          //Will point to where we are in tree during run time

    // Begin parsing, pass containers, tree head, and lexer output
    parse(stack, input, &curr, &file);
    fclose(file);

    printf("^^^^^^^^^^^^^^^Recursive bug I need to silence\n\n\n\n\n\n\n\n");
    printf("\t\t\t\tSymbol Table\n");
    printf("\t\t\t\t=============\n");

    //See what tree looks like after the syntax checking
    //printTree(root, 0);



    /////////////////////////////////////////////////////////////////
    ////////////////////////// Semantic Check ///////////////////////
    /////////////////////////////////////////////////////////////////

    

    // Create symbol table and do type checking meanwhile
    Table SymbolTable;
    charVec exprContainer;                  // container to help do type checking
    initCharVec(&exprContainer);

    char scope[30];
    memset(scope, '\0', sizeof(scope));
    strcpy(scope, "Global");

    // creates symbol table and checks if an error was returned 
    if(createSymbolTable(&SymbolTable, root, scope, &exprContainer) == 0)
    {
        return 0;
    }

    // This will handle if there is no main in the program 
    if(isMain(&SymbolTable) == 0)
    {
        printf("ERROR: Program has no main()!\n");
        //printTableTree(&SymbolTable, 0);
        return 0;
    }
    else if (isMain(&SymbolTable) == 2)
    {
        printf("ERROR: main() must not contain any arguments!\n");
        return 0;

    }

    printTableTree(&SymbolTable, 0);            //print symbol tables to see our results

    //Creating the Abstact Syntax Tree *****

    AST_Node * astRoot = newAST("P");
    createAST(root, astRoot);
    //printAST(astRoot, 0);

    //printf("Crash after print\n");
    cleanUpAST(astRoot, astRoot, &SymbolTable);

    printf("\n\t\t\t\tSome Type Checking Print Statements\n");
    printf("\t\t\t\t==================================\n");
    //typeCheck(astRoot, &SymbolTable); commented out becasue not fully complete and will cause seg fault



    //////////////////////////////////////////////////
    /////// Code Optimization and Intermediate ///////
    //////////////////////////////////////////////////

    // will be used throughout recursive function to find which ids were used
    charVec  idsUsed;
    initCharVec(&idsUsed);
    
    printf("\n\t\t\t\tOptimizing Code\n");
    printf("\t\t\t\t====================\n");

    printf("Removing Unreachable Nodes\n\n");

    optimizeCode(astRoot, &idsUsed);
    printf("Unused ids:\n");
    printCharVec(&idsUsed);


    printf("\n");

    printf("\t\t\t\tAbstract Syntax Tree\n");
    printf("\t\t\t\t====================\n");
    printAST(astRoot, 0);

    printf("\t\t\t\tThree Address Code\n");
    printf("\t\t\t\t====================\n");

    //printf("\n\n\n\n\n");

    IC_table IntermediateCode;
	initICT(&IntermediateCode);

    // recursive function to iterate through tree and create MIPS like code
    pushICT(&IntermediateCode, "Data:", "-", "-", "-");

    // add a variable for for ascii val of \n
    char newline[10];
    char newlineNum[3];
    memset(newline, '\0', sizeof(newline));
    memset(newlineNum, '\0', sizeof(newlineNum));
    strcpy(newline, "newline");


    // check if program already has a variable named newline, if so increment counter on name
    checkNewLineVar(&SymbolTable, newlineNum);
    strcat(newline, newlineNum);
    strcat(newline, ":");   // to define as variable
    pushICT(&IntermediateCode, newline, ".asciiz", "\\n", "-");

    // begin creating Intermmediate Code
    IC_Data(&IntermediateCode, astRoot, &idsUsed);
    pushICT(&IntermediateCode, "-", "-", "-", "-");  // used as a spacer

    createIC(&IntermediateCode, astRoot, &SymbolTable);
	printICT(&IntermediateCode);


    // release memory we no longer need
    freeCharVec(&idsUsed);

    // free any allocateed memory
    deleteTree(root);                   //Will iterate through tree and free nodes 
    freeSymbolTable(&SymbolTable);      // free the symbol tables allocated memory
    deleteAST(astRoot);

    

    return 0;
}