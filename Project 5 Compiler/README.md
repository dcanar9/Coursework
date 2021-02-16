# Hello Welcome, 

This compiler was created for a course taken at GCU. The compiler with some work could
be used on other LL1 context free grammar. 

The compiler begins by taking user input and running it through a lexer that I created. The lexer will tokenize the
strings separated by a space (blank character). The lexer will then print errors for any tokens that
could not be identified along with the line number in the text file that it was on. Once all the strings have been 
tokenized a syntax check is done on the file with our GCU-PL given grammar. The syntax check is done by 
using the rules from the GCU-PL, the rules are imported to the syntaxAnalyzer.h in the initialize function. 
The rules are created using a rule object. This organization helps create the LL1 table that will guide the compiler to
create a parse tree of the given code. Creation of the parse tree will verify the code is syntactically correct 
and help with the compiler process. The parse tree is later converted to a more refined abstract syntax tree and
a symbol table is created during the process. The table and tree are then used to run more serious checks that might
cause compiler errors (i.e., assignment before declaration, indexing out of bounds, illegal return type, etc.).
Once this last check is done the AST is used to create "MIPS like" three adress code. This three address code
can be exported/formatted to a text file and ran in the MIPS compiler. 

*Unfortunately the MIPS section was not commpleted 


**uses gcc compiler (more information below) **

There are two extra files in here for how the grammar for my compiler works as well as what all the abbreviations
for each termnial and non terminal are. 

The tree prints in left recursive order so the first node to print will be P and then the vairable
on its immideiate left (does not look like it but if a node is indeted and one line below it is 
the left child also the first child).


## To Compile:
- Make sure all these files are in the same directory to use the compiler:

        - lexer.h 
        - LexerOutput.txt
        - compilerStructs.h
        - SyntaxAnalyzer.h
        - SemanticAnalyzer.h
        - IntermediareCode.h
        - myCompiler.c


- Uses gcc compiler (cd to directory of the files) & use "gcc myCompiler.c -x c" (the flag is just in case the gcc compiler tries to compile with something else)
    - if the tree looks odd try making your terminal window wider
    - the more indented the node is printed the further it is from the root
    - then to run with your text file use ./a.out <filename> and it will run with the given input  <----- (how to use executable)
    - if the executable is ran without an argument after the ./a.out then it wil use what the lexer output had previously

    
