/*
Dennis Canar
9-13-20
CST-405-TOS101
LexicalAnalyzer.C
Nasser Tadayon
*/

/* 
Creating a lexer to tokenize words within a program the token
table will then be used throughout the compilation process
*/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "compilerStructs.h" // include so we cna use our structures

// Some Global Variables Used (will be updated in the future)
const int numKeywords = 10;

//All the keywords used in C--
char Keywords[numKeywords][15] = { "return", "read", "write", "writeln",
                            "break", "if", "else", "while", "int", 
                            "char", 

                    };

//Array of operators in C-- 
char BinaryOp[11][3] = {
                        "+", "/", "*", "==", "<", ">",
                         "<=", ">=", "||", "&&", "!=",           
                    };

char UnaryOP[2][2] = {"-", "!"};

//Array of special character in C--
char Specials[8][2] = {
                        "{", "}", "[", "]", "(", ")", ";", ","
                    };


// Will delete any new line chars left at the end of my string
void removeNewline(LexerTable * LTable){

    for(int i = 0; i < LTable->numTokens; i++){

        char * token = LTable->Token[i]; // token during loop

        //Check if last char is a newline
        if(token[strlen(token)-1] == '\n'){
            LTable->Token[i][strlen(token)-1] = '\0';   //change to string terminator
        }

    }
}

void readWords(LexerTable * LTable, char * filename){

    // local variables to keep track of my place in the file
    // and one to hold the current line fo strings being parsed
    size_t index = 0;     
    char * line = NULL;
    int lineCount = 0;      // keep track of line # of tokens

    // file pointer
    FILE * fp = fopen(filename, "r"); //openning file

    LTable->numTokens = 0; //initializing token counter


    // getting each line of the file then parsing into seperate words
    while((getline(&line, &index, fp))!= -1){

        lineCount++; // we read a line so we incremetn the counter

        // Splitting line by white space and getting first element
        char * word = strtok(line, " ");  // str to tokens

        //Iterating through other elements until NULL is returned
        while(word != NULL){

            // Do this to filter the newline characters in file
            if(word[0] != '\n' && strcmp(word, "\n") && word[0] != '\t'){
                //Copy word to our tables container
                strcpy(LTable->Token[LTable ->numTokens], word );
                
                // record line num of token
                LTable->lineNum[LTable->numTokens] = lineCount; 
                LTable->numTokens++; //increment token counter                
            }

            // here word knows our position in the string so we tell it 
            word = strtok(NULL, " ");       // continue parsing by " "
        }

    }

    fclose(fp);         // close file we are reading from
    

/*
    //create file type and open filename passed in
	FILE * fp = fopen(filename, "r");

	char  token[30]; //var to read token to
	LTable->numTokens = 0; //initializing counter

    //Will read each string in the file while not end of file
    //Copy each word to the container at index of token counter
    while( fscanf(fp, "%s", token) != EOF ){
        strcpy(LTable->Token[LTable->numTokens], token);
        LTable->numTokens++; //increment token counter
    }

*/

	
}

//Function to print table structure
void printLexTable(LexerTable* LTable){

    //Print table header before looping through table
    printf("Token\t\tType\t\tLine\tError");
    printf("\n=================================================================\n");

    for(int i = 0; i < LTable->numTokens; i++){

        //Row of data from our table 
        char * token = LTable->Token[i];
        char * type = LTable->Type[i];
        char * error = LTable->Error[i];
        int line = LTable->lineNum[i];

        //formatting of tabs to keep table clean
        if(strlen(LTable->Token[i]) >= 8){
            if(strlen(LTable->Type[i]) >= 8){
                printf("%s\t%s\t%d\t%s\n", token, type, line, error);
            }
            else
            {
                printf("%s\t%s\t\t%d\t%s\n", token, type, line, error);
            }
            
        }
        else{ //If token is not too long
            if(strlen(LTable->Type[i]) >= 8){
                printf("%s\t\t%s\t%d\t%s\n", token, type, line, error);
            }

            else
            {
                printf("%s\t\t%s\t\t%d\t%s\n", token, type, line, error);
            }
            
        }
        
    }
    printf("\nNumber of tokens: %d\n", LTable->numTokens);
}

bool checkKeyword(char* token){
    
    //Loop through keywords
   for(int i = 0; i < numKeywords; i++){

       //if token matches keyword return true
       if(strcmp(token, Keywords[i]) == 0){
           return true;
       }

   }
    return false;
}

// function returns true if number is in range of min and max
bool inRange(int min, int max, int num){
    if(min <= num && max >= num){
        return true;
    }

    //if not true 
    return false;
}

bool checkIdentifier(char* token, char* err){

    //Using bool to keep trach what state im in
    bool q0 = true;
    bool q1 = false;

    //Important markers in ASCII table
    int A = 65;
    int Z = 90;
    int a = 97;
    int z = 122;
    int _ = 95;
    int zero = 48;
    int nine = 57;

    for (int i = 0; i < strlen(token); i++){

        //i'th character of string ASCII value
        int ASCII = (int)token[i];

        if(q0){ //if in state

        //If input is letter or underscore
          if((inRange(A,Z,ASCII) || inRange(a,z,ASCII)) || ASCII==_){
            q0 = false;
            q1 = true;  //changing states
          }
          else{
              return false; //if not accepted input goes to trash
          }

        }

        else if(q1){ //if in state

            if(!((inRange(A,Z,ASCII) || inRange(a,z,ASCII) || inRange(zero,nine,ASCII)) || ASCII==_)){
                strcpy(err, "Identifier must only use alpahnumerics or underscore.");
                return false;   //if input is not accepted
            }

        }

    }

    //If the string ended in the accepting state
    if(q1){return true;}
    else
    {
        return false;   // just in case, not sure how function would reach here though
    }
    

}

bool checkConstant(char* token, char* err){

    //States
    bool q0 = true;
    bool q1 = false;
    bool q2 = false;
    bool q3 = false;

    //ASCII Ranges
    int zero = 48;
    int one = 49;
    int nine = 57;
    int decimal = 46;

    for (int i = 0; i < strlen(token); i++){

        int ASCII = (int)token[i]; //ASCII val of char

        if(q0){
            if(ASCII == zero){ //if input is 0
                q0 = false;
                q1 = true;      //changing states
            }
            else if(ASCII == decimal){  //if input is decimal
                q0 = false;
                q3 = true;  //changing states
            }
            else if(inRange(one,nine,ASCII)){  //if non 0 input
                q0 = false;
                q2 = true;  //changing states
            }
            else{
                return false;   //If char is not a number
            }
        }
        
        //This state knows we have a 0 already input
        else if(q1){
            if(ASCII == decimal){ //this state requires decimal
                q1 = false;
                q3 = true;
            }
            else{
                strcpy(err, "Coeficcient may not have leading zeros.");
                return false;
            }
        }
        //This state has had a non zero input
        else if(q2){
            if (ASCII == decimal){
                q2 = false;     //change state
                q3 = true;
            }
            else if(!inRange(zero,nine,ASCII)){
                strcpy(err, "Coefficient may only contain numbers.");
                return false; //if input is not an integer
            }
        }
        //If decimal has been input
        else if(q3){
            if(!inRange(zero,nine,ASCII)){
                strcpy(err, "Coefficient may only contain numbers.");
                return false;   //only non integer input goes to trash
            }
        }
        
    }

    //All states are accpeted so if not thrown to trash then valid
    return true;

}

//Will check if first and last letter to check if a string
bool checkString(char* token, char* err){

    if(token[0] == '\"' && token[strlen(token)-1] == '\"'){
        return true;
    }

    //If string has been started but not finished
    if(token[0]== '\"' && token[strlen(token)-1] != '\"'){
        strcpy(err, "String must end with \"");
    }

    return false;
}

bool checkChar(char * token, char * err){

    //This deosnt seem like the best check but in reality a char 
    // should only be symbol in between two single quotes
    if(token[0] == '\'' && token[2] == '\''){
        return true;
    }

    //If there is a token using single quotes to create a char but has too many chars
    else if (token[0] == '\'' && token[strlen(token)-1] == '\'')
    {
        strcpy(err, "Char may consits of only one character.");        
        return false;
    }
    
    else
    {
        return false;
    }
    

}

int checkOperator(char * token, char * err){

    // compare token to equals sign
    if(strcmp(token, "=") == 0){
        return 1;
    }

    //Checking Unary Operators
    for(int i = 0; i < 2; i++){

        // Checking for exact match first
        if(strcmp(token, UnaryOP[i]) == 0){
            return 2;
        }

        // checks for unary op at beginning of token to handle errors
        else if(strncmp(token, UnaryOP[i], strlen(UnaryOP[i])) == 0){
            strcpy(err, "Unary Operators must have space in between tokens.");
        }
    }

    // Will loop through binary operators to compare  
    for(int i = 0; i < 11; i++){

         if(strcmp(token, BinaryOp[i]) == 0){
            return 3;
        }

        //If operator is found at begnning of token error handle
        //only did this group because its the only one with 2 chars
        else if(strncmp(token, BinaryOp[i], strlen(BinaryOp[i])) == 0){
            strcpy(err, "Operators must have space in between tokens.");
        }
    }
    
    // If no error could be assesed
    return -1;

}

bool checkSpecial(char* token, char * err){

    //Compare token to array of special characters
   for(int i = 0; i < 8; i++){

       if(strcmp(token, Specials[i]) == 0){
           return true;
       }

       //Error handle if special char is at beginning of another token
       else if(strncmp(token, Specials[i], strlen(Specials[i])) == 0){
            //printf("Token:%s\tSpecial:%d\n", token, i);
            strcpy(err, "Special characters must have space in between tokens.");
       }
   }
    return false;

}

void checkTokens(LexerTable* LTable){

    //Will run each token through the dfa until one returns true
    //if non return true type unidentified
    for(int i = 0; i < LTable->numTokens; i++){


        if(checkKeyword(LTable->Token[i])){
            strcpy(LTable->Type[i], "Keyword");     //Change token type upon success
            strcpy(LTable->Error[i], "------");     //No error becasue type found
        }
        else if(checkIdentifier(LTable->Token[i], LTable->Error[i])){
            strcpy(LTable->Type[i], "Identifier");
            strcpy(LTable->Error[i], "------");
        }
        else if(checkConstant(LTable->Token[i], LTable->Error[i])){
            strcpy(LTable->Type[i], "Constant");
            strcpy(LTable->Error[i], "------");
        }
        else if(checkString(LTable->Token[i], LTable->Error[i])){
            strcpy(LTable->Type[i], "String");
            strcpy(LTable->Error[i], "------");
        }
        else if(checkChar(LTable->Token[i], LTable->Error[i])){
            strcpy(LTable->Type[i], "Char");
            strcpy(LTable->Error[i], "------");
        }
        else if(checkOperator(LTable->Token[i], LTable->Error[i])==1){
            strcpy(LTable->Type[i], "Equals");
            strcpy(LTable->Error[i], "------");
        }
        else if(checkOperator(LTable->Token[i], LTable->Error[i])==2){
            strcpy(LTable->Type[i], "UnaryOp");
            strcpy(LTable->Error[i], "------");
        }
        else if(checkOperator(LTable->Token[i], LTable->Error[i])==3){
            strcpy(LTable->Type[i], "BinaryOp");
            strcpy(LTable->Error[i], "------");
        }
        else if(checkSpecial(LTable->Token[i], LTable->Error[i])){
            strcpy(LTable->Type[i], "Special Symbol");
            strcpy(LTable->Error[i], "------");
        }
        else{
            strcpy(LTable->Type[i], "Unidentified");    //If no type could be made

            // if no error was assesed prior
            if(LTable->Error[i][0] == '\0'){
                strcpy(LTable->Error[i], "Unknown token");
            }
        }
    }

}

// tool used to just check tokens array
void printTokens(LexerTable * LTable){
    for(int i = 0; i < LTable->numTokens; i++){
         printf("index: %d %s\n", i, LTable->Token[i]);
    }
}

void printToFile(LexerTable * LTable, char * file){

    //first we open the file we want to write to
    FILE * fp = fopen(file, "w+");
    
    //Loop to print all rows of table
    for(int i = 0; i < LTable->numTokens; i++){

        // Getting row data 
        char * token = LTable->Token[i];
        char * type = LTable->Type[i];
        int line = LTable->lineNum[i];
        char * err = LTable->Error[i];

        //Print to file 
        fprintf(fp, "{%s;%s;%d;%s}\n", token, type, line, err);

    }

    // when done printing close the file
    fclose(fp);
}

// will iteratethrough the table and print which tokens need to be fixed
int printUID(LexerTable * table)
{
    // will be used if we find an error after the check
    int errorsBool = 0;

    for (int i = 0; i < table->numTokens; i++)
    {
        if (!strcmp(table->Type[i], "Unidentified"))
        {
            printf("Error on line: %d | Near Token: \" %s \"\n", table->lineNum[i], table->Token[i]);
            errorsBool = 1;         // let boolean signal there was some error
        }
        
    }
    
    return errorsBool;
    
}