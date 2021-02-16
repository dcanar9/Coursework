
#include "compilerStructs.h"
#include<string.h>  // to use strcpy()
#include<stdio.h>   // printf()

/* I need to store the C-- grammar rules so we can use them in the 
   parse tree. The continaer will hold a LHS and RHS where
   the LHS is what will be replaced by the RHS
*/

/////////////////////////////////////Global Variables//////////////////////////////////////////////////////


//Global Variables Grammar, table, sizes
struct Grammar C_minus;
struct LL1 ll1;
int numProd = 64;
int numNonTerminals = 29;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Returns the number of rules a nonterminal has
int numRules(struct Production p){

    int count = 0;

    // Get num of rows by dividing whole matrix by row
    for(int i = 0; i < sizeof(p.RHS)/sizeof(p.RHS[0]); i++){
            if(p.RHS[i][0] == '0'){
                return count;
            }

            else{
                count++;
            }
    }

    // if all rules are filled
    return count;
}

 //this is used to find a rule based on the non terminal found in a production
int findRule(char c){

    for(int i = 0; i < numProd; i++){
        if (C_minus.rules[i].LHS == c){
            return i;
        }
    }

    //If we dont find our non terminal something is wrong
    return -1;

}


/*  I had problems using str cat so I needed to make my own function
    Also had difficulties with duplicates so the function will signal
    If we sucessfully appended a char or not*/
int append(char * array, char c){

    //printf("appending letter %c to string: %s", c, array);

    //First we have to find where to append the letter
    for (int i = 0; i < 20; i++) //iterate through string
    {
        
        if(array[i] == '\0'){   //if we find null char
            array[i] = c;      //change it to char we want to append
            return 1;             //leave function and signal char was appended
        }

        // if the char is already in the list, do not append and return 0 to signal
        else if(array[i] == c){
            return 0;
        }
    }

    printf("Array is FULL !!\n");
    return -1;
}
    
void appendInt(int * array, int i){

    //Tried to use sizeof() but since we are using pointer it returns 4 for byte length
    for(int j = 0; j < 20; j++){

        if(array[j] == -1){ // loop through array until we found position that is not taken
            array[j] = i;
            return;
        }

    }

    //If we reach the end of the array without finding an unused array slot
    printf("interger array is full!!!!!!!\n ");

}

void printIntArr(int * arr){
    
    //needed this extra loop to initialize int array (cannot use memset on int arr)
    for (int j = 0; j < 20; j++)
    {
        if (arr[j] != -1) //dont want to print null numbers
        {
            printf("%d ", arr[j]);
        }
        
    }
    printf("\n");
}

int trailAppend(char * array, char c){

    //printf("appending letter %c to string: %s", c, array);

    //First we have to find where to append the letter
    for (int i = 0; i < 7; i++) //iterate through string
    {
        // if the char is already in the list, do not append
        if(array[i] == c){
            return 1;
        }
        

        if(array[i] == '\0'){   //if we find null char
            array[i] = c;      //change it to char we want to append
            return 0;             //leave function
        }
    }
    return 0;
}

int isTerminal(char c){

    //A string of the termianls that are not in the alphabet
    const char * terminals = "-!=*+/<>&|[]{}();,#_";
    
    //Check if lowercase
    if('a' <= c && c <= 'z'){
        return 1;
    }

    //Check special symbols we use as terminals
    else 
    {
        for (int i = 0; i < strlen(terminals) ; i++)
        {
            if(terminals[i] == c){
                return 1;
            }
        }
        
    }

    // if we do not find a match
    return 0;

}

int epsilonChainCheck(char * start){

    int chain = 1; // boolean to see if epsilon occurs in each non terminal
    int index = 0; // used to iterate through string
    int max = strlen(start); // segmentation fault prevention 

    while (chain)
    {

        //if weve iterated to our last char already
        if(index >= max){
            return index;
        }

        chain = 0; // chain will be true if we find epsilon

        //go through all rules of first()
        for (int i = 0; i < numRules(C_minus.rules[findRule(start[index])]); i++)
        {
            if (!strcmp(C_minus.rules[findRule(start[index])].RHS[i], "e"))
            {
                chain = 1; // set chain to true if we do find an epsilon
            }
            
        }
        
        // check if there was an epsilon
        if (!chain)
        {
            return index+1; //return how many unions we will need
        }
        
        // move over one char in rule
        index++;
    }

    //if we somehow make it out of the while loop
    printf("Something went wrong: Epsilon chain check\n");
    return -1;
}

//This is used in the follow function to see how many elements to the right of our follow have epsilons in their first{} set
// and will return an int to tell us which firsts will be added to the follow set
int followChainCheck(char * RHS, int followIndex){

    int chain = 0; // a boolean to check for the chain of epsilons

    //Loop through our RHS staring at the element we need to follow
    for(int i = followIndex; i < strlen(RHS); i++){

        // turn bool to false to see if there is an epsilon in the first(RHS[i])
        chain = 0;

        //Iterate through the firsts to check for epsilon
        for (int j = 0; j < strlen(C_minus.rules[findRule(RHS[i])].firsts); j++){
            
            //if there is an epsilon in the first 
            if(C_minus.rules[findRule(RHS[i])].firsts[j] ==  'e'){
                chain = 1; //set to true because our first does have an epsilon
            }

        }
        
        // return number of consecutive epsilons in the RHS given, +1 to ensure we run once for the base case
        if (chain == 0){
            return (i-followIndex)+1;
        }

    }

    //if we have checked all the RHS chars and they all have epsilon
    return strlen(RHS)-followIndex;

}

//Function wil return a string contining first set
void getFirst(struct Production p, char * first, int * firstRules, int isRoot, int rootRule){

    // step one from geeks for geeks checks if terminal not epsilon
    if (isTerminal(p.LHS)){
        if(append(first, p.LHS)){   //Adding to first
            appendInt(firstRules, rootRule);    //if char was appended then we record what main rule was used
        }
    }

    else{
        //Here we loop through the RHS to look for a epsilon
        for(int i = 0; i < numRules(p); i++){

            //This i use to see what path we took at the root of our recursive call
            //will change what RHS we are on if we are at the root
            if(isRoot == 1){
                rootRule = i;       //setting equal to RHS number
            }

            if (!strcmp(p.RHS[i], "e") && isRoot != 0){  // if there is a epsilon
                if(append(first, p.RHS[i][0])){  // add to first 
                    appendInt(firstRules, rootRule);    //if char was appended then we record what main rule was used
                }
            }

            //if not epsilon
            else if(strcmp(p.RHS[i],"e")){
                
                //printf("start of chain: ");
                //will see the possible epsilons ahead and get appropriate number of firsts
                for (int j = 0; j < epsilonChainCheck(p.RHS[i]); j++)
                {

                    //will handle special case if last nonterminal in RHS string has epsilon
                    if(j == strlen(p.RHS[i])-1){  
                    
                        getFirst(C_minus.rules[findRule(p.RHS[i][j])], first, firstRules, 2, rootRule);
                        //Above we use is Root two to signal that epsilon is allowed to be appended
                        //but this is not the main function call
                    }

                    //All other non terminals before last non term
                    else
                    {
                        getFirst(C_minus.rules[findRule(p.RHS[i][j])], first, firstRules, 0, rootRule);
                    }

                }
            }
        }
    }
}

// Used in the follow function to add the first of a production to the follow of another
void addFirst(char * first, char * follow){

    //printf("In addFirst fundtion first: %s", first);

    //iterate through the chars in the first array
    for (int i = 0; i < strlen(first); i++){

        //we do not want to add epsilon to the follows array. Add everything else
        if(first[i] != 'e'){
            append(follow, first[i]);
            //printf("Appending->%c", first[i]);
        }
    }
    
}

void getFollow(char c, char * follow, char * stack){

    if (c == 'P') //This handles the case we want to follow our starting symbol
    {
        append(follow, '$');
    }
    
    /*  First we must iterate through the rules and check if the 
    productions LHS exists somewhere in another rules RHS */

    // Loop through the enitre grammars of non terminals
    for (int i = 0; i < numNonTerminals; i++){

        // Then we need to loop through each RHS for the specific non terminal 
        for (int j = 0; j < numRules(C_minus.rules[i]); j++){
            
            // This then loops through the RHS's chars
            for (int x = 0; x < strlen(C_minus.rules[i].RHS[j]); x++){
                
                if (C_minus.rules[i].RHS[j][x] == c){       //Once we find our char in the RHS

                    //Check if there is an element to the right of the char
                    if(x+1 < strlen(C_minus.rules[i].RHS[j])){

                        //Check if the follow contains an epsilon and if any after that do too
                        for (int z = 0; z < followChainCheck(C_minus.rules[i].RHS[j], x+1); z++){ 
                            
                            // if the first() has a chain of epsilons then we loop through the RHS and add the first (e.g consider A -> BCDE 
                            // C -> epsilon and D -> epsilon, then our loop will do follow(B) = first(C)-e U first(D)-e U first(E)

                            //Special case last char in RHS string has an epsilon transition & the follow can reach it
                            // here x+z+1 is out of index so that means the epsilon transition is also in the last nonterminal
                            if (x+z+1 == strlen(C_minus.rules[i].RHS[j])) {
                                getFollow(C_minus.rules[i].LHS, follow, stack);  // find follow of LHS
                            }

                            else{                        
                                //adding firsts to follow set, the +1 is to shift over a char from the one we were looking for
                                addFirst(C_minus.rules[findRule(C_minus.rules[i].RHS[j][x+z+1])].firsts, follow);  
                            }
                        }
                        //printf("Follow for %c = first(%c) = %s\n", c, C_minus.rules[i].RHS[j][x+1], C_minus.rules[i].firsts);
                        //addFirst(C_minus.rules[findRule(C_minus.rules[i].RHS[j][x+1])].firsts, follow);
                    }

                    //if there is no element to the right of the char & do not repeat original function call
                    else if(x+1 >= strlen(C_minus.rules[i].RHS[j]) && i != findRule(c)){

                         //here I push to a stack to see where i am recursively going 
                         //if we are stuck in a loop we have no follows
                        if(trailAppend(stack,C_minus.rules[i].LHS)){
                            return;
                        }
                        getFollow(C_minus.rules[i].LHS, follow, stack);

                    }
                        
                }
            }   
        }   
    }
}

void initTable(){

    //Iterate through rows
    for (int i = 0; i < sizeof(ll1.table)/sizeof(ll1.table[0]); i++)
    {
        //Then we iterate through the columns
        for (int j = 0; j < sizeof(ll1.table[0])/sizeof(ll1.table[0][0]); j++)
        {
            ll1.table[i][j] = -1; //Set table to some null number
        }
        
    }
    
}

int findColumn(char c){

    //Iterate through all the rules to find our terminal
    for (int i = numNonTerminals; i < numProd; i++)
    {
        if (C_minus.rules[i].LHS == c){
            return i - numNonTerminals;
        }
    }
    
    //problem if we do not find the terminal
    return -1;

}

void addFollowToTable(int ruleNum, int rhsNum){

    int columnNum;
    char current;

    for (int i = 0; i < strlen(C_minus.rules[ruleNum].follows); i++)
    {

        //get char from follow set
        current = C_minus.rules[ruleNum].follows[i];
        columnNum = findColumn(current);
        

        //we dont want to over write the first sets in the table
        if (ll1.table[ruleNum][columnNum] == -1)
        {
            ll1.table[ruleNum][columnNum] = rhsNum;
        }
        
    }
    


}

void initialize(){

    /*  Before we add the rules we will clear the 2d array
        by looping through each production and using memset()
        to set all 2D array vals to 0
    */
    for(int i = 0; i < numProd ; i++){
        memset(C_minus.rules[i].RHS, '0', sizeof(C_minus.rules[i].RHS));
        memset(C_minus.rules[i].firsts, '\0', sizeof(C_minus.rules[i].firsts));
        memset(C_minus.rules[i].follows, '\0', sizeof(C_minus.rules[i].follows));

        //needed this extra loop to initialize int array (cannot use memset on int arr)
        for (int j = 0; j < sizeof(C_minus.rules[i].firstsRules)/sizeof(int); j++)
        {
            C_minus.rules[i].firstsRules[j] = -1;
        }
        

    }    

    // load the production rules into the grammar struct
    C_minus.rules[0].LHS = 'P';                //Program
    strcpy(C_minus.rules[0].RHS[0], "VF");

    C_minus.rules[1].LHS = 'V';                //VarDeclList
    strcpy(C_minus.rules[1].RHS[0], "e");
    strcpy(C_minus.rules[1].RHS[1], "WV");
        
    C_minus.rules[2].LHS = 'W';                //VarDecl
    strcpy(C_minus.rules[2].RHS[0], "Ti1");

    C_minus.rules[3].LHS = '1';                //VarDecl`
    strcpy(C_minus.rules[3].RHS[0], ";");
    strcpy(C_minus.rules[3].RHS[1], "[n];");

    C_minus.rules[4].LHS = 'F';                //FunDecList
    strcpy(C_minus.rules[4].RHS[0], "G2");

    C_minus.rules[5].LHS = '2';                //FunDecList`
    strcpy(C_minus.rules[5].RHS[0], "e");
    strcpy(C_minus.rules[5].RHS[1], "F");

    C_minus.rules[6].LHS = 'G';                //FunDecl
    strcpy(C_minus.rules[6].RHS[0], "Ti(A)B");

    C_minus.rules[7].LHS = 'A';                //ParamDecList
    strcpy(C_minus.rules[7].RHS[0], "e");
    strcpy(C_minus.rules[7].RHS[1], "R");

    C_minus.rules[8].LHS = 'R';                //ParamDeclListTail
    strcpy(C_minus.rules[8].RHS[0], "Q3");
        
    C_minus.rules[9].LHS = '3';                //ParamDeclListTail`
    strcpy(C_minus.rules[9].RHS[0], "e");
    strcpy(C_minus.rules[9].RHS[1], ",R");

    C_minus.rules[10].LHS = 'Q';                //ParamDecl
    strcpy(C_minus.rules[10].RHS[0], "Ti4");

    C_minus.rules[11].LHS = '4';                //ParamDecl`
    strcpy(C_minus.rules[11].RHS[0], "e");
    strcpy(C_minus.rules[11].RHS[1], "[]");

    C_minus.rules[12].LHS = 'B';                //Block
    strcpy(C_minus.rules[12].RHS[0], "{VL}");
        
    C_minus.rules[13].LHS = 'T';                //Type
    strcpy(C_minus.rules[13].RHS[0], "#");
    strcpy(C_minus.rules[13].RHS[1], "c");

    C_minus.rules[14].LHS = 'L';               //StmtList
    strcpy(C_minus.rules[14].RHS[0], "S5");

    C_minus.rules[15].LHS = '5';               //StmtList`
    strcpy(C_minus.rules[15].RHS[0], "e");
    strcpy(C_minus.rules[15].RHS[1], "L");

    C_minus.rules[16].LHS = 'S';               //Stmt
    strcpy(C_minus.rules[16].RHS[0], ";");
    strcpy(C_minus.rules[16].RHS[1], "Z;");
    strcpy(C_minus.rules[16].RHS[2], "rE;");
    strcpy(C_minus.rules[16].RHS[3], "di;");
    strcpy(C_minus.rules[16].RHS[4], "wE;");
    strcpy(C_minus.rules[16].RHS[5], "_;");
    strcpy(C_minus.rules[16].RHS[6], "b;");
    strcpy(C_minus.rules[16].RHS[7], "f(E)SlS");
    strcpy(C_minus.rules[16].RHS[8], "u(E)S");
    strcpy(C_minus.rules[16].RHS[9], "B");

    C_minus.rules[17].LHS = 'Z';                //Astmt
    strcpy(C_minus.rules[17].RHS[0], "i6");

    C_minus.rules[18].LHS = 'E';               //Expr
    strcpy(C_minus.rules[18].RHS[0], "IN");
    strcpy(C_minus.rules[18].RHS[1], "UEN");
    strcpy(C_minus.rules[18].RHS[2], "i6");
        
    C_minus.rules[19].LHS = '6';               //Expr!
    strcpy(C_minus.rules[19].RHS[0], "=EN");
    strcpy(C_minus.rules[19].RHS[1], "[E]=EN");
        
    C_minus.rules[20].LHS = 'N';               //Expr`
    strcpy(C_minus.rules[20].RHS[0], "e");
    strcpy(C_minus.rules[20].RHS[1], "OEN");

    C_minus.rules[21].LHS = 'I';               //Primary
    strcpy(C_minus.rules[21].RHS[0], "i7");
    strcpy(C_minus.rules[21].RHS[1], "n");
    strcpy(C_minus.rules[21].RHS[2], "(E)");

    C_minus.rules[22].LHS = '7';               //Primary`
    strcpy(C_minus.rules[22].RHS[0], "e");
    strcpy(C_minus.rules[22].RHS[1], "(X)");
    strcpy(C_minus.rules[22].RHS[2], "[E]");

    C_minus.rules[23].LHS = 'X';               //ExprList
    strcpy(C_minus.rules[23].RHS[0], "e");
    strcpy(C_minus.rules[23].RHS[1], "Y");

    C_minus.rules[24].LHS = 'Y';               //ExprListTail
    strcpy(C_minus.rules[24].RHS[0], "E8");

    C_minus.rules[25].LHS = '8';               //ExprListTail`
    strcpy(C_minus.rules[25].RHS[0], "e");
    strcpy(C_minus.rules[25].RHS[1], ",Y");

    C_minus.rules[26].LHS = 'O';               //BinaryOp
    strcpy(C_minus.rules[26].RHS[0], "+");
    strcpy(C_minus.rules[26].RHS[1], "-");
    strcpy(C_minus.rules[26].RHS[2], "*");
    strcpy(C_minus.rules[26].RHS[3], "/");
    strcpy(C_minus.rules[26].RHS[4], "q");
    strcpy(C_minus.rules[26].RHS[5], "z");
    strcpy(C_minus.rules[26].RHS[6], "<");
    strcpy(C_minus.rules[26].RHS[7], ">");
    strcpy(C_minus.rules[26].RHS[8], "g");
    strcpy(C_minus.rules[26].RHS[9], "h");
    strcpy(C_minus.rules[26].RHS[10], "&");
    strcpy(C_minus.rules[26].RHS[11], "|");

    C_minus.rules[27].LHS = 'U';                //UnaryOp
    strcpy(C_minus.rules[27].RHS[0], "-");
    strcpy(C_minus.rules[27].RHS[1], "!");

    // below are all the terminals
    C_minus.rules[28].LHS = '-';
    C_minus.rules[29].LHS = '!';
    C_minus.rules[30].LHS = '=';
    C_minus.rules[31].LHS = '*';
    C_minus.rules[32].LHS = '+';
    C_minus.rules[33].LHS = '/';
    C_minus.rules[34].LHS = 'q';
    C_minus.rules[35].LHS = 'z';
    C_minus.rules[36].LHS = '<';
    C_minus.rules[37].LHS = '>';
    C_minus.rules[38].LHS = 'g';
    C_minus.rules[39].LHS = 'h';
    C_minus.rules[40].LHS = '&';
    C_minus.rules[41].LHS = '|';
    C_minus.rules[42].LHS = '{';
    C_minus.rules[43].LHS = '}';
    C_minus.rules[44].LHS = '(';
    C_minus.rules[45].LHS = ')';
    C_minus.rules[46].LHS = '[';
    C_minus.rules[47].LHS = ']';
    C_minus.rules[48].LHS = ';';
    C_minus.rules[49].LHS = ',';
    C_minus.rules[50].LHS = 'c';
    C_minus.rules[51].LHS = '#';
    C_minus.rules[52].LHS = 'i';
    C_minus.rules[53].LHS = 'n';
    C_minus.rules[54].LHS = 'e';
    C_minus.rules[55].LHS = 'r';
    C_minus.rules[56].LHS = 'd';
    C_minus.rules[57].LHS = 'w';
    C_minus.rules[58].LHS = '_';
    C_minus.rules[59].LHS = 'b';
    C_minus.rules[60].LHS = 'f';
    C_minus.rules[61].LHS = 'l';
    C_minus.rules[62].LHS = 'u';
    C_minus.rules[63].LHS = '$';


    //Computing first() for all prouctions
    for(int i = 0; i < numProd; i++){       
        getFirst(C_minus.rules[i], C_minus.rules[i].firsts, C_minus.rules[i].firstsRules, 1, 0);
    }

    
    // Getting follows for all non terminals 
    for (int i = 0; i < numNonTerminals; i++){
        char stack[] = {'\0','\0','\0','\0','\0','\0','\0'};
        getFollow(C_minus.rules[i].LHS, C_minus.rules[i].follows, stack);
       
    }
    
}

void fixGrammarErrors(){

    //Rule 11 (symbol: 4) does not have ) in follow and they should, error with grammar maybe
    append(C_minus.rules[11].follows, ')'); 
    append(C_minus.rules[22].follows, ';'); 
    append(C_minus.rules[22].follows, ')');
    append(C_minus.rules[22].follows, ']'); 
    append(C_minus.rules[22].follows, ';');
    append(C_minus.rules[22].follows, '|');
    append(C_minus.rules[22].follows, ',');
}

void printSets()
{

    // Printing header then first sets
    printf("\t\tFirst\n");
    for(int i = 0; i < numNonTerminals; i++){
        printf("Non-Terminal: %c | First: %s\n", C_minus.rules[i].LHS, C_minus.rules[i].firsts);
    }

    //Follow sets
    printf("\n\t\tFollows\n");
    for (int i = 0; i < numNonTerminals; i++){        
        printf("Non-Terminal: %c | Follows: %s\n", C_minus.rules[i].LHS, C_minus.rules[i].follows);
    }


}

void loadTable(){

    int columnNum;
    char current;
    int epsilon; // used as a boolean to see if i have an epsilon in the first set

    //we will load the table row by row iterating through each rule
    for (int i = 0; i < numNonTerminals; i++)
    {
        epsilon = -1; //false until we find epsilon 

        //Then we iterate through the firsts for that non terminal
        for (int j = 0; j < strlen(C_minus.rules[i].firsts); j++)
        {
            current = C_minus.rules[i].firsts[j]; // get char of first we are looking for
            columnNum = findColumn(current);  //Find what column  number needed

            ll1.table[i][columnNum] = C_minus.rules[i].firstsRules[j];

            /*  if there is an epsilon in the first set then we add the 
                follow set to the table. We do this by using another function 
                to take that epsilon rule and add it to the apprpriate terminals */
            if(current == 'e'){
                epsilon = j;    //save what rule gave us epsilon transition
            }
        }

        if (epsilon != -1)
        {
            addFollowToTable(i, C_minus.rules[i].firstsRules[epsilon]);
        }
        
    }
}

void printTable(){

    printf("    ");

    //first i can print the terminal heading 
    for (int i = numNonTerminals; i < numProd; i++)
    {
        printf("%c  ", C_minus.rules[i].LHS);
    }

    printf("\n");
    

    //Iterate through rows
    for (int i = 0; i < sizeof(ll1.table)/sizeof(ll1.table[0]); i++)
    {
        //Then we iterate through the columns
        for (int j = 0; j < sizeof(ll1.table[0])/sizeof(ll1.table[0][0]); j++)
        {
            //Used to print left side headers
            if(j == 0){
                printf("%c   ", C_minus.rules[i].LHS);
            }

            if (ll1.table[i][j] == -1)
            {
                printf("   ");
            }
            
            else{
                printf("%d ", ll1.table[i][j]); //print cell
            }
        }
        
        printf("\n");

    }
    
}

void getData(char * tk, char * type, char * lNum, char * line){

    //markerused to parse words and to count semicolons
    int left = 1, semiC = 0;

    // quick check to see format is correct
    if (line[0] != '{')
    {
        printf("THERE IS AN ERROR IN THE INPUT FORMAT!\n");
        return; //end data parsing
    }


    //iterate through the line starting after { and first char
    for (int i = 2; i < strlen(line); i++)
    {
        //when we find our delimeter
        if(line[i]==';')
        {
            semiC++;        //increment counter

            if (semiC == 1)    //when we find first delimeter
            {
                strncpy(tk, line + left, i-left); //copy token 
                left = i+1;     //Set marker to left side of sub string we want
                //printf("leftMarker: %d\n", left);
            }
            
            else if(semiC == 2) //when we find the second delimeter
            {
                strncpy(type, line + left, i-left); //copy token type 
                tk[i] = '\0';
                left = i+1;
                
            }

            else if(semiC == 3) //final delimeter
            {
                strncpy(lNum, line + left, i-left); //copy line number 
            }
        }   
    }
}

void appendTo(char * arr, char c, int arrSize){

    //iterate until we find null place to put char
    for (int i = 0; i < arrSize; i++)
    {
        if (arr[i] == '\0')
        {
            arr[i] = c;
            return;
        }    
    }
    
    printf("!No more room in array!\n");
    return;

}

//this function reads the input and changes it into a symbol the grammar recognizes
void addToInput(char * tk, char * type, char * inputArr){

    //printf("Adding to input: %s\n", tk);

    //if num
    if(!strcmp(type, "Constant"))
    {
        appendTo(inputArr, 'n', 500);
    }
    
    else if (!strcmp(type, "Identifier"))
    {
        appendTo(inputArr, 'i', 500);
    }

    //if its a special symbol we can just append the token itself
    else if(!strcmp(type, "Special Symbol"))
    {
        appendTo(inputArr, tk[0], 500);
    }

    // if we have a keyword we look to see which one
    else if (!strcmp(type, "Keyword"))
    {

        if (!strcmp(tk, "char"))
        {
            appendTo(inputArr, 'c', 500);
        }

        else if (!strcmp(tk, "int"))
        {
            appendTo(inputArr, '#', 500);
        }
        
        else if (!strcmp(tk, "return"))
        {
            appendTo(inputArr, 'r', 500);
        }

         else if (!strcmp(tk, "read"))
        {
            appendTo(inputArr, 'd', 500);
        }

        else if (!strcmp(tk, "write"))
        {
            appendTo(inputArr, 'w', 500);
        }
        
        else if (!strcmp(tk, "writeln"))
        {
            appendTo(inputArr, '_', 500);
        }

        else if (!strcmp(tk, "break"))
        {
            appendTo(inputArr, 'b', 500);
        }

        else if (!strcmp(tk, "if"))
        {
            appendTo(inputArr, 'f', 500);
        }

        else if (!strcmp(tk, "else"))
        {
            appendTo(inputArr, 'l', 500);
        }

        else if (!strcmp(tk, "while"))
        {
            appendTo(inputArr, 'u', 500);
        }
    }

    else if (!strcmp(type, "BinaryOp") || !strcmp(type, "UnaryOp") || !strcmp(type, "Equals"))
    {

        //operators with string length of one are represented by the input
        if(strlen(tk) == 1)
        {
            appendTo(inputArr, tk[0], 500);
        }

        else if(!strcmp(tk, "=="))
        {
            appendTo(inputArr, 'q', 500);
        }

        else if(!strcmp(tk, "!="))
        {
            appendTo(inputArr, 'z', 500);
        }

        else if(!strcmp(tk, "<="))
        {
            appendTo(inputArr, 'g', 500);
        }

        else if(!strcmp(tk, ">="))
        {
            appendTo(inputArr, 'h', 500);
        }

        else if(!strcmp(tk, "&&"))
        {
            appendTo(inputArr, '&', 500);
        }

        else if(!strcmp(tk, "||"))
        {
            appendTo(inputArr, '|', 500);
        }

    }

}

void getInput(FILE ** file, char * in){

    //What we want from the lexer will be stored here temporarily
    char line[60];

    //temp variables
    char token[50];
    char type[20];
    char lineNum[4];  

    //Get input from the Lexer line by line
    while (fgets(line, 60, *file) != NULL)
    {

        //reset temp variables
        memset(token, '\0', sizeof(token));
        memset(type, '\0', sizeof(type));
        memset(lineNum, '\0', sizeof(lineNum));

        //get the token, type, and line number 
        getData(token, type, lineNum, line);
        
        //we need to take the input and convert it to what our grammar recognizes
        addToInput(token, type, in);
    }

    //Final char in input will b $ to know when weve hit the bottom
    appendTo(in, '$', 500);
}

void push(char * Stack, char c, int size)
{
    for (int i = size-1; i >= 0 ; i--)
    {
        //Traverse up stack until we find top
        if(Stack[i] == '\0')
        {
            Stack[i] = c; //place char at top
            return;     //leave function
        }
    }

    //If we make it outside of the loop
    printf("No more room in stack!!\n");
    
}

void pushString(char * Stack, char * string, int size)
{
    for (int i = strlen(string)-1; i >= 0; i--)
    {
        push(Stack, string[i], size);
    }
    
}

void pop(char * Stack, int size)
{
    // iterate from top of container to find element on top and delete
    for (int i = 0; i < size ; i++)
    {   
        
        if(Stack[i] != '\0')
        {
            Stack[i] = '\0';    //delete top char on stack
            return;
        }
    }

    //If we make it outside of the loop
    printf("Cant pop stack empty!!\n");
    
}

void printStack(char * Stack, int size){

    for (int i = 0; i < size; i++)
    {
        if(Stack[i] != '\0')
        {
            printf("%c", Stack[i]);
        }
    }
    printf("\n");

}

char top(char * container, int size)
{
    // loop throgh the array until we find top char
    for (int i = 0; i < size; i++)
    {
        if(container[i] != '\0')
        {
            return container[i];
        }
    }

    printf("The container is empty could not return!!\n");
    return '~';
    
}

char lookAhead(char * input, int offset)
{

    // look for top element then return look ahead char
    for (int i = 0; i < 500; i++)
    {
        if(input[i] != '\0'){
            return input[i+offset];
        }
    }
    
    printf("Empty Container! in Look Ahead!\n");
    return '\0';

}

//Will return what we have in the table for given variables (row, column)
int useTable(char nonT, char terminal, char * input)
{
    //markers for when we want to read from the table
    int row = findRule(nonT); 
    int column = findColumn(terminal);

    //There are some cases where we need to look ahead so we do here
    if (nonT == 'V')
    {
        //Look ahead to see if we find a (
        if(lookAhead(input, 2) == '(')
        { 
            column = findColumn('e');
            return ll1.table[row][column];
        }
        else
        {
            return ll1.table[row][column];
        }
        
    }
    
    
    if (nonT == 'E')
    {
        //Have a look ahead case becasue we can get to id two different ways
        if (lookAhead(input, 1) == '=')
        {
            return 2; // we want it to use this rule instead of the other recorded on the table
        }
        else if(lookAhead(input, 4) == ';')
        {
            return 0;  //return rule 0 for E 
        }
        
        
    }
    

    //If table has no data for this specific rule and terminal
    if(ll1.table[row][column] == -1)
    {
        printf("There is no transition for this input!!\n");
    }

    //If none of the rules above appy continue with how LL1 tbale should work
    return ll1.table[row][column];

}

void updateStep(char * Step, char LHS, int ruleNum){

    appendTo(Step, LHS, 10);
    strcat(Step, "-->");
    strcat(Step, C_minus.rules[findRule(LHS)].RHS[ruleNum]);

}

// will alocate new memory and return pointer to it
struct Node* newNode(char symbol)
{   
                            //Cast pointer type to allocated memory
    struct Node * node = (struct Node *)malloc(sizeof(struct Node));

    //intialize node with its char
    node->symbol = symbol;

    //Set other pointer to null because we do not know what they will be
    node->one = NULL;
    node->two = NULL;
    node->three = NULL;
    node->four = NULL;
    node->five = NULL;
    node->six = NULL;
    node->seven = NULL;
    node->parent = NULL;

    // makes sure data is clear
    memset(node->data, '\0', sizeof(node->data));

    //When done initializing return node
    return node;
}

//Will be called at the end to free our allocated memory
void deleteTree(struct Node * Root)
{
    //printf("Deleting Node Symbol: %c\n", Root->symbol);

    //Call recursively on children of Node if pointer not NULL
    if (Root->one)
    {
        deleteTree(Root->one);
    }
    if (Root->two)
    {
        deleteTree(Root->two);
    }
    if (Root->three)
    {
        deleteTree(Root->three);
    }
    if (Root->four)
    {
        deleteTree(Root->four);
    }
    if (Root->five)
    {
        deleteTree(Root->five);
    }
    if (Root->six)
    {
        deleteTree(Root->six);
    }
    if (Root->seven)
    {
        deleteTree(Root->seven);
    }

    //After recursive call free allocated memory
    //printf("Deleting Node Symbol: %c\n", Root->symbol);
    free(Root);

}

//will return child mostleft + index 
int isChild(struct Node * parent, struct Node * child, int index)
{
    
    //printf("Found: %c\n", (*node)->symbol);
    switch (index)      
    {
        case 0:
            if (parent->one == child)
            {
                return 1;
            }
            break;
        case 1:
            if (parent->two == child)
            {
                return 1;
            }
            break;
        case 2:
            if (parent->three == child)
            {
                return 1;
            }
            break;
        case 3:
            if (parent->four == child)
            {
                return 1;
            }
            break;
        case 4:
            if (parent->five == child)
            {
                return 1;
            }
            break;
        case 5:
            if (parent->six == child)
            {

                return 1;
            }
            break;
        case 6:
            if (parent->seven == child)
            {
                return 1;
            }
            break;
    }

    //if there was no match
    return 0;

}

/*  Will add another level to the tree by adding the children to the tree
    and updating pointer*/
void addChildren(struct Node ** node, char c, int ruleNum)
{

    // RHS that is being used for parsing
    char * RHS = C_minus.rules[findRule(c)].RHS[ruleNum];

    // Here we loop though the length of the RHS string becasuse each char will be it own node
    for (int i = 0; i < strlen(RHS) ; i++)
    {

        //Create new node with appropriate char inside 
        struct Node * New = newNode(RHS[i]);

        //Switch case so we dont overwrite children pointers
        switch (i)
        {
        case 0:
            New->parent = *node; // Point parent to original node we were passed
            (*node)->one = New;    //Update parent node to point to child
            break;
        case 1:
            New->parent = *node;
            (*node)->two = New;
            break;
        case 2:
            New->parent = *node;
            (*node)->three = New;
            break;
        case 3:
            New->parent = *node;
            (*node)->four = New;
            break;
        case 4:
            New->parent = *node;
            (*node)->five = New;
            break;
        case 5:
            New->parent = *node;
            (*node)->six = New;
            break;
        case 6:
            New->parent = *node;
            (*node)->seven = New;
            break;
        }
    }
    

}

void goToNext(struct Node ** node, char topStack){

    //printf("Parent Node: %c\n", (*node)->parent->symbol);
    //printf("Looking for symbol: %c\n", topStack);

    int childNum;
                                    // if node has no parent AKA root of tree
    if ((*node)->parent == NULL) 
    {
        printf("Could not find next node!\n");
        return; //leave function 
    }

    
    //first we find out what child we are
    for (int i = 0; i < 7; i++)
    {
        if(isChild((*node)->parent, *node, i))
        {
            childNum = i;
            //printf("Is Child num: %d\n", i);
        }
    }

                                        //look to right of current node for symbol 
    for (int j = childNum+1; j < 7; j++)
    {
        switch (j)      //  excluded case 0 here becasue at best node would be child 0 and we do not want to
        {               //  check the value of the node we currently are. Also we do not check left side of current node
            case 1:
                if (!((*node)->parent->two))   //if child 2 is NULL pointer
                {
                    break;
                }
                
                else if ((*node)->parent->two->symbol == topStack)
                {
                    *node = (*node)->parent->two;
                    //printf("Comparing to Node:%c\n", (*node)->parent->two->symbol);
                    //printf("Upadted Pointer in Func: %c\n", (*node)->symbol);
                    return;
                }
                break;
            case 2:
                if (!((*node)->parent->three))   
                {
                    break;
                }
                
                else if ((*node)->parent->three->symbol == topStack)
                {
                    *node = (*node)->parent->three;
                    //printf("Comparing to Node:%c\n", node->parent->three->symbol);
                    return;
                }
                break;
            case 3:
                if (!((*node)->parent->four))   
                {
                    break;
                }
                
                else if ((*node)->parent->four->symbol == topStack)
                {
                    *node = (*node)->parent->four;
                    //printf("Comparing to Node:%c\n", node->parent->four->symbol);
                    return;
                }
                break;
            case 4:
                if (!((*node)->parent->five))   //if child 2 is NULL pointer
                {
                    break;
                }
                
                else if ((*node)->parent->five->symbol == topStack)
                {
                    *node = (*node)->parent->five;
                    return;
                }
                break;
            case 5:
                if (!((*node)->parent->six))   //if child 2 is NULL pointer
                {
                    break;
                }
                
                else if ((*node)->parent->six->symbol == topStack)
                {
                    *node = (*node)->parent->six;
                    return;
                }
                break;
            case 6:
                if (!((*node)->parent->seven))   //if child 2 is NULL pointer
                {
                    break;
                }
                
                else if ((*node)->parent->seven->symbol == topStack)
                {
                    *node = (*node)->parent->seven;
                    return;
                }
                break;
        }

    }
        
    //if the top of the stack is not a node on our layer call fucntion on parent
    *node = (*node)->parent;
    goToNext(node, topStack);
        
    
}

void printTree(struct Node * root, int depth)
{

    for (int i = 0; i < depth; i++)
    {
        printf("    ");
    }

    depth++;

    if(root->data[0] != '\0')
    {
        printf("%s\n", root->data);
    }
    else
    {
        printf("%c\n", root->symbol);
    }
    
    if(root->one)
    {
        printTree(root->one, depth);
    }
    if(root->two)
    {
        printTree(root->two, depth);
    }
    if(root->three)
    {
        printTree(root->three, depth);
    }
    if(root->four)
    {
        printTree(root->four, depth);
    }
    if(root->five)
    {
        printTree(root->five, depth);
    }
    if(root->six)
    {
        printTree(root->six, depth);
    }
    if(root->seven)
    {
        printTree(root->seven, depth);
    }

}

void addData(char * stack, struct Node ** node, FILE ** File)
{

    int tokenNum;

    // iterate through stack to find token number (500 comes from stack len in main())
    for(int i = 0; i < 500; i++)
    {
        // until we find non null char
        if(stack[i] != '\0')
        {
            tokenNum = i;
            break;
        }
    }

    //Read that token number from the the Lexer Output
    //Get input from the Lexer line by line
    int lineCount = 0;      //counters
    char line[60];          //will hold line read from file
    
    // reset pointer in file to beginning of file
    rewind(*File);

    while (fgets(line, 60, *File) != NULL)
    {
        //Once we get to the tokens whos data we need
        if (lineCount == tokenNum)
        {            

            //iterate through line (start at one to skip bracket in line)
            for(int j = 1; j < strlen(line); j++)
            {
                if(line[j] == ';'){break;}      //leave loop when done reading from data section

                appendTo((*node)->data, line[j], 30);
            }

            break;          //leave while loop once done adding data
        }
        lineCount++;            // increment line counter
    }
    //printf("Added Data to Node %c --> %s\n", (*node)->symbol, ((*node)->data));

}

void parse(char * Stack, char * In, struct Node ** Root, FILE ** File)
{
    //              ===========Variables========
    //Will be used to clearly see what is at the top of our stack and input
    char sTop = top(Stack, 500), iTop = top(In, 500);  //Initialize

    int tableVal;                       //Will hold rule we need to use temporarily

    ////////////////////////////////////////////////////////////////////////////////

    /*  The function will run until we reach the end symbol on both
        the stack and the input container                        */

    while (!(sTop == '$' && iTop == '$'))
    {
        //When the tops of our containers match we can pop and update the tops
        if(sTop == iTop)
        {
            //printf("Action: Popping--> %c\n", sTop);

            // if we created a node for an id or num add that data to the node
            if(sTop == 'i' || sTop == 'n')
            {   
                addData(In, Root, File);  //will take data from lexer to create parse tree
            }

            pop(Stack, 500);
            pop(In, 500);
            sTop = top(Stack, 500);
            iTop = top(In, 500);

            // update our tree pointer to point to parent of current node
            goToNext(Root, sTop);
            //printf("Updated Curr Pointer: %c\n", (*Root)->symbol);
        }

        //If we dont have matching chars then we must do a rule to continue
        else
        {
            tableVal = useTable(sTop, iTop, In);

            if(tableVal == -1)
            {
                printf("ERROR COULD NOT GET TRANSITION FROM -> %c -> %c\n", sTop, iTop);
                return;     //end parsing if we cannot continue to make parse tree
            }

            //update rule used for current iteration 
            //printf("Action: %c --> %s\n", sTop, C_minus.rules[findRule(sTop)].RHS[tableVal]);

            //Add node to parse tree
            addChildren(Root, sTop, tableVal);
            *Root = (*Root)->one;       // move to left most child

            //Pop LHS that we just used
            pop(Stack, 500);

            //push RHS of rule to stack if not epsilon transition
            if(strcmp(C_minus.rules[findRule(sTop)].RHS[tableVal], "e"))
            {
                pushString(Stack, C_minus.rules[findRule(sTop)].RHS[tableVal], 500);
                //update top of our stack
                sTop = top(Stack, 500);
            }
            else
            {
                //update top of our stack
                sTop = top(Stack, 500); 
                goToNext(Root, sTop);
            }
            
            //printf("Updated Curr Pointer: %c\n", (*Root)->symbol);
            
        }
        /*
        // print to see what is going on in the parser
        printf("Stack: ");
        printStack(Stack, 500);
        printf("Input: ");
        printStack(In, 500);
        printf("\n");
        */
        
    }

    //If you make it out of the loop that means the input parsed with no error
    //printf("CONGRATS INPUT CONTAINED 0 ERRORS\n\n");
    return;

}
