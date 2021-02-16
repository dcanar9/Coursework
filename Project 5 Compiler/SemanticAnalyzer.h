/*
Dennis Canar
11 - 15 - 20
CST - 405 - TOS101
Nasser Tadayon

In this header file I will be placing all the function that will help our parse
tree become an AST 
*/

#include "compilerStructs.h" // include so we cna use our structures
#include <assert.h>

int maxStringLen = 30;

/////////////////Dynamic char vector methods/////////////

void initCharVec(charVec * vector)
{
    vector->container = NULL;
    vector->size = 0;
}

void printCharVec(charVec * vector)
{

    for (int i = 0; i < vector->size; i++)
    {
        printf("%s, ", vector->container[i]);
    }

    printf("\n");
    

}

void freeCharVec(charVec * vector)
{

    //first we must free all the string 
    for (int i = 0; i < vector->size; i++)
    {
        free(vector->container[i]);
    }
    
    // then we can free the container of strings
    free(vector->container);

}

void pushCharVec(charVec * vector, char * string)
{

    // if the array has not been allocated yet
    if (vector->size == 0)
    {

        // allocate room for one character pointer
        vector->container = calloc(1 ,sizeof(char*));
        // allocate memory for that pointer to store chars
        vector->container[vector->size] = calloc(maxStringLen, sizeof(char));
        // add string to the contatiner
        strcpy(vector->container[vector->size], string);
    }
    else{
        // create a temp pointer pointer with one more pointer than the container we are pushing to
        char ** ptr;
        ptr =  calloc(vector->size+1,  sizeof(char*));

        if (ptr == NULL)
        {
            printf("ERROR: could not reallocate vector data!\n");
            return;
            
        }

        // copy strings to the new vector
        for (int i = 0; i < vector->size; i++)
        {
            ptr[i] =  calloc(maxStringLen, sizeof(char));
            strcpy(ptr[i], vector->container[i]);
        }    

        // free the old vector and set to our new one
        freeCharVec(vector);
        vector->container = ptr;

        // allocate room for string add the new string we are pushing to the vector
        vector->container[vector->size] = (char*) malloc(maxStringLen * sizeof(char));
        strcpy(vector->container[vector->size], string);
    }

    //i increment size counter
    vector->size++;
}

// will remove item in the last index of the array 
void popCharVec(charVec * vec)
{

	if (vec->size == 0)
	{
		printf("Unable to pop empty stack\n");
		return;
	}
	else
	{
		//create temp container one size smaller and copy over elements
		char ** temp = calloc(vec->size-1, sizeof(char*));

		// copy over elements
		for (int i = 0; i < vec->size-1; i++)
		{
			temp[i] = (char*) malloc(maxStringLen * sizeof(char));	// make room for string in array
			strcpy(temp[i], vec->container[i]);						//copy over string
		}
		
		// free old vector
		freeCharVec(vec);
		vec->container = temp;
		vec->size--;
	}
}

// used becasue we cannot index with a charVec type (index starts at 0)
char * getCharVec(charVec * vector, int index)
{
    return vector->container[index];
}

void clearCharVec(charVec * vector)
{

    // if there is any memory to delete
    if (vector->container != NULL)
    {
        free(vector->container);            // release the memory we have allocated previously
        vector->container = NULL;           // set pointer to NULL becasue there is no data
        vector->size = 0;                   // update size 
    }
    
    return;
}

//////////////////////////////Symbol Table Methods//////////////////////////////

//will initialize the cointainers within our table
void initSymbolTable(Table * table)
{

    /*  This will make sure the underlying pointers are not pointing to a random peice
        of data & this will also initialize the size similar to the bottom of this function.
        Also we pass this argument by reference so we edit the actual struct and not a copy*/
    initCharVec(&table->identifier);
    initCharVec(&table->type);
    initCharVec(&table->array);
    initCharVec(&table->funcArgs);
    initCharVec(&table->action);
    initCharVec(&table->scope);

    table->children = NULL;
    table->parent = NULL;

    table->size = 0;
    table->numChildren = 0;

}

void printSymbolTable(Table * table)
{

    if (table->size == 0)
    {
        printf("The table is empty!\n");
    }
    

    for (int i = 0; i < table->size; i++)
    {
        // get data for current iteration 
        char * id = getCharVec(&table->identifier, i);
        char * type = getCharVec(&table->type, i);
        char * arr = getCharVec(&table->array, i);
        char * args = getCharVec(&table->funcArgs, i);
        char * action = getCharVec(&table->action, i);
        char * scope = getCharVec(&table->scope, i);

        printf("Row %d: %s, %s, %s, %s, %s, %s\n", (i+1), id, type, arr, args, action, scope);
    }
    
}

//******NOTES we need to be able to push that array data and then update all the function calls

// will be used to create an entry within our table
void pushTable(Table * table, char * id, char * type, char* arr, char * args, char * action, char * scope)
{

    // add the data to the table then we increment the size of our table
    pushCharVec(&table->identifier, id);
    pushCharVec(&table->type, type);
    pushCharVec(&table->array, arr);
    pushCharVec(&table->funcArgs, args);
    pushCharVec(&table->action, action);
    pushCharVec(&table->scope, scope);

    table->size++;
}

// fucntion will delete our whole symbol table tree to prevent memory leaks
void freeSymbolTable(Table * table)
{
    // first we call recursively to the children to delete themselves
    for (int i = 0; i < table->numChildren; i++)
    {
        freeSymbolTable(table->children[i]);
    }
    
    // then free the current tables containers
    freeCharVec(&table->identifier);
    freeCharVec(&table->type);
    freeCharVec(&table->array);
    freeCharVec(&table->action);
    freeCharVec(&table->funcArgs);
    freeCharVec(&table->scope);

    // once all children are free the child container can be freed
    free(table->children);

}

void copyTableHelper(charVec * vec1, charVec * vec2)
{
    // make room for all strings 
    vec1->container =  calloc(vec2->size,  sizeof(char*));

    if (vec1 == NULL)
    {       
        printf("ERROR: could not reallocate vector data!\n");
        return;
    }

    // copy strings to the new vector
    for (int i = 0; i < vec2->size; i++)
    {
        vec1->container[i] =  calloc(maxStringLen, sizeof(char));       // make room for strings in new vec
        strcpy(vec1->container[i], vec2->container[i]);                 //copy strings over
    }    

    // move size attribute over to new vector and free the old one
    vec1->size = vec2->size;
    freeCharVec(vec2);

}

void copyTable(Table * table1, Table * table2)
{

    /*  in this function we need to allocate room in the new table charVec
        and copy the data to the appropriate charVec */

    copyTableHelper(&table1->identifier, &table2->identifier);
    copyTableHelper(&table1->type, &table2->type);
    copyTableHelper(&table1->array, &table2->array);
    copyTableHelper(&table1->funcArgs, &table2->funcArgs);
    copyTableHelper(&table1->action, &table2->action);
    copyTableHelper(&table1->scope, &table2->scope);

    table1->size = table2->size;        //update size of new table

}

//index starting at 0
void printSymbolTableRow(Table * table, int rowNum)
{

    // get data for current iteration 
    char * id = getCharVec(&table->identifier, rowNum);
    char * type = getCharVec(&table->type, rowNum);
    char * arr = getCharVec(&table->array, rowNum);
    char * args = getCharVec(&table->funcArgs, rowNum);
    char * action = getCharVec(&table->action, rowNum);
    char * scope = getCharVec(&table->scope, rowNum);

    printf("Row %d: %s, %s, %s, %s, %s, %s\n", (rowNum+1), id, type, arr, args, action, scope);
}

void printTableTree(Table * table, int depth)
{

    if (table->size == 0)
    {

        for (int j = 0; j < depth; j++)
        {
            printf("\t");
        }
        
        printf("Printing Empty Table\n");
    }
    

    // print the current table row
    for (int row = 0; row < table->size; row++)
    {   
        for (int j = 0; j < depth; j++) //formatting so we can see tree level a bit better
        {
            printf("\t");
        }   

        printSymbolTableRow(table, row);
    }
    
    printf("#ch:%d\n", table->numChildren);

    //update the depth because after this the children will be called upon
    depth++;
    
    //print the rest of the children recursively
    for (int i = 0; i < table->numChildren; i++)
    {
        printTableTree(table->children[i], depth);
    }
    
}

Table* addSubTable(Table * table)
{

    // Create a temporary variable so we can create room for another table in the children
    Table ** temp; 
    temp = calloc(table->numChildren+1, sizeof(Table*));    // allocate room for one more subtable

    // copy over the children to the new container
    for (int i = 0; i < table->numChildren; i++)
    {
        temp[i] = calloc(1, sizeof(Table));     // allocate room for one table
        copyTable(temp[i], table->children[i]); // need this to copy char vecs of old table to the new one
        temp[i]->parent = table;
    }

    // allocate one more for the new table
    temp[table->numChildren] = calloc(1, sizeof(Table));

    // free the old children & the child container
    for (int j = 0; j < table->numChildren; j++)
    {
        free(table->children[j]);       // free individual children pointer
    }
    free(table->children);              // free pointer to children pointers

    // update child container with the new one we made
    table->children = temp;

    //initalize the new tabble we are adding
    initSymbolTable(table->children[table->numChildren]);

    // update the new child tables parent pointer
    (table->children[table->numChildren])->parent = table;

    //increment child counter 
    table->numChildren++;

    // return the new table we just created so we can update our table pointer
    return table->children[table->numChildren-1];  

    /*
    if(table->numChildren == 0)
    {
        //Create room for a table pointer
        table->children = malloc(table->numChildren+1 * sizeof(Table*));
        table->children[table->numChildren] = (Table*) malloc(1 * sizeof(Table));     //allocate room for table in table pointer
    } 

    // reallocate the data with room for one more table pointer 
    else
    {
        table->children = realloc(table->children, table->numChildren+1 * sizeof(Table*));
        table->children[table->numChildren] = (Table*) malloc(1*sizeof(Table));
    }

    // increment child counter
    table->numChildren++;

    // return the new table we just created so we can update our table pointer
    return table->children[table->numChildren];
    */

}


//////////////////////////////////////////////////////////////////////
//////////////////////Symbol Table Creation Methods///////////////////
//////////////////////////////////////////////////////////////////////


/*  When we use a variable we need to check it has been declared so we iterate 
    our current table and its parent tables to see if it is in scope*/
void findVarDecl(Table * table, char * id, charVec * container)
{

    // iterate through the table rows
    for (int i = 0; i < table->size; i++)
    {
        // if we find the identifier we are looking for & it's declaration 
        if (!strcmp(getCharVec(&table->identifier, i), id) && !strcmp(getCharVec(&table->action, i), "Variable Declaration"))
        {
            pushCharVec(container, getCharVec(&table->type, i));

            //return getCharVec(&table->type, i);      // return the type we are looking for 
        }
    }

    //if we did not find the id we go to the parent to search (unless that parent is the global table)
    if (table->parent == NULL)
    {
        return;
    }
    
    // go to the parent table and search that table
    else
    {
        findVarDecl(table->parent, id, container);
    }
    
    // if for some reason we make it out here
    return;
}

int findDecl(Table * table, char * id)
{

    // iterate through the table rows
    for (int i = 0; i < table->size; i++)
    {
        // if we find the identifier we are looking for 
        if (!strcmp(getCharVec(&table->identifier, i), id))
        {
            return 1;
        }
    }

    // if we could not find the id in the current table we chcek if there is a parent and go to that table
    if (table->parent != NULL)
    {
        return findDecl(table->parent, id);     // look for what is parent table and return that
    }

    // if we cannot go further up the symbolTable tree then the id was never declared
    return 0;
}

// will search table for the identifier
char * getType(Table * table, char * id)
{

    // iterate through the table rows
    for (int i = 0; i < table->size; i++)
    {
        // if we find the identifier we are looking for & it's declaration 
        if (!strcmp(getCharVec(&table->identifier, i), id))
        {
            return getCharVec(&table->type, i);      // return the type we are looking for 
        }
    }

    //if we did not find the id we go to the parent to search (unless that parent is the global table)
    if (table->parent == NULL)
    {
        printf("test2\n");
        return NULL;
    }

    // go to the parent table and search that table
    else
    {
        getType(table->parent, id);
    }
    
    printf("test2\n");

    // if for some reason we make it out here
    return NULL;
}

// This function will collect all of the terminals from the expression to type check
void getExpr(struct Node * node, charVec * vector)
{

    // if the node has not even one child then we knwo it is a terminal
    if(node->one == NULL)
    {

        //push that nodes val
        if (node->data[0] != '\0')
        {
            pushCharVec(vector, node->data);    
        }
        else if (node->symbol != 'e')
        {
            pushCharVec(vector, &node->symbol);
        }

    }

    // Below are recursive calls to the children nodes if they exist 
    if(node->one)
    {
       getExpr(node->one, vector);
    }
    if(node->two)
    {
        getExpr(node->two, vector);
    }
    if(node->three)
    {
        getExpr(node->three, vector);
    }
    if(node->four)
    {
        getExpr(node->four, vector);
    }
    if(node->five)
    {
        getExpr(node->five, vector);
    }
    if(node->six)
    {
        getExpr(node->six, vector);
    }
    if(node->seven)
    {
        getExpr(node->seven, vector);
    }
    
}

// will check the current table for if a declaration already exists
int doubleDeclaration(Table * table, char * id)
{
    for (int i = 0; i < table->size; i++)
    {
        if(!strcmp(getCharVec(&table->identifier, i), id))
        {
            return 1;
        }
    }

    return 0;
    
}

/*  Here we will take the leaves of the parameters to find out what type they are and add it to 
    our table, the function will work off of the commas used to seperate parameters */
void formatArgTypes(charVec * vector)
{

    // to place our types in then we will move it to the actual vector container
    char temp[20];
    memset(temp, '\0', sizeof(temp));

    // if theres nothing to format leave function
    if (vector->size == 0)
    {
        return;
    }

    // handling case there is only one arg, this is done becasue there are no commas to help with iteraton
    else if (vector->size < 5)
    {
        // regular arg
        if (vector->size == 2)
        {
            strcat(temp, vector->container[0]);
        }
        // array 
        else if (vector->size == 4)
        {
            // now we have to check what type of array it is
            if (!strcmp(vector->container[0], "#"))
            {
                strcat(temp, "[");          // we push this symbol to note that it is a integer array  
            }
            else if (!strcmp(vector->container[0], "c"))
            {
                strcat(temp, "]");          // we push this symbol to note that it is a character array  
            }
        }
    }
    // here we hanfle if there is n=more than one argument
    else
    {
        for (int i = 0; i < vector->size; i++)
        {
            
            // once we encounter a comma we look back one poistion to see if the arg is an array or not (or if end of array)
            if (!strcmp(vector->container[i], ",") || i == vector->size-1)
            {
                
                // if the argument is an array
                if (!strcmp(vector->container[i-1], "]"))
                {
                    // we then check what type of array it is
                    if (!strcmp(vector->container[i-4], "#"))
                    {
                        strcat(temp, "[");           
                    }
                    else if (!strcmp(vector->container[i-4], "c"))
                    {
                        strcat(temp, "]");          
                    }
                }
                
                // if its a normal int or char record type from container
                else
                {
                    if (i == vector->size-1)
                    {
                        strcat(temp, vector->container[i-1]);
                    }
                    else
                    {
                        strcat(temp, vector->container[i-2]);
                    }
                }
            }
        }
        
    }
    
    // once all the args are in the temp container we can copy them over to the actual container
    strcpy(vector->container[0], temp);

}

// will add the parameters of a function declaration to the symbol table
void addArgsToTable(Table * table, charVec * vector, char * scope)
{
    // used to index in string where we have already formatted the types of data
    int argCounter = 0;
    char type[2] = "\0\0";  // temp container to get type char (\0 in pos[1] helps identify end of string)

    for (int i = 0; i < vector->size; i++)
    {
        // this helps us find the name of the identifier in the charVec
        if (i == 0 || !strcmp(vector->container[i], "#") || !strcmp(vector->container[i], "c"))
        {
            type[0] = vector->container[0][argCounter];
            pushTable(table, vector->container[i+1], type, "N/A", "N/A", "Function Parameter", scope);
            argCounter++;       // increment so we can go to next type in index[0]
        }    
    }

}

// will take number and add it to the beginning of function name
char* appendBlockNum(int bNum, char * id)
{
    char * token;

    if (isdigit(id[0]))
    {
        token = strtok(id, ":"); // point to numbers section of scope
        token = strtok(NULL, " ");  // point to id name section of scope
    }
    else
    {
        token = id;
    }
    

    char * tempName = calloc(35, sizeof(char));
    sprintf(tempName, "%d", bNum);
    strcat(tempName, ":");   // some formatting   
    strcat(tempName, token);
    return tempName;
}

// will get current pointer to table and return the last scope value
int getScopeNum(Table * table)
{
  
  // check if we are at root func def
  if (!strcmp(getCharVec(&table->parent->scope, 0), "Global"))
  {
      // if this is the first scop to be made for this funciton 
      if (table->numChildren == 0)
      {
          return 1;     // this means this is the irst scope to be created
      }
      else
      {
        // if there are already children then we look at the last one
        // and any ther scopes within that one

        Table * tempTablePtr = table->children[table->numChildren-1];

        // will get us to the furthest scope (meaning the last one made)
        while (tempTablePtr->numChildren != 0)
        {
            tempTablePtr = tempTablePtr->children[tempTablePtr->numChildren-1];
        }
        
        // find out what block nunmber needs to come next
        char tempBlockNum[4];       // allow up to 999 blocks
        memset(tempBlockNum, '\0', sizeof(tempBlockNum));

        //printf("test1\n");

        int i = 0;  // will be used for indexing scope name
        // will extract block number for me from scope
        while (isdigit(getCharVec(&tempTablePtr->scope, 0)[i]))
        {
            tempBlockNum[i] = getCharVec(&tempTablePtr->scope, 0)[i];
            i++;
        }

        //printf("test end\n");

        // once the number section has been extracted convert to int and return +1 so the call will know what the next scope should be
        return atoi(tempBlockNum)+1;
    }
            
  }
  // this means we are in some scope already
  else
  {

    // if we are already in a scope check if there were other scopes 
    // declared before this current one
    if (table->numChildren == 0)
      {
        // find out what block nunmber needs to come next
        char tempBlockNum[4];       // allow up to 999 blocks
        memset(tempBlockNum, '\0', sizeof(tempBlockNum));

        int i = 0;  // will be used for indexing scope name
        // will extract block number for me from scope
        while (isdigit(getCharVec(&table->scope, 0)[i]))
        {
            printf("i: %d\n", i);
            tempBlockNum[i] = getCharVec(&table->scope, 0)[i];
            i++;
        }
        return atoi(tempBlockNum)+1;
      }
      else
      {
        // if there are already children then we look at the last one
        // and any ther scopes within that one

        Table * tempTablePtr = table->children[table->numChildren-1];

        // will get us to the furthest scope (meaning the last one made)
        while (tempTablePtr->numChildren != 0)
        {
            tempTablePtr = tempTablePtr->children[tempTablePtr->numChildren-1];
        }
        
        // find out what block nunmber needs to come next
        char tempBlockNum[4];       // allow up to 999 blocks
        memset(tempBlockNum, '\0', sizeof(tempBlockNum));

        int i = 0;  // will be used for indexing scope name
        // will extract block number for me from scope
        while (isdigit(getCharVec(&tempTablePtr->scope, 0)[i]))
        {
            printf("i: %d\n", i);
            tempBlockNum[i] = getCharVec(&tempTablePtr->scope, 0)[i];
            i++;
        }

        // once the number section has been extracted convert to int and return +1 so the call will know what the next scope should be
        return atoi(tempBlockNum)+1;
    }
  } 
}

// recursive function that will create our table
int createSymbolTable(Table * table, struct Node * root, char * currentScope, charVec * container)
{
    // used to prevent errors of wrong scope on table
    if (table->size > 0)
    {
        strcpy(currentScope, getCharVec(&table->scope, 0)); 
    }
    

    // if we find a variable decleration we add it to the table
    if (root->symbol == 'W')
    {

        // first check for double declarations in same scope
        if (doubleDeclaration(table, root->two->data))
        {
            printf("Variable cannot be declared twice within the same scope --> %s\n", root->two->data);
            return 0;
        }
        
        /*  push to the table the data from the tree and currentScope passed from the recursive call
            the scope will change when we find a block and will then effect the next recursive calls*/

        if (root->three->one->symbol == ';')
        {
            pushTable(table, root->two->data, &root->one->one->symbol, "N/A", "N/A" , "Variable Declaration", currentScope);
        }
        // if the varDecl is an array we record that and its size
        else
        {
            // first we check if the array declaration is not of size 0
            if(!strcmp(root->three->two->data, "0"))
            {
                printf("ERROR: %s array may not be of size 0\n", root->two->data);
                return 0;             // end table creation becasue there is an error;
            }

            // Then we check if it is a interger array or character array
            if (root->one->one->symbol == '#')
            {
                pushTable(table, root->two->data, "[", root->three->two->data, "N/A", "Variable Declaration", currentScope);
            }
            else if (root->one->one->symbol == 'c')     // if character array
            {
                pushTable(table, root->two->data, "]", root->three->two->data, "N/A", "Variable Declaration", currentScope);
            }   
        }     
    }

    // if we reach a function declaration node we  add it to the table & update the scope
    else if (root->symbol == 'G')
    {
        clearCharVec(container);                // clear container before we use it

        // first check for double declarations in same scope
        if (doubleDeclaration(table, root->two->data))
        {
            printf("Function cannot be declared twice within the same scope --> %s\n", root->two->data);
            return 0;
        }

        // if the parameter decl list is not empty 
        if(root->four->one->symbol != 'e')
        {
            getExpr(root->four, container);      // reusing getExpr function to get terminals of node
            formatArgTypes(container);              // get parameter types and format them for the table
            pushTable(table, root->two->data, &root->one->one->symbol, "N/A", container->container[0], "Function Declaration", currentScope);
            strcpy(currentScope, root->two->data);         // update the scope
            addSubTable(table);                     // create table child
            table = table->children[table->numChildren-1];    // go to the table we just created (-1 for indexing purposes)
            addArgsToTable(table, container, currentScope);        
        }
        else
        {
            pushTable(table, root->two->data, &root->one->one->symbol, "N/A", "N/A", "Function Declaration", currentScope);
            strcpy(currentScope, root->two->data);         // update the scope
            addSubTable(table);                     // create table child
            table = table->children[table->numChildren-1];    // go to the table we just created (-1 for indexing purposes)        
        }
        
    }

   // will make sure any time an identifier is used that it has been already declared
   // also we dont check paramDecl identifiers becasuse those do not need to de declared elsewhere
   else if (root->symbol == 'i' && root->parent->symbol != 'Q')     
   {
       // if we cannot dinf the id once it has been used then we issue an error 
       if (findDecl(table, root->data) == 0)
       {
           printf("ERROR: Could not find declartaion for --> %s\n", root->data);
           return 0;
       }
   }
   
   /*
   // for any blocks we encounter they must have theri own scope
   else if (root->symbol == 'B' && root->parent->symbol != 'G')
   {
        
        //printf("Need to create new scope --> %c\nParent: --> %s\n", root->parent->symbol, getCharVec(&table->parent->scope, 0));
        //printTableTree(table->parent, 0);
        //printf("Current: %s |", getCharVec(&table->scope, 0));

        // check if there is a scope within the function already
        int nextScope = getScopeNum(table);  

        // Create new scope and go to that table 
        //strcpy(currentScope, appendBlockNum(tempBlockNum, currentScope));
        currentScope = appendBlockNum(nextScope, currentScope);
        
        addSubTable(table);
        //printf("num ch: %d\n", table->numChildren);

        table->numChildren = nextScope;     // ensure the number of children is correct
        table = table->children[table->numChildren-1];
        pushTable(table, "N/A", "N/A", "N/A", "N/A", "Block Scope", currentScope);
        free(currentScope);

        //printf("\nNew Scope:\n");
        //printTableTree(table, 0);    
   }
   */
   

    // Below are recursive calls to the children nodes if they exist and a check for any errors to report back to the main function call 
    if(root->one)
    {
        if(createSymbolTable(table, root->one, currentScope, container) == 0)
        {
           return 0;
        }
    }
    if(root->two)
    {
        if(createSymbolTable(table, root->two, currentScope, container) == 0)
        {
           return 0;
        }
    }
    if(root->three)
    {
        if(createSymbolTable(table, root->three, currentScope, container) == 0)
        {
           return 0;
        }
    }
    if(root->four)
    {
        if(createSymbolTable(table, root->four, currentScope, container) == 0)
        {
           return 0;
        }
    }
    if(root->five)
    {
        if(createSymbolTable(table, root->five, currentScope, container) == 0)
        {
           return 0;
        }
    }
    if(root->six)
    {
        if(createSymbolTable(table, root->six, currentScope, container) == 0)
        {
           return 0;
        }
    }
    if(root->seven)
    {
        if(createSymbolTable(table, root->seven, currentScope, container) == 0)
        {
           return 0;
        }
    }

    
    // if we make it down here the execution was sucessful
    return 1;

}

// the program will only run if there is a main(), this will check for it
int isMain(Table * table)
{

    // here we only iterate through the global table becasue main should exist there
    for (int i = 0; i < table->size; i++)
    {
        if (!strcmp(getCharVec(&table->identifier, i), "main") && !strcmp(getCharVec(&table->action, i), "Function Declaration"))
        {

            // additionally we must check main has no parameters
            if (!strcmp(getCharVec(&table->funcArgs, i), "N/A"))
            {
                return 1;       // return a succesful find
            }
            else
            {
                return 2;
            }
            
            
            
        }
        
    }
    
    // if we make it out here that means there is no main
    return 0;               // return failure

}


//////////////////////////////////////////////////////////////////////
///////////////////////////////AST Methods////////////////////////////
//////////////////////////////////////////////////////////////////////

// adds AST node to the tree
AST_Node* newAST(char * newData)
{

    // allocate memory for the new Node
    AST_Node * newNode = malloc(1 * sizeof(AST_Node));
    memset(newNode->data, '\0', sizeof(newNode->data));

    if (strlen(newData) == 1)
    {
        newNode->data[0] = newData[0];
        newNode->data[1] = '\0';
    }
    else
    {
        // add data to node
        strcpy(newNode->data, newData);
    }
    
   //strcpy(newNode->data, newData);
    
    // set default values for new node
    newNode->parent = NULL;
    newNode->children = NULL;
    newNode->numChildren = 0;

    // return the new node we created so we can use it
    return newNode;


}

void copyNode(AST_Node * node1, AST_Node * node2)
{

    if (strlen(node2->data) == 1)
    {
        node1->data[0] = node2->data[0];
        node1->data[1] = '\0';
    }
    else
    {
        // add data to node
        strcpy(node1->data, node2->data);
    }

    node1->parent = node2->parent;
    node1->children = node2->children;
    node1->numChildren = node2->numChildren;
}

void addChild(AST_Node * parent, AST_Node * child)
{
    // create temporary array of pointers container, but one child size larger
    AST_Node ** temp = malloc(parent->numChildren+1 * sizeof(AST_Node*));   // allocate room fro one more pointer

    char indexZeroBackup[20];       // for issue i have with the first index in the dynamic array

    // copy the children of the old container to the new one
    for (int i = 0; i < parent->numChildren; i++)
    {

        // creating a backup of the first index just incase
        if (i == 0)
        {
            // trying solution
            memset(indexZeroBackup, '\0', sizeof(indexZeroBackup));
            strcpy(indexZeroBackup, parent->children[i]->data);
        }
        

        printf("Before Copy: %s\n", parent->children[i]->data);
        temp[i] = malloc(1 * sizeof(AST_Node));         // allocate memory for one Node where our pointer is pointing to 
        copyNode(temp[i], parent->children[i]);                  // copy data to new node
        printf("After Copy: %s\n", parent->children[i]->data);
    }

    // add new child to children (no need to allocate memory for new child because that was done before)
    temp[parent->numChildren] = child;
    child->parent = parent;             //update parent pointer

    printf("Before Free: \n");
 
    for(int p = 0; p < parent->numChildren; p++)
    {
        printf("%s\n", parent->children[p]->data);
    }

    // free old child container and update to new one
    free(parent->children);
    parent->children = temp;

    printf("After Free: \n");

    for(int j = 0; j < parent->numChildren; j++)
    {
        printf("%s\n", parent->children[j]->data);
    }

    // check if the data was lost (but first we check if this parent has any children to check)
    if (parent->numChildren > 0)
    {
        // if the strings are no the same oafter the free 
        if(strcmp(parent->children[0]->data, indexZeroBackup))
        {
            // fix the problem and continue
            /*
            AST_Node * tempNode = newAST(indexZeroBackup);
            tempNode->numChildren = parent->children[0]->numChildren;
            tempNode->parent = parent;
            tempNode->children = parent->children[0]->children;
            */

            //strcpy(parent->children[0]->data, indexZeroBackup);

            //free(parent->children[0]);
            //parent->children[0] = tempNode;
            
        }
    }

    printf("After fix: \n");
    for(int q = 0; q < parent->numChildren; q++)
    {
        printf("%s\n", parent->children[q]->data);
    }

    //increment child counter for parent
    parent->numChildren++;

    
}

void addChild2(AST_Node * parent, char * newChild)
{

    //printf("Adding child: %s to parent: %s\n", newChild, parent->data);

    // allocate one more pointer container and initialize the nodes within
    AST_Node ** temp = calloc(parent->numChildren+1, sizeof(AST_Node*));

    // init container and tables they point to
    for (int i = 0; i < parent->numChildren+1; i++)
    {
        temp[i] = calloc(1, sizeof(AST_Node));
    }
    
    //copy all the old data and add new child
    for (int i = 0; i < parent->numChildren+1; i++)
    {
        if (i == parent->numChildren)
        {
            //copyNode(temp[i], child);
            strcpy(temp[i]->data, newChild);
            temp[i]->parent = parent;
            temp[i]->numChildren = 0;
            //printf("Adding: %s -> %s\n", parent->data, temp[i]->data);
        }
        else
        {
            copyNode(temp[i], parent->children[i]); 
            free(parent->children[i]);      // free child we just copied
        }
    
    }

    // free old container
    free(parent->children);

    // assign temp container as new parent child container
    parent->children = temp;

    //printf("New Add: %s\n", parent->children[parent->numChildren]->data);

    //increment counter
    parent->numChildren++;


}

void printAST(AST_Node * root, int depth)
{
    // before we print do some formatting to read the tree better
    for (int i = 0; i < depth; i++)
    {
        //printf("    ");
        printf("    ");
    }

    // increment depth casue all other function calls will be one layer deeper
    depth++;

    // Print current node then children all of its children
    //printf("%s : %d\n", root->data, root->numChildren);
    printf("%s\n", root->data);

    for (int i = 0; i < root->numChildren; i++)
    {
        printAST(root->children[i], depth);
    }
    
    
}

void deleteAST(AST_Node * root)
{
    //recursive call to children to delete their children then delete themselves
    for (int i = 0; i < root->numChildren; i++)
    {
        deleteAST(root->children[i]);
        //printf("Trying to free: %s\n", root->children[i]->data);
        free(root->children[i]);
    }
    
    //free child container
    free(root->children);

    // free node after its children have been freed
    //free(root);

}

// Global variables used in functions below
char * removableNodes = "GWIZ2Q3N{}7SXYe8()"; 

int removable(struct Node * node)
{
    
    // Will check the nodes we are allowed to remove and return either true or false 
    for(int i = 0; i < strlen(removableNodes); i++)
    {
        if(node->symbol == removableNodes[i])
        {
            return 1;
        }
    }
    // return false if no match
    return 0;
}

void createAST(struct Node * pRoot, AST_Node * aRoot);  //forward declaration 

/// function is used to update pointer around specific node and delete it
void removeG(struct Node * pNode, AST_Node * aNode)
{

    // To remoeve the node we first need to connect its children to its own parent
    
    
    //move function name to G's position
    strcpy(aNode->data, pNode->two->data);

    //Move parameters to the left side of function name 
    addChild2(aNode, &pNode->four->symbol);
    //printf("New Params's Data: %s\n", aNode->children[0]->data);
    createAST(pNode->four,aNode->children[0]);      //Continue and let tree grow left side

    //move block to right side of function node
    addChild2(aNode->parent, &pNode->six->symbol);
    //printf("functions right side: %s\n", aNode->parent->children[1]->data);
    createAST(pNode->six, aNode->parent->children[aNode->parent->numChildren-1]);

   // printf("Tesst\n");


    /*
    printf("Attempting to remove %c\n", node->symbol);

    //get position of our node so we can insert its children into that position
    int nodeIndex = getChildNum(node);

    //Check if we need to shift children or not (is the node to the left of any children)
    int shiftBool = needShift(nodeIndex, node);

    // now if we need to shift can we? (check number of children and empty child spaces)
    int numNodeChildren = countChildren(node);
    int numParentChildren = countChildren(node->parent);

    // if there is enough room to fit all the children
    if ((numNodeChildren + numParentChildren) <= 7)
    {
        // first we make space for the children on the parent layer
        shiftChildren(nodeIndex, numNodeChildren, node);

    }
    else    
    {
        printf("Too many children to make node replacement!\n");
        return;     //leave function becasue we cant adjust the children in the tree 
    }
    

    // update children parent pointer & update parent with new children if there is a child there
     if(node->one)
    {
        node->one->parent = node->parent;
        node->parent->one = node->one;
    }
    if(node->two)
    {
        node->two->parent = node->parent;
        node->parent->two = node->two;
    }
    if(node->three)
    {
        node->three->parent = node->parent;
        node->parent->three = node->three;
    }
    if(node->four)
    {
       node->four->parent = node->parent;
       node->parent->four = node->four;
    }
    if(node->five)
    {
        node->five->parent = node->parent;
        node->parent->five = node->five;
    }
    if(node->six)
    {
        node->six->parent = node->parent;
        node->parent->six = node->six;
    }
    if(node->seven)
    {
        node->seven->parent = node->parent;
        node->parent->seven = node->seven;
    }

    // when done updating pointer free the node from memory 
    printf("Freeing Node: %c\n", node->symbol);
    free(node);
    */
}

// remove varDecl
void removeW(struct Node * pNode, AST_Node * aNode)
{
    // this fix is easy we just replace it with the variable name there isnt even a
    // need to continue the recursive call becasue the nodes end past the data
    strcpy(aNode->data, pNode->two->data);

}

// pointelss to have these dead end nodes in the tree
void remove_e(struct Node * pNode, AST_Node * aNode)
{
    aNode = aNode->parent->parent;                          // move pointer to parent of parent
    free(aNode->children[aNode->numChildren-1]->children[0]);   // free child "e"
    free(aNode->children[aNode->numChildren-1]->children);      // free dynamic container 
    free(aNode->children[aNode->numChildren-1]);                // free nonterminal that parented "e"
    aNode->numChildren--;                                     //update number of children parent has
}

// removing primary node and adjusting tree
void removeI(struct Node * pNode, AST_Node * aNode)
{
    // best case we pull just the variable name
    if (pNode->one->symbol != '(')
    {
        // move data to expression position
        strcpy(aNode->parent->data, pNode->one->data);

        if (pNode->two != NULL)     // exprexted Primary`,move primary to old I position
        {
            strcpy(aNode->data, &pNode->two->symbol);
            createAST(pNode->two, aNode);       // continue with recursive call
        }
        else    // remove I node becasue it is not needed
        {
            aNode->parent->numChildren--;
            free(aNode);
        }
        
    }
    
    // in the case an expression goes to an expression with parenthesis, we just move that expression to replace the original expression
    else
    {

        //add children then call to recursive AST function to execute rest of expression
        //addChild2(aNode, &pNode->one->symbol);
        strcpy(aNode->parent->data, pNode->two->data);

        // let parent know child is going to be removed
        aNode->parent->numChildren--;
        free(aNode->children);
        free(aNode);

        //addChild2(aNode, pNode->two->data);
        //addChild2(aNode, &pNode->three->symbol);

        // continue with normal tree creation
        //createAST(pNode->two, aNode->children[1]);
        createAST(pNode->two, aNode->parent);
    }
    
}

// remove asmt non terminal 
void removeZ(struct Node * pNode, AST_Node * aNode)
{
    // move = operator to Z's position
    // check which position it is in
    if (pNode->two->one->symbol == '=')
    {

        // this pointer will be static during the moving of nodes so we use it for refrencing
        aNode = aNode->parent;

        strcpy(aNode->data, "=");       // S -> =


        strcpy(aNode->children[0]->data, pNode->one->data);         //  add left side of equal sign
        addChild2(aNode, &pNode->two->two->symbol);          //  add expr
        addChild2(aNode, &pNode->two->three->symbol);        // and expr'

        //continue recursive calls
        createAST(pNode->two->two, aNode->children[1]);
        createAST(pNode->two->three, aNode->children[2]);


    }
    else    //will handle id it is the fourth child
    {

        aNode = aNode->parent;

        strcpy(aNode->data, "=");
        strcpy(aNode->children[0]->data, pNode->one->data);
        addChild2(aNode->children[0], &pNode->two->two->symbol);     // Expr (for index of array)
        addChild2(aNode, &pNode->two->five->symbol);    // Expr
        addChild2(aNode, &pNode->two->six->symbol);     // Expr`

        //continue recursive calls
        createAST(pNode->two->two, aNode->children[0]->children[0]);
        createAST(pNode->two->five, aNode->children[1]);
        createAST(pNode->two->six, aNode->children[2]);
    }
    
    
}

// Removing FunDeclList` (replacing with function if the 2 does not go to epsilon)
void remove2(struct Node * pNode, AST_Node * aNode)
{
    // if there is a function so we can simplify the tree we will move the Nodes around
    if (pNode->one->symbol == 'F')
    {
       // move the function on the same layer as the other function definitons

        aNode = aNode->parent; // user parent pointer becasue it wil not change during the tree maniupulation

        // remove the 2 as the child of the parent
        free(aNode->children[aNode->numChildren-1]);
        aNode->numChildren--;           // let parent know child was released

        // go to parent of F so we can move the next function declaration to the same level
        aNode = aNode->parent;

        addChild2(aNode, &pNode->one->symbol);      // add function to same level 
        createAST(pNode->one, aNode->children[aNode->numChildren-1]);


    }
    else if(pNode->one->symbol == 'e')        // if 2 happens to go to epsilon we just wont add it to the AST
    {
        aNode->parent->numChildren--;       // let parent know it has one less child now
        free(aNode);                        // free the 2 node becasue we wont be needing it anymore
    }
    
    
}

// will remove the ParamDecl non terminal node
void removeQ(struct Node * pNode, AST_Node * aNode)
{
    // Q will always be followed by a R so we will reduce the tree to that node
    strcpy(aNode->parent->data, pNode->two->data);  // move var name to R postition
    aNode->parent->numChildren--;                   // let parent know we are removing Q
    free(aNode);                                    // free Q

}

// will remove paramDeclListTail`
void remove3(struct Node * pNode, AST_Node * aNode)
{
    // if there is no epsilon transition then we move R to its position
    if (pNode->one->symbol != 'e')
    {
        strcpy(aNode->data, &pNode->two->symbol);
        createAST(pNode->two, aNode);       //continue with normal tree building
    }
    else    // if we do have an epsilon just remove the Node
    {
        aNode->parent->numChildren--;
        free(aNode);
    }
    
    
}

// will try to simplify when binary operators are used
void removeN(struct Node * pNode, AST_Node * aNode)
{
    //move binary operator to the N position, but first we decode it and add to our AST node
    char * needDecode = "qzgh&|";

    // if Expr` ends
    if (pNode->one->symbol == 'e')
    {
        aNode->parent->numChildren--;   //let parent know it has one less child
        free(aNode);                     // free N node
    }
    else 
    {
        char decoded[3];                            // temp container
        memset(decoded, '\0', sizeof(decoded));     // make sure array is empty

        if (strchr(needDecode, pNode->one->one->symbol) != NULL)   //if its a node who's symbol doesnt represent them well
        {

            switch (pNode->one->one->symbol)
            {
                case 'q':
                    strcpy(decoded, "==");
                    break;
                case 'z':
                    strcpy(decoded, "!=");
                    break;
                case 'g':
                    strcpy(decoded, "<=");
                    break;
                case 'h':
                    strcpy(decoded, ">=");
                    break;
                case '&':
                    strcpy(decoded, "&&");
                    break;
                case '|':
                    strcpy(decoded, "||");
                    break;
            }
        }
        else        // if symbol was the operator it represented
        {
            strcpy(decoded, &pNode->one->one->symbol);
        }

        // make N the operator
        strcpy(aNode->data, decoded);
        addChild2(aNode, &pNode->two->symbol);      //move possible expressions over
        addChild2(aNode, &pNode->three->symbol);

        createAST(pNode->two, aNode->children[0]);
        createAST(pNode->three, aNode->children[1]);
    }    

    

}

// will remove brackets from the AST
void removeBrackets(struct Node * pNode, AST_Node * aNode)
{

    // let parent know we are removing a child
    aNode->parent->numChildren--;

    // free the data allocated for that node
    free(aNode->children);
    free(aNode);

    // no need to do anything else becasue the { or } are terminals

}

// will handle if some primary has any more arguments after it (for params or indexing)
void remove7(struct Node * pNode, AST_Node * aNode)
{

    if (pNode->one->symbol != 'e')
    {
        // the primary` node is just going to move its children to the identifier children
        
        strcpy(aNode->data, &pNode->one->symbol);       // move left paren or bracket to 7's pos

        aNode = aNode->parent;                          // use id's pointer becasue it will remain static

        addChild2(aNode, &pNode->two->symbol);          // add expr or exprList
        //addChild2(aNode, &pNode->three->symbol);        // add ) or ]

        createAST(pNode->two, aNode->children[1]);          // continue building expression 
        

    }
    else
    {
        // let parent know child is going to be removed
        aNode->parent->numChildren--;
        free(aNode->children);
        free(aNode);
    }

}

// will re format the if statement 
void format_f(struct Node * pNode, AST_Node * aNode)
{

    // if we replace the Stmt becasue an if can only be derived from a Stmt node
    strcpy(aNode->data, "if");

    // add boolean expr to the left side of the if, Stmt in the midle and else stmt on the right
    addChild2(aNode, &pNode->three->symbol);
    addChild2(aNode, &pNode->five->symbol);
    addChild2(aNode, &pNode->seven->symbol);

    // continue with growing the tree
    createAST(pNode->three, aNode->children[0]);
    createAST(pNode->five, aNode->children[1]);
    createAST(pNode->seven, aNode->children[2]);

}

void format_r(struct Node * pNode, AST_Node * aNode)
{

    // let return take the place of stmt
    strcpy(aNode->data, "return");

    //add the the expression child then the semicolon
    addChild2(aNode, &pNode->two->symbol);
    addChild2(aNode, &pNode->three->symbol);

    //continue growing expression in AST
    createAST(pNode->two, aNode->children[0]);

}

void format_d(struct Node * pNode, AST_Node * aNode)
{
    // let return take the place of stmt
    strcpy(aNode->data, "read");

    //add the the expression child then the semicolon
    addChild2(aNode, pNode->two->data);
    addChild2(aNode, &pNode->three->symbol);
}

void format_w(struct Node * pNode, AST_Node * aNode)
{

    // let return take the place of stmt
    strcpy(aNode->data, "write");

    //add the the expression child then the semicolon
    addChild2(aNode, &pNode->two->symbol);
    addChild2(aNode, &pNode->three->symbol);

    //continue growing expression in AST
    createAST(pNode->two, aNode->children[0]);

}

void format_writeln(struct Node * pNode, AST_Node * aNode)
{
    strcpy(aNode->data, "writeln");
}

void format_u(struct Node * pNode, AST_Node * aNode)
{
    strcpy(aNode->data, "while");

    // add boolean expression to check
    addChild2(aNode, &pNode->three->symbol);
    //add stmt to execute
    addChild2(aNode, &pNode->five->symbol);

    //continue tree building
    createAST(pNode->three, aNode->children[0]);
    createAST(pNode->five, aNode->children[1]);
}

void format_b(struct Node * pNode, AST_Node * aNode)
{
    strcpy(aNode->data, "break");
}

// will move block to the statement position and continue creating the tree
void format_sBlock(struct Node * pNode, AST_Node * aNode)
{
    strcpy(aNode->data, "B");
    createAST(pNode->one, aNode);
}

void removeX(struct Node * pNode, AST_Node * aNode)
{
    // if its an epsilon transition we just remove the node
    if (pNode->one->symbol == 'e')
    {
        aNode->parent->numChildren--;   // alert parent child is being removed
        free(aNode->children);
        free(aNode);
    }
    else    // replace exprList with the tail
    {
        strcpy(aNode->data, &pNode->one->symbol);
        createAST(pNode->one, aNode);       // continue tree
    }
    
    
}

// will move expr to its poition and continue exrListTail` on the same child layer
void removeY(struct Node * pNode, AST_Node * aNode)
{
    strcpy(aNode->data, &pNode->one->symbol);           // change Y to expr
    createAST(pNode->one, aNode);                       // expr continue creating AST


    // if the tail will lead to an epsilon then there is no point in adding it so we check
    if (pNode->two->one->symbol != 'e')
    {
        addChild2(aNode->parent, &pNode->two->symbol);    // add tail as expr sibling
        createAST(pNode->two, aNode->parent->children[aNode->parent->numChildren-1]); // listTail`
    }
    
}

// trying to simplify the expr list tail by removing node and comma then adding expr with is sibling expr
void remove8(struct Node * pNode, AST_Node * aNode)
{
    if (pNode->one->symbol == 'e')
    {
        aNode->parent->numChildren--;
        free(aNode->children);
        free(aNode);
    }
    else    // replace 8 with expr and cintinue building the tree
    {
        strcpy(aNode->data, &pNode->two->symbol);
        createAST(pNode->two, aNode);
    }
    
    
}

void formatI(struct Node * pNode, AST_Node * aNode)
{

}

int whatASTamI(AST_Node * Node)
{

    //printf("Parent %s has numchildren: %d\n", Node->parent->data, Node->parent->numChildren);

    for (int i = 0; i < Node->parent->numChildren; i++)
    {

        //printf("Comparing %s to %s\n", Node->parent->children[i]->data, Node->data);

        if(Node->parent->children[i] == Node)
        {
            return i;
        }
        
    }
    
    // if the child cant be found
    printf("AST Node cannot find itslef, pointer errors?\n");
    return -1;
    
}

/*  main call to create AST, we begin by passing it the default starting symbol in our grammar 'P'
    from there the tree will be traversed and the AST will be created */
void createAST(struct Node * pRoot, AST_Node * aRoot)
{
    //printf("Current Iteration: %c - %s\n", pRoot->symbol, aRoot->data);
    //For some nodes we will not copy instead other connections will be made
    if (removable(pRoot))
    {
        if (pRoot->symbol == 'G')
        {
            removeG(pRoot, aRoot);      //will re-arange tree and 
        }
        else if (pRoot->symbol == 'W')
        {
            removeW(pRoot, aRoot);      
        }
        else if (pRoot->symbol == 'e')
        {
            remove_e(pRoot, aRoot);      
        }
        else if (pRoot->symbol == 'I')
        {
            removeI(pRoot, aRoot);
        }
        else if (pRoot->symbol == 'Z')
        {
            removeZ(pRoot, aRoot);
        }
        else if (pRoot->symbol == '2')
        {
            remove2(pRoot, aRoot);
        }
        else if (pRoot->symbol == 'Q')
        {
            removeQ(pRoot, aRoot);
        }
        else if (pRoot->symbol == '3')
        {
            remove3(pRoot, aRoot);
        }
        else if (pRoot->symbol == 'N')
        {
            removeN(pRoot, aRoot);
        }
        else if (pRoot->symbol == '}' || pRoot->symbol == '{' || pRoot->symbol == ')' || pRoot->symbol == '(')
        {
            removeBrackets(pRoot, aRoot);
        }
        else if (pRoot->symbol == '7')
        {
            remove7(pRoot, aRoot);
        }             
        else if (pRoot->symbol == 'X')
        {
            removeX(pRoot, aRoot);
        }   
        else if (pRoot->symbol == 'Y')
        {
            removeY(pRoot, aRoot);
        }   
        else if (pRoot->symbol == '8')
        {
            remove8(pRoot, aRoot);
        }   
         // will handle some of the stmts that can be simplified
        else if(pRoot->symbol == 'S')
        {
            // once in here we look ahead to see if the statement is one we can simplfy
            if (pRoot->one->symbol == 'f')      // if statement is an if
            {
                format_f(pRoot, aRoot);
            }
            else if (pRoot->one->symbol == 'r')
            {
                format_r(pRoot, aRoot);
            }
            else if (pRoot->one->symbol == 'd')
            {
                format_d(pRoot, aRoot);
            }
            else if (pRoot->one->symbol == 'w')
            {
                format_w(pRoot, aRoot);
            }    
            else if (pRoot->one->symbol == '_')
            {
                format_writeln(pRoot, aRoot);
            }    
            else if (pRoot->one->symbol == 'u')
            {
                format_u(pRoot, aRoot);
            }    
            else if (pRoot->one->symbol == 'b')
            {
                format_b(pRoot, aRoot);
            }   
            else if (pRoot->one->symbol == 'B')
            {
                format_sBlock(pRoot, aRoot);
            }              
            else
            {

                if (pRoot->one)
                {
                    if (pRoot->one->data[0] != '\0')
                    {
                        addChild2(aRoot, pRoot->one->data);
                    }       
                    else
                    {
                        addChild2(aRoot, &pRoot->one->symbol);
                    }

                    // recursive call on child and newAST child that was just made
                    createAST(pRoot->one, aRoot->children[aRoot->numChildren-1]);

                }

                if (pRoot->two)
                {
                    if (pRoot->two->data[0] != '\0')
                    {
                        addChild2(aRoot, pRoot->two->data);
                    }       
                    else
                    {
                        addChild2(aRoot, &pRoot->two->symbol);
                    }
                    createAST(pRoot->two, aRoot->children[aRoot->numChildren-1]);
                }

                if (pRoot->three)
                {
                    if (pRoot->three->data[0] != '\0')
                    {
                        addChild2(aRoot, pRoot->three->data);
                    }       
                    else
                    {
                        addChild2(aRoot, &pRoot->three->symbol);
                    }
                    createAST(pRoot->three, aRoot->children[aRoot->numChildren-1]);
                }

                if (pRoot->four)
                {
                    if (pRoot->four->data[0] != '\0')
                    {
                        addChild2(aRoot, pRoot->four->data);
                    }       
                    else
                    {
                        addChild2(aRoot, &pRoot->four->symbol);
                    }
                    createAST(pRoot->four, aRoot->children[aRoot->numChildren-1]);
                }

                if (pRoot->five)
                {
                    if (pRoot->five->data[0] != '\0')
                    {
                        addChild2(aRoot, pRoot->five->data);
                    }       
                    else
                    {
                        addChild2(aRoot, &pRoot->five->symbol);
                    }
                    createAST(pRoot->five, aRoot->children[aRoot->numChildren-1]);
                }

                if (pRoot->six)
                {
                    if (pRoot->six->data[0] != '\0')
                    {
                        addChild2(aRoot, pRoot->six->data);
                    }       
                    else
                    {
                        addChild2(aRoot, &pRoot->six->symbol);
                    }
                    createAST(pRoot->six, aRoot->children[aRoot->numChildren-1]);
                }
                
                if (pRoot->seven)
                {
                    if (pRoot->seven->data[0] != '\0')
                    {
                        addChild2(aRoot, pRoot->seven->data);
                    }       
                    else
                    {
                        addChild2(aRoot, &pRoot->seven->symbol);
                    }
                    createAST(pRoot->seven, aRoot->children[aRoot->numChildren-1]);
                }
            }
            
        }

    }    
        

    else {
        //Begin by looking at children and creating copies and making pointer connections
        if (pRoot->one)
        {
            if (pRoot->one->data[0] != '\0')
            {
                addChild2(aRoot, pRoot->one->data);
            }       
            else
            {
                addChild2(aRoot, &pRoot->one->symbol);
            }

            // recursive call on child and newAST child that was just made
            createAST(pRoot->one, aRoot->children[aRoot->numChildren-1]);

        }
        if (pRoot->two)
        {
            if (pRoot->two->data[0] != '\0')
            {
                addChild2(aRoot, pRoot->two->data);
            }       
            else
            {
                addChild2(aRoot, &pRoot->two->symbol);
            }
            createAST(pRoot->two, aRoot->children[aRoot->numChildren-1]);
        }
        if (pRoot->three)
        {
            if (pRoot->three->data[0] != '\0')
            {
                addChild2(aRoot, pRoot->three->data);
            }       
            else
            {
                addChild2(aRoot, &pRoot->three->symbol);
            }
            createAST(pRoot->three, aRoot->children[aRoot->numChildren-1]);
        }
        if (pRoot->four)
        {
            if (pRoot->four->data[0] != '\0')
            {
                addChild2(aRoot, pRoot->four->data);
            }       
            else
            {
                addChild2(aRoot, &pRoot->four->symbol);
            }
            createAST(pRoot->four, aRoot->children[aRoot->numChildren-1]);
        }
        if (pRoot->five)
        {
            if (pRoot->five->data[0] != '\0')
            {
                addChild2(aRoot, pRoot->five->data);
            }       
            else
            {
                addChild2(aRoot, &pRoot->five->symbol);
            }
            createAST(pRoot->five, aRoot->children[aRoot->numChildren-1]);
        }
        if (pRoot->six)
        {
            if (pRoot->six->data[0] != '\0')
            {
                addChild2(aRoot, pRoot->six->data);
            }       
            else
            {
                addChild2(aRoot, &pRoot->six->symbol);
            }
            createAST(pRoot->six, aRoot->children[aRoot->numChildren-1]);
        }
        if (pRoot->seven)
        {
            if (pRoot->seven->data[0] != '\0')
            {
                addChild2(aRoot, pRoot->seven->data);
            }       
            else
            {
                addChild2(aRoot, &pRoot->seven->symbol);
            }
            createAST(pRoot->seven, aRoot->children[aRoot->numChildren-1]);
        }
    }
}


//////////////////////////////////////////////////////////////////////
////////////////////// Cleaning AST Methods///////////////////////////
//////////////////////////////////////////////////////////////////////


// forward declarations, becasue we have function pairs that call eachother
void cleanLists(AST_Node * node);                               // for list cleaning

void cleanOperators(AST_Node * node, Table * table);                            // for operators

// used becasue after creating the tree i realized some of the parent pointer are not working properly
void fixParentPointers(AST_Node * node)
{

    // all that is done is the nodes children are iterated through and the pointers are updated
    for (int i = 0; i < node->numChildren; i++)
    {
        node->children[i]->parent = node;       // ensure parent pointer is the actual pointer
    }
    
    // recursive call to children 
    for (int j = 0; j < node->numChildren; j++)
    {
        fixParentPointers(node->children[j]);
    }
    


}

// will look at the nodes data to see if it is an operator and return 1 or 0
int isOperator(AST_Node * node)
{
    // we will be checking for these nodes within the tree
    char operators[12][3] =  { "<=", ">=", "||", "&&",
                              "-", "+", "/", "<", ">",
                              "*", "!=", "==" 
                            };

    for (int i = 0; i < 12; i++)
    {
        if (!strcmp(node->data, operators[i]))
        {
            return 1;
        }  
    }

    // if there were no mathces we return false
    return 0;
}

void formatList(AST_Node * aNode)//, AST_Node * realRoot)
{

    if (aNode->numChildren >= 2/*aNode->children[1]*/)  // check if L has the second child
    {
        if(aNode->children[1]->numChildren)
        {

            // look if list has another list to pull up
            if (!strcmp(aNode->children[1]->children[0]->data, "L"))
            {
                
                //printf("Entering clean lists L --> %s\n", aNode->children[0]->children[0]->data);
                //printf("L num child: %d\n", aNode->numChildren);

                int Lpos = whatASTamI(aNode);
                aNode = aNode->parent;  //static pointer we will use for function 
                
                //move L to same layer as other L
                addChild2(aNode, "L");
                
                //move L's children
                aNode->children[aNode->numChildren-1]->children = aNode->children[Lpos]->children[1]->children[0]->children;
                aNode->children[aNode->numChildren-1]->numChildren = aNode->children[Lpos]->children[1]->children[0]->numChildren;

                // remove 5 and L
                free(aNode->children[Lpos]->children[1]->children[0]); //L
                free(aNode->children[Lpos]->children[1]->children);  //5 child container (all it held was L)
                free(aNode->children[Lpos]->children[1]);           //5

                //let parent know 5 was freed
                aNode->children[Lpos]->numChildren--;
            

                //printf("Before recursive call to clean: %s\n", aNode->children[aNode->numChildren-1]->data);

                //printf("#########After moving L#########\n");
                //printAST(realRoot, 0);

                // continue cleaning new node we just moved 
                //cleanUpAST(aNode->children[aNode->numChildren-1], realRoot);   
                cleanLists(aNode->children[aNode->numChildren-1]);
            }
        }
    }


    // continue recursively cleaning the tree (handles if we dont meet conditions above)
    for (int i = 0; i < aNode->children[aNode->numChildren-1]->numChildren; i++)
    {
        //printf("Calling clean on child %s\n", aNode->children[aNode->numChildren-1]->children[i]->data);
        //cleanUpAST(aNode->children[aNode->numChildren-1]->children[i], realRoot);
        cleanLists(aNode->children[aNode->numChildren-1]->children[i]);
    }
}

void cleanLists(AST_Node * node)
{
    if(!strcmp(node->data, "L"))
    {
        //printf("L num children: %d\n", aRoot->numChildren);
        //printf("%s-->%s-->%s\n", aRoot->data, aRoot->children[0]->data, aRoot->children[0]->children[0]->data);
        formatList(node);
    }
    else
    {
        // recursive call to all nodes to clean tree
        for (int i = 0; i < node->numChildren; i++)
        {
            //printf("(Main Function Call)Parent: %s Child: %s\n", aRoot->data, aRoot->children[i]->data);
            cleanLists(node->children[i]);
        }
    }
}

// will make op node connect expression or identifiers together
void formatOperators(AST_Node * node, Table * table)//, AST_Node * realRoot)
{

    // move to operators parent so we can move sub nodes
    //node = node->parent;

    char oldParent[20];
    memset(oldParent, '\0', sizeof(oldParent));
    strcpy(oldParent, node->data);

    // swap the posisiton of the operator and its parent expression

    strcpy(node->data, node->children[0]->data);    // move operator to position of parent
    strcpy(node->children[0]->data, oldParent);    // move parent to operator position

    //printf("%s addChild-> %s\n", node->data, node->children[0]->children[0]->data);

    // move operators child expression along along with it
    addChild2(node, node->children[0]->children[0]->data);

    //copy rest of nodes data to its new node (replacement)
    node->children[node->numChildren-1]->numChildren = node->children[0]->children[0]->numChildren;
    node->children[node->numChildren-1]->children = node->children[0]->children[0]->children;
    node->children[node->numChildren-1]->parent = node->children[0]->children[0]->parent;

    // free the node that we just copied over
    free(node->children[0]->children[0]);

    //printf("Continuing cleaning: %s\n", node->children[node->numChildren-1]->data);

    //printf("num prior to decrement: %d\n", node->children[0]->numChildren);
    // let parent of freed node know we removed it
    node->children[0]->numChildren--;


    // continue process with the node we have moved over
    cleanOperators(node->children[node->numChildren-1], table);  

    /*
    // temp var to hold data we will be over writing
    char oldParent[20];
    memset(oldParent, '\0', sizeof(oldParent));
    strcpy(oldParent, node->parent->data);

    //printf("copying:%s to %s\n", node->data, node->parent->data);

    // move operator to position of parent
    strcpy(node->parent->data, node->data);

    printf("Node after copying childs data: %s\n", node->parent->data);

    // let parent of node we moved know it no longer has that child
    node->children[0]->numChildren--;

    // move parent to op position
    strcpy(node->data, oldParent);

    //printAST(realRoot, 0);

    // move pointer back to the operator
    node = node->parent;

    // create node with copied data of operators child
    addChild2(node, node->children[0]->children[0]->data);   

    printf("after adding newChild %s\n", node->children[node->numChildren-1]->data);

    // move operators child children and update num Children

    if (node->children[0]->children[0]->numChildren > 0)
    {
        printf("Moving Children\n");
        node->children[node->numChildren-1]->children = node->children[0]->children[0]->children;
        node->children[node->numChildren-1]->numChildren = node->children[0]->children[0]->numChildren;
    }
    
    

    printf("before updating parents numChildren: %s\n", node->children[node->numChildren-1]->data);

    

    printf("before free %s\n", node->children[node->numChildren-1]->data);

    // free old operator node
    free(node->children[0]->children[0]);
    
    printf("Attemting to clean: %s\n", node->children[node->numChildren-1]->data);

    

    // continue cleaning chain of expressions
    //cleanUpAST(node->children[node->numChildren-1], realRoot);
    cleanOperators(node->children[node->numChildren-1]);
    */

}

// will use symbol table to get how many arguments the function call uses
int getNumParams(Table * table, char * funcID)
{
    //printf("test: %s\n", funcID);
    
    // loop through global table to find function declaration
    for (int i = 0; i < table->size; i++)
    {
        if (!strcmp(getCharVec(&table->identifier, i), funcID))
        {
            return strlen(getCharVec(&table->funcArgs, i));
        }
        
    }

    // return -1 so we know there was some error, maybe the function call is not a real function
    return -1;
}

// we needed to create a different formating method for this expr becaue an operator is allowed to be placed after a Primary
// and that case was not handled previoudsly. Instead it would skip this operator 
void formatExprAfterI(AST_Node * node, Table * table)
{

    // if operator is on same line as function call, make sure that it is not an operator to calculate a function argument
    if (node->parent->children[0]->data[0] == '(')
    {
        //printf("Comparing for %s-->%s %d : %d\n", node->data, node->children[0]->data, whatASTamI(node)+1, getNumParams(table, node->parent->data));
        if (whatASTamI(node) <= getNumParams(table, node->parent->data))
        {
            return;
        }        
    }

    // at the beginnig of the function call we are passed the operators pointer
    
    //first we copy the parent (should be an id) and all children 
    addChild2(node, node->parent->data);

    // move children of parent to new position of parent
    node->children[node->numChildren-1]->children = node->parent->children;
    node->children[node->numChildren-1]->numChildren = node->parent->numChildren - 1; 

    
    // connect operator as parents parents child
    node->parent->parent->children[whatASTamI(node->parent)] = node; 
    AST_Node * temp = node->parent;             // will be used to delete later
    node->parent = node->parent->parent;        // update operator parent also


    // no longer need parent, free node
    free(temp);
    
    //finally we need to swap the order of the nodes to keep the order that was given by the user
    temp = node->children[0];
    node->children[0] = node->children[1];
    node->children[1] = temp;

    // now that it is fomratted correctly the operator can continue through the normal operator cleaning process
    cleanOperators(node, table);

}


// will move misplaced operator to child of expression in parenthesis, this will ensure proper execution order
void formatExprAfterParen(AST_Node * node, Table * table)
{

    // if operator is on same line as function call, make sure that it is not an operator to calculate a function argument
    if (node->parent->children[0]->data[0] == '(')
    {
        //printf("Comparing %d : %d\n", whatASTamI(node)+1, getNumParams(table, node->parent->data));

        if (whatASTamI(node) <= getNumParams(table, node->parent->data))
        {
            return;
        }
        
    }
    

    //printf("test: %s\n",  node->parent->parent->data);
    //printAST(node->parent, 0);

    // first we find the node that is the end of the expression
    AST_Node * lastExprNode = node;
    while (lastExprNode->numChildren > 0)
    {
        lastExprNode = lastExprNode->children[0];
    }
    
    // move incorrect operator by copying data to 
    lastExprNode->children = calloc(1, sizeof(AST_Node*));
    lastExprNode->children[0] = node->parent->children[whatASTamI(node) + 1];
    lastExprNode->numChildren++;

    // let parent know that child was moved
    node->parent->numChildren--;    
}

void cleanOperators(AST_Node * node, Table * table)
{
    // in this clean function we look one ahead because when we recursively jump out of call back to the
    // parent node we call clean again on an operator that has already been cleaned. This happens becasue we move a
    // node to the parent and in the format function we already call to clean the node. 

    //printf("cleaning op: %s \tnumCh: %d -> ", node->data, node->numChildren );

    if(node->numChildren >= 1)
    {
        // a quick fix to a bug i found inside the operator cleaning
        if (node == node->children[0])
        {
            node->numChildren--;
        }
        

        if (node->parent != NULL && isOperator(node))       // will look for expr after function call or array indexes
        {
    
            // special check for if operateor is after Primary with index or function call
            if ((node->parent->children[0]->data[0] == '(' || node->parent->children[0]->data[0] == '[') /*&& whatASTamI(node) == node->parent->numChildren-1*/)
            {

                // fist check if its even possible for an operator to be next to this operator
                if (node->parent->numChildren > whatASTamI(node)+1)
                {
                    // check if there are mutiple expressions after primary so we can stack them for further processing
                    if (isOperator(node->parent->children[whatASTamI(node)+1]))
                    {
                        //printf("going into special case\n");
                        formatExprAfterParen(node, table);
                    }
                    
                }
                

                // check if expression` with operator after a primary needs to be moved
                if (whatASTamI(node) == node->parent->numChildren-1)
                {
                    //printf("formatting expression for: %s --> %s\n", node->parent->data, node->data); 
                    formatExprAfterI(node, table);
                }
                
            }
        }
        if(isOperator(node->children[0]))
        {
            // special case when expression` follows a set of parenthesis for an expression
            if (node->numChildren == 2 )
            {
                if (isOperator(node->children[1]))
                {
                    formatExprAfterParen(node->children[0], table);
                }                
            }
            
            formatOperators(node, table);
        }
        else
        {
    
        // recursive call to all nodes to clean tree
            for (int i = 0; i < node->numChildren; i++)
            {
                cleanOperators(node->children[i], table);
            }
        }
    }
}

// Will remove the V's and make them chilldren of the appropriate node
void formatVarDecl(AST_Node * node, AST_Node * varRoot)
{

    //printf("%s\t%s\t%s\n", node->data, node->children[0]->data, node->parent->data);

    // if the variabel Decl doesnt have enough variables to need formatting.
    if (node == varRoot && node->numChildren < 2)
    {
        return;
    }
    
    // dont do anything to root var decl continue with formatting 
    else if (node == varRoot && node->numChildren >= 2)
    {
        formatVarDecl(node->children[1], varRoot);
    }   
    else
    {
        if (node->parent == varRoot)
        {
            // replace V with variable id
            strcpy(node->data, node->children[0]->data);  
            
            if (node->numChildren >= 2)
            {
                formatVarDecl(node->children[1], varRoot);
            }
            

        }
        else
        {
            // add this node as a child to the root var decl
            addChild2(varRoot, node->children[0]->data);

            if (node->numChildren >= 2)
            {
                formatVarDecl(node->children[1], varRoot);
            }
        }

    }
    
    

}

// recursively clean the tree. 
void cleanVarDecl(AST_Node * node)
{
    if(!strcmp(node->data, "V"))
    {

        formatVarDecl(node, node);
        
        if (node->numChildren >= 2)
        {
            // free initial identifier
            free(node->children[1]->children[0]->children);
            free(node->children[1]->children[0]);

            if (node->children[1]->numChildren >= 2)
            {
                deleteAST(node->children[1]->children[1]);
            }
            
            // let second id know it has no children becasue we deleted
            node->children[1]->numChildren = 0;
        }
        
    }
    else
    {
        // recursive call to all nodes to clean tree
        for (int i = 0; i < node->numChildren; i++)
        {
            cleanVarDecl(node->children[i]);
        }
        
    }
}

/*  will make our tree look nicer with some final touches that are easier
    to be done after the parse tree has been created*/

void cleanUpAST(AST_Node * aRoot, AST_Node * realRoot, Table * table)
{

    // will iterate through the entire tree once to fix our stmts
    fixParentPointers(aRoot);     // recursive parent pointer fix
    cleanLists(aRoot);
    fixParentPointers(aRoot);
    //printAST(aRoot, 0);
    cleanOperators(aRoot, table);
    //printAST(realRoot, 0);
    fixParentPointers(aRoot);       //not sure why but some parent pointers were not correct
    cleanVarDecl(aRoot);
    
}

/////////////////////////////////////////////////////////
/////////////////Type Checking Methods///////////////////
/////////////////////////////////////////////////////////

char * typeToString(char type)
{
    switch (type)
    {
    case '#':
        return "integer";
    case 'c':
        return "character";
    case '[':
        return "integer array";
    case ']':
        return "character array";
    default:
        return NULL;    // we should expect no other types to be retuned
    }
}

int typeCheckParamExpr(Table * table, AST_Node * node, char type, int argPos)
{
    
    // if the expression goes further than one expression
    if (isOperator(node->children[1]))
    {
        // check first child is correct type then continue to check the following operator

        // if not a constant
        if (!isdigit(node->children[0]->data[0]))
        { 
            if (node->children[0]->numChildren == 0)
            {    
                if (getType(table, node->children[0]->data)[0] != type)     // compare type returned to type given from table
                {
                    printf("ERROR: Function %s() expects argument in position %d to be of type %s\n", node->parent->data, argPos+1, typeToString(type));
                    return 0;
                }
            }
            else
            {
                if (strcmp(node->children[0]->children[0]->data, "["))        // if its an array index
                {
                     if (!strcmp(getType(table, node->children[0]->data), "[") && type != '#')            // Remember to check for seg fault
                    {
                        printf("ERROR: Function %s() expects argument in position %d to be of type %s\n", node->parent->data, argPos+1, typeToString(type));
                        return 0;
                    }

                    else if (!strcmp(getType(table, node->children[0]->data), "]") && type != 'c')
                    {
                        printf("ERROR: Function %s() expects argument in position %d to be of type %s\n", node->parent->data, argPos+1, typeToString(type));
                        return 0;
                    }
                }


                // if its a fucntion we just check the return type is correct
                else if (!strcmp(node->children[0]->children[0]->data, "("))
                {
                    if (getType(table, node->children[0]->data)[0] != type)     // if it is not
                    {
                        printf("ERROR: Function %s() expects argument in position %d to be of type %s\n", node->parent->data, argPos+1, typeToString(type));
                        return 0;
                    }
                    
                }   
            }
        }

        else // if we were passed a constant and int is no the expected type
        {
            if (type != '#')
            {
                printf("ERROR: Function %s() expects argument in position %d to be of type %s\n", node->parent->data, argPos+1, typeToString(type));
                return 0;
            }
            
        }
        
        // continue with recursive check of the next operator
        return typeCheckParamExpr(table, node->children[1], type, argPos);
    }


    else // expr is only one bin op long
    {
        // check if paramter input is a constant
        if (isdigit(node->children[0]->data[0]) && type != '#')
        {
            printf("ERROR: Function %s() expects argument in position %d to be of type %s\n", node->parent->data, argPos+1, typeToString(type));
            return 0;
        }
        if (isdigit(node->children[1]->data[0]) && type != '#')
        {
            printf("ERROR: Function %s() expects argument in position %d to be of type %s\n", node->parent->data, argPos+1, typeToString(type));
            return 0;
        }

                                                                                    // if identifiers/functions are used for left arg
        if (!isdigit(node->children[0]->data[0]))
        {            
            if (node->children[0]->numChildren == 0)
            {    
                if (getType(table, node->children[0]->data)[0] != type)
                {
                    printf("ERROR: Function %s() expects argument in position %d to be of type %s\n", node->parent->data, argPos+1, typeToString(type));
                    return 0;
                }
            }
            else
            {
                if (strcmp(node->children[0]->children[0]->data, "["))        // if its an array index
                {
                     if (!strcmp(getType(table, node->children[0]->data), "[") && type != '#')            // Remember to check for seg fault
                    {
                        printf("ERROR: Function %s() expects argument in position %d to be of type %s\n", node->parent->data, argPos+1, typeToString(type));
                        return 0;
                    }

                    else if (!strcmp(getType(table, node->children[0]->data), "]") && type != 'c')
                    {
                        printf("ERROR: Function %s() expects argument in position %d to be of type %s\n", node->parent->data, argPos+1, typeToString(type));
                        return 0;
                    }
                }


                // if its a fucntion we just check the return type is correct
                else if (!strcmp(node->children[0]->children[0]->data, "("))
                {
                    if (getType(table, node->children[0]->data)[0] != type)     // if it is not
                    {
                        printf("ERROR: Function %s() expects argument in position %d to be of type %s\n", node->parent->data, argPos+1, typeToString(type));
                        return 0;
                    } 
                }   
            }
        }
        
                                                                                        // if id is used for right arg
        if (!isdigit(node->children[1]->data[0]))
        {            
            if (node->children[1]->numChildren == 0)
            {
                if (getType(table, node->children[1]->data)[0] != type)
                {
                    printf("ERROR: Function %s() expects argument in position %d to be of type %s\n", node->parent->data, argPos+1, typeToString(type));
                    return 0;
                }
            }
            else
            {
                 // if its an array index
                if (!strcmp(node->children[1]->children[0]->data, "["))        
                {
                     if (!strcmp(getType(table, node->children[1]->data), "[") && type != '#')            // Remember to check for seg fault
                    {
                        printf("ERROR: Function %s() expects argument in position %d to be of type %s\n", node->parent->data, argPos+1, typeToString(type));
                        return 0;
                    }

                    else if (!strcmp(getType(table, node->children[1]->data), "]") && type != 'c')
                    {
                        printf("ERROR: Function %s() expects argument in position %d to be of type %s\n", node->parent->data, argPos+1, typeToString(type));
                        return 0;
                    }
                }

                // if its a fucntion we just check the return type is correct
                else if (!strcmp(node->children[1]->children[0]->data, "("))
                {
                    if (getType(table, node->children[1]->data)[0] != type)     // if it is not
                    {
                        printf("ERROR: Function %s() expects argument in position %d to be of type %s\n", node->parent->data, argPos+1, typeToString(type));
                        return 0;
                    }
                    
                }
                
            }
        }


        // if it makes it through all the checks then the types are good
        return 1;
    }
    
    
}

int checkParameterType(Table * table, AST_Node * node, char type, int argPos)
{

    printf("\nType Checking: %s and %c\n", node->data, type);
    if (node->numChildren == 0)
    {
        
        // best case its an id of the same type (also check for [indexing] )
        if (getType(table, node->data)[0] ==  type)
        {
            return 1;
        }

        // check if its a constant
        else if (isdigit(node->data[0]) && type == '#')    // check first char in data to see if its a number
        {
            return 1;
        }

        else    // if we make it here then the type given is wrong
        {
            //printf("test\n");
            printf("ERROR: Function %s() expects argument in position %d to be of type %s\n", node->parent->data, argPos+1, typeToString(type));
            return 0;
        }
        
        
    }

    // if the arg has children that means its an array or an expression we must type check
    else
    {
        // handle if the parameter is an array
        if (!strcmp(node->children[0]->data, "["))
        {

            // indexing into array returns int so we check 
            if (!strcmp(getType(table, node->data), "[") && type == '#')            // Remember to check for seg fault
            {
                return 1;
            }

            else if (!strcmp(getType(table, node->data), "]") && type == 'c')
            {
                return 1;
            }
            else
            {
                printf("ERROR: Function %s() expects argument in position %d to be of type %s\n", node->parent->data, argPos+1, typeToString(type));
                return 0;
            }
            
            
            
        }

        // if the parameter is an expression call another function to handle it
        else if (isOperator(node))
        {
            if (!typeCheckParamExpr(table, node, type, argPos))
            {
                return 0;
            }
        }
    }
    


    return 1;

}

// will be passed the identifier we expect to be a function call
int functionCallCheck(AST_Node * node, Table * symbolTable)
{

    // variable to help do checking
    char params[maxStringLen];
    memset(params, '\0', sizeof(params));

    // first we look up this function in the global table 
    for (int i = 0; i < symbolTable->size; i++)
    {
        // if the id is the one we are getting parameters for
        if (!strcmp(getCharVec(&symbolTable->identifier, i), node->data))
        {
            // then we check whether is a variable or function
            if (!strcmp(getCharVec(&symbolTable->action, i), "Function Declaration"))
            {
                strcpy(params, getCharVec(&symbolTable->funcArgs, i));
            }
            else
            {
                printf("ERROR: Expecting function, instead given -> %s\n", node->data);
                return 0; 
            }     
        }
    }

    // Checking the number of parameters given for the function call
    // if functino has no parameters
    if (!strcmp(params,"N/A"))
    {
        if (node->numChildren != 1) // we expect one child just the (
        {
            printf("ERROR: function call --> %s() does not expect any arguments\n", node->data);
            return 0;
        }
        else if (node->numChildren == 1)
        {
            return 1;           // no arguments supplied so return check as sucessful
        }
        
        
    } 
    //then we check how many parameters were given from code
    else if ((node->numChildren -1) != strlen(params))
    {
        printf("ERROR: fucntion call --> %s() expected %lu arguments, given %d instead\n", node->data, strlen(params), node->numChildren-1);
        return 0;
    }
    
    // if there are enough parameters given we check their types
    for (int i = 0; i < strlen(params); i++)
    {
        // function will check and return a 1 if the parameter is the same type as its declared
        int isSameTypeBool = checkParameterType(symbolTable, node->children[i+1], params[i], i);

        if (isSameTypeBool == 0)
        {
            return 0;
        }

        printf("Successful Check!\n");
        
    }
    

    return 1; // succesful check
    
}

char tableTypeSearch(Table * table, char * id, char * scope)
{
    // go to table of scope given if not asked for global
    if(strcmp(scope, "global"))
    {    
        for (int i = 0; i < table->numChildren; i++)
        {
            if (table->children[i]->size > 0)
            {
                if (!strcmp(getCharVec(&table->children[i]->scope, 0), scope))
                {
                    table = table->children[i];     // go to that sub table
                }
            }
        }
    }
    

    // once we go to global parent table will be null
    while (table != NULL)
    {
        // search the current table rows for the variable
        for (int j = 0; j < table->size; j++)
        {
            if (!strcmp(getCharVec(&table->identifier, j), id))
            {
                return getCharVec(&table->type, j)[0];  // return just the char
            }
            
        }

        // if we went through the table without finding it 
        table = table->parent;
    }
    
    // if we make it out here we could not find the id
    return '\0';

    
}

int typeCheckReturnExpr(Table * table, AST_Node * node, char type, char * scope)
{

    printf("Type checking %s & %s scope: %s\n", node->children[0]->data, node->children[1]->data, scope);
    
    // if the expression goes further than one expression
    if (isOperator(node->children[1]))
    {
        // check first child is correct type then continue to check the following operator

        // if not a constant
        if (!isdigit(node->children[0]->data[0]))
        { 
        
            if (node->children[0]->numChildren == 0)
            {   

                if (tableTypeSearch(table, node->children[0]->data, scope) != type)     // compare type returned to type given from table
                {
                    printf("ERROR: return");
                    return 0;
                }
            }
            else
            {
                if (strcmp(node->children[0]->children[0]->data, "["))        // if its an array index
                {
                     if (!strcmp(getType(table, node->children[0]->data), "[") && type != '#')            // Remember to check for seg fault
                    {
                        //printf("ERROR: Function %s() expects argument in position %d to be of type %s\n", node->parent->data, argPos+1, typeToString(type));
                        return 0;
                    }

                    else if (!strcmp(getType(table, node->children[0]->data), "]") && type != 'c')
                    {
                        //printf("ERROR: Function %s() expects argument in position %d to be of type %s\n", node->parent->data, argPos+1, typeToString(type));
                        return 0;
                    }
                }


                // if its a fucntion we just check the return type is correct
                else if (!strcmp(node->children[0]->children[0]->data, "("))
                {
                    if (getType(table, node->children[0]->data)[0] != type)     // if it is not
                    {
                        //printf("ERROR: Function %s() expects argument in position %d to be of type %s\n", node->parent->data, argPos+1, typeToString(type));
                        return 0;
                    }
                }   
            }
        }

        else // if we were passed a constant and int is no the expected type
        {
            if (type != '#')
            {
                //printf("ERROR: Function %s() expects argument in position %d to be of type %s\n", node->parent->data, argPos+1, typeToString(type));
                return 0;
            }
            
        }
        
        
        // continue with recursive check of the next operator
        return typeCheckReturnExpr(table, node->children[1], type, scope);
    }


    else // expr is only one bin op long
    {
        // check if paramter input is a constant
        if (isdigit(node->children[0]->data[0]) && type != '#')
        {
            //printf("ERROR: Function %s() expects argument in position %d to be of type %s\n", node->parent->data, argPos+1, typeToString(type));
            return 0;
        }
        if (isdigit(node->children[1]->data[0]) && type != '#')
        {
            //printf("ERROR: Function %s() expects argument in position %d to be of type %s\n", node->parent->data, argPos+1, typeToString(type));
            return 0;
        }

                                                                                    // if identifiers/functions are used for left arg
        if (!isdigit(node->children[0]->data[0]))
        {            
            if (node->children[0]->numChildren == 0)
            {    
                if (getType(table, node->children[0]->data)[0] != type)
                {
                    //printf("ERROR: Function %s() expects argument in position %d to be of type %s\n", node->parent->data, argPos+1, typeToString(type));
                    return 0;
                }
            }
            else
            {
                if (strcmp(node->children[0]->children[0]->data, "["))        // if its an array index
                {
                     if (!strcmp(getType(table, node->children[0]->data), "[") && type != '#')            // Remember to check for seg fault
                    {
                        //printf("ERROR: Function %s() expects argument in position %d to be of type %s\n", node->parent->data, argPos+1, typeToString(type));
                        return 0;
                    }

                    else if (!strcmp(getType(table, node->children[0]->data), "]") && type != 'c')
                    {
                        //printf("ERROR: Function %s() expects argument in position %d to be of type %s\n", node->parent->data, argPos+1, typeToString(type));
                        return 0;
                    }
                }


                // if its a fucntion we just check the return type is correct
                else if (!strcmp(node->children[0]->children[0]->data, "("))
                {
                    if (getType(table, node->children[0]->data)[0] != type)     // if it is not
                    {
                        //printf("ERROR: Function %s() expects argument in position %d to be of type %s\n", node->parent->data, argPos+1, typeToString(type));
                        return 0;
                    } 
                }   
            }
        }
        
                                                                                        // if id is used for right arg
        if (!isdigit(node->children[1]->data[0]))
        {            
            if (node->children[1]->numChildren == 0)
            {
                if (getType(table, node->children[1]->data)[0] != type)
                {
                    //printf("ERROR: Function %s() expects argument in position %d to be of type %s\n", node->parent->data, argPos+1, typeToString(type));
                    return 0;
                }
            }
            else
            {
                 // if its an array index
                if (!strcmp(node->children[1]->children[0]->data, "["))        
                {
                     if (!strcmp(getType(table, node->children[1]->data), "[") && type != '#')            // Remember to check for seg fault
                    {
                        //printf("ERROR: Function %s() expects argument in position %d to be of type %s\n", node->parent->data, argPos+1, typeToString(type));
                        return 0;
                    }

                    else if (!strcmp(getType(table, node->children[1]->data), "]") && type != 'c')
                    {
                        //printf("ERROR: Function %s() expects argument in position %d to be of type %s\n", node->parent->data, argPos+1, typeToString(type));
                        return 0;
                    }
                }

                // if its a fucntion we just check the return type is correct
                else if (!strcmp(node->children[1]->children[0]->data, "("))
                {
                    if (getType(table, node->children[1]->data)[0] != type)     // if it is not
                    {
                        //printf("ERROR: Function %s() expects argument in position %d to be of type %s\n", node->parent->data, argPos+1, typeToString(type));
                        return 0;
                    }
                    
                }
                
            }
        }


        // if it makes it through all the checks then the types are good
        return 1;
    }
    
}

AST_Node * getFuncReturnType(Table * table, AST_Node * node)
{
    // get to function node to find id
    while (node->data[0] != 'F')
    {
        node = node->parent;

        if (node->parent == NULL)
        {
            return NULL;
        }
        
    }

    // if we find the node we make it out here
    return node->children[0];
    
}

int returnCheck(AST_Node * node, Table * symbolTable)
{

    printf("\ntype checking return %s\n", node->children[0]->data);

    // gettic the functions return type if possible
    AST_Node * funcID = getFuncReturnType(symbolTable, node); 


    // if the return was not within a function 
    if (funcID == NULL)
    {
        printf("Unexpected return %s\n", node->children[0]->data);
        return 0;
    }

    // if returning a constant
    if (isdigit(node->children[0]->data[0]))
    {
        if (getType(symbolTable, funcID->data)[0] != '#')
        {
            printf("ERROR: Function %s() expects return expression of type %s\n", funcID->data, getType(symbolTable, funcID->data));
            return 0;
        }
    }
    else
    {
        // there is an expression to process
        if (isOperator(node->children[0]))
        {
            if(!typeCheckReturnExpr(symbolTable, node->children[0], getType(symbolTable, funcID->data)[0], funcID->data))
            {

                printf("ERROR: Function %s() expects return expression of type %s\n", funcID->data, getType(symbolTable, funcID->data));
                return 0;
            }
        }

        // checking if identifier
        else
        {
            if (node->children[0]->numChildren == 0)
            {
                printf("test%c\n", getType(symbolTable, node->children[0]->data)[0]);
                if (getType(symbolTable, funcID->data)[0] != getType(symbolTable, node->children[0]->data)[0])
                {
                    printf("ERROR: Function %s() expects return expression of type %s\n", funcID->data, getType(symbolTable, funcID->data));
                    return 0;
                }
            }

            // returning either array or function call
            else
            {
                //
                if (node->children[0]->children[0]->data[0] == '[')
                {
                            // indexing into array returns int so we check 
                    if (!strcmp(getType(symbolTable, node->children[0]->data), "[") && getType(symbolTable, funcID->data)[0] != '#')            // Remember to check for seg fault
                    {
                        return 0;
                    }

                    else if (!strcmp(getType(symbolTable, node->children[0]->data), "]") && getType(symbolTable, funcID->data)[0] != 'c')
                    {
                        return 0;
                    }
                }

                // we only need to check what a function call will return 
                else
                {
                    if (strcmp(getType(symbolTable, funcID->data), getType(symbolTable, node->children[0]->data)))
                    {
                        printf("ERROR: Function %s() expects return expression of type %s\n", funcID->data, getType(symbolTable, funcID->data));
                        return 0;
                    }
                }
                    
            }
                
        }
                
    }

    // if it doesnt fail a check its a valid return
    printf("return check succesful\n");
    return 1;
}

int typeCheckIndexExpr(Table * table, AST_Node * node, char * scope)
{
    // if the expression goes further than one expression
    if (isOperator(node->children[1]))
    {
        // check first child is correct type then continue to check the following operator

        // if the one arg we can check is not a constant
        if (!isdigit(node->children[0]->data[0]))
        { 
            // if its an id with no children
            if (node->children[0]->numChildren == 0)
            {    
                if (tableTypeSearch(table, node->children[0]->data, scope) != '#')     // compare type returned to type given from table
                {
                    printf("ERROR: %s index must be of type integer\n", node->parent->data);
                    return 0;
                }
            }
            // if id has children 
            else
            {
                if (strcmp(node->children[0]->children[0]->data, "["))        // if its an array index
                {
                    // check if array being indexed returns int if not print this errror
                     if (tableTypeSearch(table, node->children[0]->data, scope) != '[')            // Remember to check for seg fault
                    {
                        printf("ERROR: %s index must be of type integer\n", node->parent->data);
                        return 0;
                    }
                }


                // if its a function we just check the return type is correct
                else if (!strcmp(node->children[0]->children[0]->data, "("))
                {
                    if (tableTypeSearch(table, node->children[0]->data, scope) != '#')     // if it is not an int
                    {
                        printf("ERROR: %s index must be of type integer\n", node->parent->data);
                        return 0;
                    }
                    
                }   
            }
        }
        
        // continue with recursive check of the next operator
        return typeCheckIndexExpr(table, node->children[1], scope);
    }


    else // expr is only one bin op long
    {

        // if identifiers/functions are used for left arg
        if (!isdigit(node->children[0]->data[0]))
        {            
            // id with no index or func call
            if (node->children[0]->numChildren == 0)
            {    
                if (tableTypeSearch(table, node->children[0]->data, scope) != '#')
                {
                    printf("ERROR: %s index must be of type integer\n", node->parent->data);
                    return 0;
                }
            }
            else
            {
                if (strcmp(node->children[0]->children[0]->data, "["))        // if its an array index
                {
                    // check array indexing is an int array, if not return error
                    if (tableTypeSearch(table, node->children[0]->data, scope) != '[')          
                    {
                        printf("ERROR: %s index must be of type integer\n", node->parent->data);
                        return 0;
                    }
                }

                // if its a fucntion we just check the return type is correct
                else if (!strcmp(node->children[0]->children[0]->data, "("))
                {
                    if (tableTypeSearch(table, node->children[0]->data, scope) != '#')     // if it is not
                    {
                        printf("ERROR: %s index must be of type integer\n", node->parent->data);
                        return 0;
                    } 
                }   
            }
        }
        
        // if id is used for right arg
        if (!isdigit(node->children[1]->data[0]))
        {            
            if (node->children[1]->numChildren == 0)
            {
                if (tableTypeSearch(table, node->children[1]->data, scope) != '#')
                {
                    printf("ERROR: %s index must be of type integer\n", node->parent->data);
                    return 0;
                }
            }
            else
            {
                 // if its an array index
                if (!strcmp(node->children[1]->children[0]->data, "["))        
                {
                    // checks array indexing is int array
                     if (tableTypeSearch(table, node->children[1]->data, scope) != '[')            // Remember to check for seg fault
                    {
                        printf("ERROR: %s index must be of type integer\n", node->parent->data);
                        return 0;
                    }
                }

                // if its a fucntion we just check the return type is correct
                else if (!strcmp(node->children[1]->children[0]->data, "("))
                {
                    if (tableTypeSearch(table, node->children[1]->data, scope) != '#')     // if it is not
                    {
                        printf("ERROR: %s index must be of type integer\n", node->parent->data);
                        return 0;
                    }
                    
                }
                
            }
        }


        // if it makes it through all the checks then the types are good
        return 1;
    }
}

int getArraySize(Table * table, char * id, char * scope)
{
    // go to table of scope given if not asked for global
    if(strcmp(scope, "global"))
    {    
        for (int i = 0; i < table->numChildren; i++)
        {
            if (table->children[i]->size > 0)
            {
                if (!strcmp(getCharVec(&table->children[i]->scope, 0), scope))
                {
                    table = table->children[i];     // go to that sub table
                }
            }
        }
    }
    

    // once we go to global parent table will be null
    while (table != NULL)
    {
        // search the current table rows for the variable
        for (int j = 0; j < table->size; j++)
        {
            if (!strcmp(getCharVec(&table->identifier, j), id))
            {
                if(!strcmp(getCharVec(&table->action, j), "Function Parameter"))
                {
                    // if we encounter N/A its becasue its a parameter so we let the compiler handle it
                    return -1;

                }
                else
                {
                    return atoi(getCharVec(&table->array, j));  // return just the char
                }
                
            }
            
        }

        // if we went through the table without finding it 
        table = table->parent;
    }
    
    // if we make it out here we could not find the id
    return 0;

        
}

int indexCheck(AST_Node * node, Table * symbolTable, char * scope)
{

    printf("\nIndex Checking %s and %s\n", node->parent->data, node->parent->children[1]->data);

    char varType = tableTypeSearch(symbolTable, node->parent->data, scope);

    printf("varType: %c Scope: %s\n", varType, scope);

    // first we check whether the variable we are trying to index is even an array
    if (varType != '[' && varType != ']')
    {
        printf("ERROR: Attempting to index %s when not of array type\n", node->parent->data);
        return 0;
    }


    // next we check if the expression is of type int
    if(node->parent->children[1]->numChildren == 0)
    {
        // if its an id
        if (!isdigit(node->parent->children[1]->data[0]))
        {
            if (tableTypeSearch(symbolTable, node->parent->children[1]->data, scope) != '#')
            {
                printf("ERROR: %s index operator expects integer type\n", node->parent->data);
                return 0;
            }
            
        }
        else
        {
            // if its a number we just make sure it is withing range
            if (atoi(node->parent->children[1]->data) >= getArraySize(symbolTable, node->parent->data, scope))
            {
                // check index is not referencing function paramater
                if(getArraySize(symbolTable, node->parent->data, scope) != -1) // func returns -1 becasue func param arrays declare no size
                {
                    printf("ERROR: %s indexing is out of bounds\n", node->parent->data);
                    return 0;
                }
                
            }
            
        }
        
    }

    // if index arg had children
    else
    {
        // check if its an expression we need to simplify
        if (isOperator(node->parent->children[1]))
        {
            if (!typeCheckIndexExpr(symbolTable, node->parent->children[1], scope))
            {
                return 0;
            }
            
        }
        else
        {
            // if its a function call check return type of function
            if (node->parent->children[1]->children[0]->data[0] == '(')
            {
                if (getType(symbolTable, node->parent->children[1]->data)[0] != '#')
                {
                    printf("ERROR: %s index operator expects integer type\n", node->parent->data);
                    return 0;
                }
            }
            // if array indexing check id is int array type
            else if (node->parent->children[1]->children[0]->data[0] == '[')
            {
                if (getType(symbolTable, node->parent->children[1]->data)[0] != '[')
                {
                    printf("ERROR: %s index operator expects integer type\n", node->parent->data);
                    return 0;
                }
            }
            
        }
        
    }
    
    
    // sucessful check
    printf("Index check successful\n");
    return 1;

}

// will go to nearest block and make sure it is a while that is trying to be escaped
int breakCheck(AST_Node * node)
{

    printf("\nChecking break stmt\n");

    // find nearest block or stop at root of tree
    while (strcmp(node->data, "B") && node->parent != NULL)
    {
        node = node->parent;
    }

    // check if we found the while or not
    if (strcmp(node->parent->data, "while"))
    {
        printf("ERROR: unexpected break \n");
    }
    
    // if we pass the check that means there was a while loop to break out of 
    printf("sucessful check \n");
    return 1;

}

// will be the main call to do type checking for some semantic rules
void typeCheck(AST_Node * root, Table * symbolTable) 
{
   // printf("type cheking %s\n", root->data);

    // When we have a ( we expect a function and its paremeters
    if (!strcmp(root->data, "("))
    {
        if(!functionCallCheck(root->parent, symbolTable))
        {
            return;         //stop checking becasue there is 
        }
    }
    
    /*
    if (!strcmp(root->data, "return"))
    {
        if (!returnCheck(root, symbolTable))
        {
            return;
        }
        
    }
    */

    if (!strcmp(root->data, "["))
    {
        // try to get scope of var indexing
        AST_Node * temp = getFuncReturnType(symbolTable, root);

        if (temp == NULL)   // if couldnt find a func at best try 
        {
            if (!indexCheck(root, symbolTable, "global"))
            {
                return;
            }
        }
        else
        {
            if (!indexCheck(root, symbolTable, temp->data))
            {
                return;
            }
        }
        
    }
    
    if (!strcmp(root->data, "break"))
    {
        breakCheck(root);
    }
    

    for (int i = 0; i < root->numChildren; i++)
    {
        typeCheck(root->children[i], symbolTable);
    }
    


}
