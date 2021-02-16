/*
    Dennis Canar
    10 - 27 - 2020
    CST - 405 - TOS101
*/

/*  add this becasue we want the compiler to know if this file is
    already loaded in, we do not want to load it again.
    (Specifically prevents redefinition from include in another .h file)*/
#pragma once

//Creating containers to simulate a table, 500 words
//30 characters max
typedef struct{
    char Token[500][30];
    char Type[500][30];
    char Error[500][100];   //to suggest error checks
    int lineNum[500]; //Array to keep track which line the token is on
    int numTokens;  //keep track of how many array rows used
}LexerTable;

//Here the node can have pointer to seven other nodes because we can have at most a rule that gives an RHS of 7 chars
struct Node{

    //Here is the data that will need be used to identify the node
    char symbol;
    char data[30];

    //Will point to sub nodes
    struct Node *one;
    struct Node *two;
    struct Node *three;
    struct Node *four;
    struct Node *five;
    struct Node *six;
    struct Node *seven;

    struct Node *parent;

};

struct LL1{
    
    int table[27][35];
    
};

struct Production{

    char LHS;
    char RHS[12][10];  //This allows me to have 12 rules with a max length of 10 chars
    char firsts[20];   //Will be used to hold firsts
    char follows[20];  //Hold the follow sets
    int firstsRules[20];  //hold what rule is used for each first 
};

// This struct will be made up of the Production type and will hold all our grammar
struct Grammar{
    struct Production rules[64];  
};

// this structure will help create our AST
typedef struct Nodes
{

    char data[30];                      // will hold important information about the node
    struct Nodes * parent;              // point to the Nodes parent
    struct Nodes ** children;           // will point to an array of pointers of type Nodes (dynamic container)
    int numChildren;

}AST_Node;

// we create a new type of variable
typedef struct charVecs
{
    char ** container;
    int size;

}charVec;

// create a new type of variable to represent our table
typedef struct Tables
{
    // these are my table headers and data will be stored within them
    charVec identifier;
    charVec type;
    charVec array;
    charVec funcArgs;
    charVec action; 
    charVec scope;


    // will be a dynamicaally allocated array pointing to children 
    struct Tables ** children; 
    struct Tables * parent;     // will make traversing out symbol table tree easier

    // will let us know how amy rows are within the table 
    int size;
    int numChildren;
}Table;

typedef struct ICtables
{
	charVec Op;
	charVec Arg1;
	charVec Arg2;
	charVec Result;

	int size;
    int tempCounter;
    charVec stack;

}IC_table;
