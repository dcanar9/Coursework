# OpenGL: Fractal Forest
### written in C++ using the OpenGL libraries

- To get the program to read a custom file the make sure that the file has one of 
these formats:

- Code does not work if the file has an extra empty line

               Len:4
               Deg: 22.5
               Iter:3
               maxDist: 225
               maxRad: 0.9
               F
               F->FF-[-F+F+F]+[+F-F-F]


               Stochastic
               len: 1
               rot: 30.0
               iter: 5
               F
               F(0.33): F[+F]F[-F]F
               F(0.33): F[+F]F
               F(0.34): F[-F]F

               len: 1
               rot: 25.7
               iter: 5
               maxDist: 309
               maxRad: 1.2
               F
               F->F[+F]F[-F]F
               
- The stochastic tree must have the stochastic string and nothing else on the first line

- There is currently no limit to the number of rules one can add

- For the non stochastic file the rule -> must be found to record the rule

- The stochastic file needs to have the F(): and in parenthesis the percentage values , the value must be out of a toal of 1.00 (e.g. 0.25, 0.25, 0.25, 0.25)

- To compile the code use the correct directory and have all files in the same directory as the .cpp file

- g++ FractalForest.cpp -lGLU -lglut -lGL

- After compilation type in the terminal "./a.out" without the quotation marks

- To close the program press the escape key

- The program does have a moving camera but it will move very slow if openGL is running at a low frame rate

