/*
    Dennis Canar
    12 - 7 - 20 
    CST - 405 - TOS101
    Nasser Tadayon

    This file is a continuation of the compiler project. This specific header file is
    for creating mips like code from the AST created in the previous project. 
*/

#include "compilerStructs.h"

////////////////////////////////////////////////////////////
/////////////// code optimixaztion /////////////////////////
////////////////////////////////////////////////////////////

void idCheck(char * id, charVec * idContainer)
{
    // compare current node passed data to container
    for (int i = 0; i < idContainer->size; i++)
    {
        // if we find the variable has been used we remove it from the list and leave the function 
        if (!strcmp(id, getCharVec(idContainer, i)))
        {
            // clear the array entry becasue we no longer need to see if it was used
            strcpy(idContainer->container[i], "\0");        
            return;
        }
    }
}

// will be used to remove unreachable nodes and 
void optimizeCode(AST_Node * root, charVec * ids)
{

    // when we encounter the var declerations load them into the container
    if (!strcmp(root->data, "V"))
    {
        for (int i = 0; i < root->numChildren; i++)
        {pushCharVec(ids, root->children[i]->data);}
        
        // recursive call ends here becasue there is nothing past the id names
        return;
    }
        
    // need to remove anything after a continue in a while
    // or anything after a return within its scope
    else if (!strcmp(root->data, "return"))
    {
        // return should be one of the last statements withing its scope
        // so if there is anything after, we remove it
        
        AST_Node * blockPtr = root->parent->parent;        // go to block that return is in
        
        // check if there are any statements after it
        while (blockPtr->children[blockPtr->numChildren-1] != root->parent)
        {

            // remove children until return statement is the last within its block
            deleteAST(blockPtr->children[blockPtr->numChildren-1]);
            free(blockPtr->children[blockPtr->numChildren-1]);      // free the child itself
            blockPtr->numChildren--;            // decrement child counter
        }
    }

    // will keep track of ids that are used
    if (root->parent != NULL)
    {        
        // will check if id is on the left side of the equal side, and wont count it as an id usage
        if (!(!strcmp(root->parent->data, "=") && root->parent->children[0] == root))
        {            
            idCheck(root->data, ids);
        }
    }
    
    // recursive call to children
    for (int i = 0; i < root->numChildren; i++)
    {
        optimizeCode(root->children[i], ids);
    }
    
}


////////////////////////////////////////////////////////////
/////////////// 3 address code //////////////////////////////
////////////////////////////////////////////////////////////


void pushICT(IC_table * table, char * op, char * arg1, char * arg2, char * res)
{
	// add these vals to each column of the table
	pushCharVec(&table->Op, op);
	pushCharVec(&table->Arg1, arg1);
	pushCharVec(&table->Arg2, arg2);
	pushCharVec(&table->Result, res);
	table->size++;			// incerase table size by one 
}

void initICT(IC_table * table)
{
	initCharVec(&table->Op);
	initCharVec(&table->Arg1);
	initCharVec(&table->Arg2);
	initCharVec(&table->Result);

	table->size = 0;
    table->tempCounter = 0;
    initCharVec(&table->stack);
}

void printICT(IC_table * table)
{

    printf("Op\t\tArg1\t\tArg2\t\tResult\n");
    printf("=======================================================\n");

	for (int i = 0; i < table->size; i++)
	{
		char * op = getCharVec(&table->Op, i);
		char * arg1 = getCharVec(&table->Arg1, i);
		char * arg2 = getCharVec(&table->Arg2, i);
		char * res = getCharVec(&table->Result, i);

		
        if (strlen(op) >= 8)
        {
            printf("%s\t%s\t\t%s\t\t%s\n", op, arg1, arg2, res);
            
        }
        else if (strlen(arg1) >= 8 && strlen(arg2) >= 8)
        {
            printf("%s\t\t%s\t%s\t%s\n", op, arg1, arg2, res);
        }
        else if (strlen(arg1) >= 8)
        {
            printf("%s\t\t%s\t%s\t\t%s\n", op, arg1, arg2, res);
        }
        else if (strlen(arg2) >= 8)
        {
            printf("%s\t\t%s\t\t%s\t%s\n", op, arg1, arg2, res);
        }
        
        else
        {
            printf("%s\t\t%s\t\t%s\t\t%s\n", op, arg1, arg2, res);
        }
        
        
	}
	
}

void checkIfIndex(IC_table * table, AST_Node * a1, AST_Node * a2, AST_Node * res)
{
    // first check if the argument has any children then check if its the index op
    if(a1 != NULL)
    {
        if (a1->numChildren > 0)
        {
            if (!strcmp(a1->children[0]->data, "["))
            {
                strcat(getCharVec(&table->Arg1, table->size-1), "[]");
            }
            
        }
    }
    if(a2 != NULL)
    {
        if (a2->numChildren > 0)
        {
            if (!strcmp(a2->children[0]->data, "["))
            {
                strcat(getCharVec(&table->Arg2, table->size-1), "[]");
            }
        }
    }
    if(res != NULL)
    {
        if (res->numChildren > 0)
        {
            if (!strcmp(res->children[0]->data, "["))
            {
                strcat(getCharVec(&table->Result, table->size-1), "[]");
            }
        }
    }
}

void binOpExpr2MIPS(IC_table * table, AST_Node * opPtr)
{
    // boolean so we can know when the loop is past its first iteration
    int firstIter = 1;
    
    // will run until there are no more operators that need to be
    // translated to the three address code
    while (isOperator(opPtr->children[1]))
    {
        // create a contiainer for the name and one to onvert the int to a string
        // then the number is added to the name 
        char tempName [] = {'T', '\0', '\0', '\0', '\0', '\0'};
        char tempNumStr [4];
        memset(tempNumStr, '\0', sizeof(tempNumStr));
        sprintf(tempNumStr, "%d", table->tempCounter+1);
        strcat(tempName, tempNumStr);           // add temp number to string
        table->tempCounter++;                   // increment temp variable counter

        if (firstIter)
        {
            //printf("going through first iter: %s\n", opPtr->children[0]->data);
            pushICT(table, opPtr->data, opPtr->children[0]->data, opPtr->children[1]->children[0]->data, tempName);
            //printf("new appended row %s\n", getCharVec(&table->Arg1, table->size-1));
            checkIfIndex(table, opPtr->children[0], opPtr->children[1]->children[0], NULL);
        }
        else
        {
            pushICT(table, opPtr->data, getCharVec(&table->Result, table->size-1), opPtr->children[1]->children[0]->data, tempName);
            checkIfIndex(table, opPtr->children[0], opPtr->children[1]->children[0], NULL);
        }

        // update pointer to the next operator to continue loop
        opPtr = opPtr->children[1];
        firstIter = 0;      // switch boolean becasue we have reached the end of this while
    }

    // once out here we should have reached the end of the chain of operations
    char tempName [] = {'T', '\0', '\0', '\0', '\0', '\0'};
    char tempNumStr [4];
    memset(tempNumStr, '\0', sizeof(tempNumStr));
    sprintf(tempNumStr, "%d", table->tempCounter+1);
    strcat(tempName, tempNumStr);           // add temp number to string
    table->tempCounter++;                   // increment temp variable counter

    pushICT(table, opPtr->data, getCharVec(&table->Result, table->size-1), opPtr->children[1]->data, tempName);
    checkIfIndex(table, NULL, opPtr->children[1], NULL);
    
}

// will find the temp name of for the las if stmt
char * lastIfTemp(IC_table * table)
{

    for (int i = table->size-1; i >= 0; i--)
    {
        if (!strcmp(getCharVec(&table->Op,i), "beq"))
        {
            return getCharVec(&table->Result, i);
        }
        else if (!strcmp(getCharVec(&table->Op,i), "bltz"))
        {
            return getCharVec(&table->Result, i);
        }
        else if (!strcmp(getCharVec(&table->Op,i), "bgtz"))
        {
            return getCharVec(&table->Result, i);
        }
        else if (!strcmp(getCharVec(&table->Op,i), "blez"))
        {
            return getCharVec(&table->Result, i);
        }       
        else if (!strcmp(getCharVec(&table->Op,i), "bgez"))
        {
            return getCharVec(&table->Result, i);
        }   
        else if (!strcmp(getCharVec(&table->Op,i), "bne"))
        {
            return getCharVec(&table->Result, i);
        }             
    }
    // if no condition is found
    return NULL;
}

void ifExpr2MIPS(IC_table * table, AST_Node * opPtr)
{
    // boolean for some condition
    int firstIter = 1;
    
    // will run until there are no more operators that need to be
    // translated to the three address code
    while (isOperator(opPtr->children[1]))
    {
        // create a contiainer for the name and one to onvert the int to a string
        // then the number is added to the name 
        char tempName [] = {'T', '\0', '\0', '\0', '\0', '\0'};
        char tempNumStr [4];
        memset(tempNumStr, '\0', sizeof(tempNumStr));
        sprintf(tempNumStr, "%d", table->tempCounter+1);
        strcat(tempName, tempNumStr);           // add temp number to string
        table->tempCounter++;                   // increment temp variable counter

        if (firstIter)
        {
            pushICT(table, opPtr->data, opPtr->children[0]->data, opPtr->children[1]->children[0]->data, tempName);
            checkIfIndex(table, opPtr->children[0], opPtr->children[1]->children[0], NULL);
        }
        else
        {
            pushICT(table, opPtr->data, getCharVec(&table->Result, table->size-1), opPtr->children[1]->children[0]->data, tempName);
            checkIfIndex(table, opPtr->children[0], opPtr->children[1]->children[0], NULL);
        }
        

        // update pointer to the next operator to continue loop
        opPtr = opPtr->children[1];
        firstIter = 0;
    }

    // once out here we should have reached the end of the chain of operations
    char tempName [] = {'T', '\0', '\0', '\0', '\0', '\0'};
    char tempNumStr [4];
    memset(tempNumStr, '\0', sizeof(tempNumStr));
    sprintf(tempNumStr, "%d", table->tempCounter+1);
    strcat(tempName, tempNumStr);           // add temp number to string
    table->tempCounter++;                   // increment temp variable counter

    pushICT(table, opPtr->data, getCharVec(&table->Result, table->size-1), opPtr->children[1]->data, tempName);
    checkIfIndex(table, NULL, opPtr->children[1], NULL);
    
}


void conditionToMIPS(AST_Node * root, IC_table * table, char * tempName)
{
    // case for comparison
    if (!strcmp(root->children[0]->data, "=="))
    {
        pushICT(table, "beq", root->children[0]->children[0]->data, root->children[0]->children[1]->data, tempName);
        checkIfIndex(table, root->children[0]->children[0], root->children[0]->children[1], NULL);
    }
    else if (!strcmp(root->children[0]->data, ">"))
    {
        pushICT(table, "bgt", root->children[0]->children[0]->data, root->children[0]->children[1]->data, tempName);
        checkIfIndex(table, root->children[0]->children[0], root->children[0]->children[1], NULL);
    }
    else if (!strcmp(root->children[0]->data, ">="))
    {
        pushICT(table, "bge", root->children[0]->children[0]->data, root->children[0]->children[1]->data, tempName);
        checkIfIndex(table, root->children[0]->children[0], root->children[0]->children[1], NULL);
    }                                 
    else if (!strcmp(root->children[0]->data, "<"))
    {
        pushICT(table, "blt", root->children[0]->children[0]->data, root->children[0]->children[1]->data, tempName);
        checkIfIndex(table, root->children[0]->children[0], root->children[0]->children[1], NULL);
    }     
    else if (!strcmp(root->children[0]->data, "<="))
    {
        pushICT(table, "ble", root->children[0]->children[0]->data, root->children[0]->children[1]->data, tempName);
        checkIfIndex(table, root->children[0]->children[0], root->children[0]->children[1], NULL);
    }
    else if (!strcmp(root->children[0]->data, "!="))
    {
        pushICT(table, "bne", root->children[0]->children[0]->data, root->children[0]->children[1]->data, tempName);
        checkIfIndex(table, root->children[0]->children[0], root->children[0]->children[1], NULL);
    }                       
    else if (!strcmp(root->children[0]->data, "&&"))
    {
        pushICT(table, "bltz", root->children[0]->children[0]->data, root->children[0]->children[1]->data, tempName);
        checkIfIndex(table, root->children[0]->children[0], root->children[0]->children[1], NULL);
    }                                 
    else if (!strcmp(root->children[0]->data, "||"))
    {
        pushICT(table, "bltz", root->children[0]->children[0]->data, root->children[0]->children[1]->data, tempName);
        checkIfIndex(table, root->children[0]->children[0], root->children[0]->children[1], NULL);
    }
}

// simlar to condition to mips but conditions are negated/inversed
void whileCondToMIPS(AST_Node * root, IC_table * table, char * tempName)
{
    // case for comparison
    if (!strcmp(root->children[0]->data, "=="))
    {
        pushICT(table, "bne", root->children[0]->children[0]->data, root->children[0]->children[1]->data, tempName);
        checkIfIndex(table, root->children[0]->children[0], root->children[0]->children[1], NULL);
    }
    else if (!strcmp(root->children[0]->data, ">"))
    {
        pushICT(table, "ble", root->children[0]->children[0]->data, root->children[0]->children[1]->data, tempName);
        checkIfIndex(table, root->children[0]->children[0], root->children[0]->children[1], NULL);
    }
    else if (!strcmp(root->children[0]->data, ">="))
    {
        pushICT(table, "blt", root->children[0]->children[0]->data, root->children[0]->children[1]->data, tempName);
        checkIfIndex(table, root->children[0]->children[0], root->children[0]->children[1], NULL);
    }                                 
    else if (!strcmp(root->children[0]->data, "<"))
    {
        pushICT(table, "bge", root->children[0]->children[0]->data, root->children[0]->children[1]->data, tempName);
        checkIfIndex(table, root->children[0]->children[0], root->children[0]->children[1], NULL);
    }     
    else if (!strcmp(root->children[0]->data, "<="))
    {
        pushICT(table, "bgt", root->children[0]->children[0]->data, root->children[0]->children[1]->data, tempName);
        checkIfIndex(table, root->children[0]->children[0], root->children[0]->children[1], NULL);
    }
    else if (!strcmp(root->children[0]->data, "!="))
    {
        pushICT(table, "beq", root->children[0]->children[0]->data, root->children[0]->children[1]->data, tempName);
        checkIfIndex(table, root->children[0]->children[0], root->children[0]->children[1], NULL);
    }                       
    else if (!strcmp(root->children[0]->data, "&&"))
    {
        pushICT(table, "bltz", root->children[0]->children[0]->data, root->children[0]->children[1]->data, tempName);
        checkIfIndex(table, root->children[0]->children[0], root->children[0]->children[1], NULL);
    }                                 
    else if (!strcmp(root->children[0]->data, "||"))
    {
        pushICT(table, "bltz", root->children[0]->children[0]->data, root->children[0]->children[1]->data, tempName);
        checkIfIndex(table, root->children[0]->children[0], root->children[0]->children[1], NULL);
    }
}

// will iterate through tree and create IC from the AST
void createIC(IC_table * table, AST_Node * root, Table * symTable)
{

    // function declaration 
    if (!strcmp(root->data, "F"))
    {
        // create temp container to add ":"" to funciton name
        char temp[30];
        memset(temp, '\0', sizeof(temp));

        strcpy(temp, root->children[0]->data);
        strcat(temp, ":");

        pushICT(table, temp, "-", "-", "-");

        printf("loading %d args\n", getNumParams(symTable, root->children[0]->data));
    }
    
    // assignment operators
    else if (!strcmp(root->data, "="))
    {
        // temp variables for the function
        AST_Node * op = NULL;
        AST_Node * arg1 = NULL;
        AST_Node * arg2 = NULL;
        AST_Node * res = NULL;

        // if the = op has another operation within it
        if (isOperator(root->children[1]))
        {
            // if there is an operator we need to see if it will require any temp variables
            if (isOperator(root->children[1]->children[1]))
            {
                //printf("%s: '=' expression with more than one bin op\n", root->children[0]->data);   
                // will make the three adress code for the right side of the = sign first
                binOpExpr2MIPS(table, root->children[1]); 
                
                // finally take temp var made in the previous function and set it equal to our left side
                pushICT(table, "sw", getCharVec(&table->Result, table->size-1), root->children[0]->data, "-"); 

                return;             
            }
            // if there is just one operator we can add this to the table
            // by just pushing on one new row
            else
            {
                op = root->children[1];
                arg1 = root->children[1]->children[0];
                arg2 = root->children[1]->children[1];
                res = root->children[0];
            }
        }
        else
        {
            arg1 = root->children[1];
            res = root->children[0];            
        }
        
        // will let us know = operation being evaluated has no operators in it
        if (op == NULL)
        {
            // check if loading an immiediate
            if (isdigit(arg1->data[0]))
            {
                pushICT(table, "li", res->data, arg1->data, "-");
            }
            else
            {
                pushICT(table, "sw", arg1->data, res->data, "-");                
            }
            
        }
        // if equal is one operation long we can just save the result straigt to the left side of the equal sign 
        else
        {
            pushICT(table, op->data, arg1->data, arg2->data, res->data);
        }

        // check if any of the arguments are indexes into arrays
        checkIfIndex(table, arg1, arg2, res);
    }
    
    // will handle conditional statements for if and while
    else if (!strcmp(root->data, "if"))
    {
        //printf("\nprocessing %s statement\n", root->data);
        // getting name for the new temp variable
        char tempName [] = {'T', '\0', '\0', '\0', '\0', '\0'};
        char tempNumStr [4];
        memset(tempNumStr, '\0', sizeof(tempNumStr));
        sprintf(tempNumStr, "%d", table->tempCounter+1);
        strcat(tempName, tempNumStr);           // add temp number to string
        table->tempCounter++;                   // increment temp variable counter

        // will keep track of what temp var needs to be used as a break variable
        pushCharVec(&table->stack, tempName);
    
        // first check if the if condition is some boolean operator
        if (root->children[0]->numChildren > 1)
        {
            // check if the condition stmt has more than one bin op 
            if (isOperator(root->children[0]->children[1]))
            {
                //printf("if condition with an operator\n");
                ifExpr2MIPS(table, root->children[0]->children[1]);
            }
            // 
            else
            {
                // if just one binary op long
                if (isOperator(root->children[0]))
                {
                    conditionToMIPS(root, table, tempName);
                }                  
            }    
        }

        // if were in this stament we expect one arg in the 
        else
        {
            pushICT(table, "beq", root->children[0]->data, "$zero", tempName);
            checkIfIndex(table, root->children[0]->children[0], NULL, NULL);
            
        } 
    }
    
    else if (!strcmp(root->data, "while"))
    {

        printf("\nprocessing %s statement\n", root->data);
        // getting name for the new temp variable
        char tempName [] = {'T', '\0', '\0', '\0', '\0', '\0'};
        char tempNumStr [4];
        memset(tempNumStr, '\0', sizeof(tempNumStr));
        sprintf(tempNumStr, "%d", table->tempCounter+1);
        strcat(tempName, tempNumStr);           // add temp number to string
        table->tempCounter++;                   // increment temp variable counter

        // pushes label on to stack so we can know which label to use on exit of loop
        pushCharVec(&table->stack, tempName);
    
        // first check if the condition is some boolean operator
        if (root->children[0]->numChildren > 1)
        {
            // check if the condition stmt has more than one bin op 
            if (isOperator(root->children[0]->children[1]))
            {
                //ifExpr2MIPS(table, root->children[0]->children[1]);
            }
            // 
            else
            {
                // if just one binary op long
                if (isOperator(root->children[0]))
                {
                    printf("one op long while\n");
                    whileCondToMIPS(root, table, tempName);
                }                  
            }    
        }

        // if were in this stament we expect one arg for the condition 
        else
        {
            pushICT(table, "beq", root->children[0]->data, "$zero", tempName);
            checkIfIndex(table, root->children[0]->children[0], NULL, NULL);
            
        } 

    }
    
   // Creates label to jump out of while 
    else if(!strcmp(root->data, "L") && whatASTamI(root) != 0)
   {

       // ensure were not looking for a child that does not exist
       if (root->parent->children[whatASTamI(root)-1]->numChildren > 0)
       {
            // check if statement is after the while loop expressions (a.k.a the else expressions)
            if (!strcmp(root->parent->children[whatASTamI(root)-1]->children[0]->data, "while"))
            {
                //printf("Found while loop exit for: %s\n", getCharVec(&table->stack, table->stack.size-1));
                pushICT(table, getCharVec(&table->stack, table->stack.size-1), "-", "-", "-");
                popCharVec(&table->stack);
                strcat(getCharVec(&table->Op, table->size-1), ":");   // formatting to let know this is an else:
            }
       }
   }
   

    // not sure if write means to load or to print to screen or both
    else if(!strcmp(root->data, "write"))
    {

        if (root->children[0]->numChildren == 0 )
        {
            pushICT(table, "li", "$v0", "1", "-");
            pushICT(table, "move", root->children[0]->data, "-", "$a0");
            pushICT(table, "syscall", "-", "-", "-");            
        }
        else
        {

            // check if there is an operator so we can break down the expression
            if (isOperator(root->children[0]))
            {
                printf("write with an expression: %s\n", root->children[0]->children[0]->data);
                binOpExpr2MIPS(table, root->children[0]);
            }
            
            // getting name for the new temp variable
            char tempName [] = {'T', '\0', '\0', '\0', '\0', '\0'};
            char tempNumStr [4];
            memset(tempNumStr, '\0', sizeof(tempNumStr));
            sprintf(tempNumStr, "%d", table->tempCounter+1);
            strcat(tempName, tempNumStr);           // add temp number to string
            table->tempCounter++;                   // increment temp variable counter

            //push mips to make system call to rprint
            pushICT(table, "li", "$v0", "1", "-");
            pushICT(table, "move", tempName, "-", "$a0");
            pushICT(table, "syscall", "-", "-", "-");            
        }
        
        


    }

    // will add a jump register to go back to where function call was made 
    else if(!strcmp(root->data, "return"))
    {

        // see if returning exression with one variable or many 
        if(root->children[0]->numChildren == 0)
        {
            //printf("move to $v0\n");
            pushICT(table, "move", "$v0", root->children[0]->data, "-");
        } 

        else
        {
            /* code */
        }
        

        // MIPS to return to function call
        pushICT(table, "jr", "$ra", "-", "-");
    }

    else if (!strcmp(root->data, "writeln"))
    {

        // temp var used to get newline name without :
        char temp[10];
        strcpy(temp, strtok(getCharVec(&table->Op, 1), ":"));

        // MIPS to print an empty line char
        pushICT(table, "la", "$a0", temp, "-");
        pushICT(table, "li", "$v0", "4", "-");
        pushICT(table, "syscall", "-", "-", "-");
    }
    
    // simple MIPS we expect an id so we just use the system call to get the user input and move it to the var
    else if (!strcmp(root->data, "read"))
    {
        pushICT(table, "li", "$v0", "5", "-");
        pushICT(table, "syscall", "-", "-", "-"); 
        pushICT(table, "sw", root->children[0]->data, "$v0", "-"); 
    }
    
    else if(!strcmp(root->data, "("))
    {
        pushICT(table, "jal", root->parent->data, "-", "-");
    }

    //make condition for index and fucntion call && do index declaration

    // will make label for else to jump to
    if(root->parent != NULL)
    {
        if (!strcmp(root->parent->data, "if") && whatASTamI(root) == 2)
        {
            // begin by getting the temp name of the else function
            pushICT(table, getCharVec(&table->stack, table->stack.size-1), "-", "-", "-");
            popCharVec(&table->stack);
            strcat(getCharVec(&table->Op, table->size-1), ":");   // formatting to let know this is an else:
        }
    }

    // continue iterating through AST and creating IC
    for (int i = 0; i < root->numChildren; i++)
    {
        createIC(table, root->children[i], symTable);
    }
    
}

// will return boolean if id is one that should not be added to data section
int isUnusedVar(charVec * unused, char * id)
{
    for (int i = 0; i < unused->size; i++)
    {
        if (!strcmp(getCharVec(unused, i), id))
        {
            return 1;
        }
    }
    // if id is not in one of the unused ones 
    return 0;
}

// will create our data section before the creating MIPS
void IC_Data(IC_table * table, AST_Node * root, charVec * unusedIDs)
{

    //  when we encounter the variable declaratinos
    if(!strcmp(root->data, "V"))
    {
        // will go through vars
        for (int i = 0; i < root->numChildren; i++)
        {
            //check if array or not
            if (root->children[i]->numChildren == 0)
            {
                if (!isUnusedVar(unusedIDs, root->children[i]->data))
                {
                    pushICT(table, root->children[i]->data, ".word", "0", "-");
                    strcat(getCharVec(&table->Op, table->size-1), ":");         // some formatting for the data section
                }   
            }   
        }
    }
    
    for (int i = 0; i < root->numChildren; i++)
    {
        IC_Data(table, root->children[i], unusedIDs);
    }
}

// will return boolenan val on if the variable name is being used
int newLineHelper(Table * table, char * numContainer)
{
    // create string to compare to nodes
    char tempName[10];
    memset(tempName, '\0', sizeof(tempName));
    strcpy(tempName, "newline");
    strcat(tempName, numContainer);
    
    // loop through current table to find if the id was used or not
    for (int i = 0; i < table->size; i++)
    {
        // if we find a var with the name we would like to use return 1
        if (!strcmp(getCharVec(&table->identifier, i), tempName))
        {
            return 1;
        }
    }
    
    //printf("testh: %s\n", getCharVec(&table->scope, 0));
  
    // continue recursive search
    for (int i = 0; i < table->numChildren; i++)
    {
        return newLineHelper(table->children[i], numContainer);
    }
    
    // if we end here the var name was not used
    return 0;
}

void checkNewLineVar(Table * table, char * numContainer)
{
    int counter = 0;

    // will loop through tree looking if there is already a var with the same name 
    while (newLineHelper(table, numContainer))
    {
        counter++;  //increment counter
        sprintf(numContainer, "%d", counter);
    }

    

}