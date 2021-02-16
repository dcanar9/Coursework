** uses gcc compiler (more information below) **

There are two extra files in here for how the grammar for my compiler works as well as what all the abbreviations
for each termnial and non terminal are. 

The tree prints in left recursive order so the first node to print will be P and then the vairable
on its immideiate left (does not look like it but if a node is indeted and one line below it is 
the left child also the first child).


To Compile:
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

    
