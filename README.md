# Hello Welcome! 
Below is a quick view of some of the work I have done over the years while attending GCU. Each project will have a title,
description, and some sample of the result. If you would like to see the source code for the project please click the title, it 
should bring you to the git where you can see the code along with a README file.

Thanks you for taking the time to look at my work if you would like to contact me please do so via:
* [Email](mailto:anthony.canar@gmail.com)
* [Linked In](https://www.linkedin.com/in/dennis-canar-b60929178/)

<img src="https://github.com/dcanar9/Coursework/blob/master/images/me.jpg?raw=true" width="300" height="400">

---

# [Generic Vector Class in C++](https://github.com/dcanar9/Coursework/tree/master/MyGenericVec)
![Generic Vector Class IMG](/images/GenericVector.png)

The purpose of this project was to show how templating works in C++ and to learn the basic operations
an array can preform. Although vectors already exist, this project taught how the class handles the 
undelying data structure. To view a code run through and sample run, click this [link](https://www.useloom.com/share/b005120a36574512bde76d4549da7698).

**if you want to know how to use the header file its best to watch the video**

---
<br/>

# [Card Game Class in C++](https://github.com/dcanar9/Coursework/tree/master/Card%20Game%20Files)

<img src="https://github.com/dcanar9/Coursework/blob/master/images/Card%20Game.png?raw=true" width="300" height="300">

The Card Class is a set of header files to simulate a game of war between to people. The project was to 
demonstrate the use of polymorphism within C++. The class begins at the most basic structure the card, then the 
deck, and finally the hand of the player. The loom video going through code is posted in two parts [here](https://www.useloom.com/share/7d04375206d34c34a146c9d2070cea8e). and [here]( https://www.useloom.com/share/7d04375206d34c34a146c9d2070cea8e)

---
<br/>

# [Truth Table Generator in Python](https://github.com/dcanar9/Coursework/tree/master/Truth%20Table)
![Truth Table Example](/images/TruthTable.png)

The Python is all in one file, there are a group of methods that are used to identify the variables, create combinations,
and then finally simplify the actual logical expression. The calculator will take a logical expression (with each symbol seperated
by a space) then give all possibilities using the truth table. 

---
<br/>

# [Self Solving Maze in C++](https://github.com/dcanar9/Coursework/tree/master/Self%20Solving%20Maze)

<img src="https://github.com/dcanar9/Coursework/blob/master/images/MazeOutput.png?raw=true" width="300" height="450">

The maze works by taking in input from a text file to create the maze. The input is a list of coordinates that state the maze size, starting block, 
finish block, and walls. The maze object is built from the vertices and edges class that then make up the graph class. The maze starts by reading this graph and creating a representation for it with a 2 dimenstional array. The maze is then handed to a maze solver class that uses Dijktras algorithm to find the shortest path to the finish. In the example above, it can be seen the maze starts in the top left corner at the S then teh directional v's show the path to the finish block F.

_This code was made a while ago without proper knowledge of code organization so sorry for the classes all in one cpp file_

---
<br/>

# [Azure Hosted Jokes Web Application](https://github.com/dcanar9/Coursework/tree/master/Azure%20Jokes%20Web%20App)

<img src="https://github.com/dcanar9/Coursework/blob/master/images/JokesLoginScreen.png?raw=true" width="450" height="250">
<img src="https://github.com/dcanar9/Coursework/blob/master/images/JokesSearchScreen.png?raw=true" width="450" height="250">

This was an on-going project for a class in 2020. The jokes application is a set of pages that can be used to search jokes, login to the system, add jokes, and view results. The last modification to this project was to host it to a cloud service, Azure. I wish I could've provided a link but the time for the Azure trial expired. To view how the web app worked while it was live please view the following [video](https://www.loom.com/share/1237e5f329cf4c7498de3405cbf56983?sharedAppSource=personal_library) . To try to run the web app on your own/ on a localhost, please use the soucre code provided by the link in the title. All the files are in the jokes app folder, but I removed the sql file with the db and the vendor folder that Google's OAuth2 uses. Therefore to use the project download OAuth2 from the git https://github.com/googleapis/google-api-php-client, this git google recommends if the backend uses PHP. 

---

# [GCU-PL Compiler Written in C](https://github.com/dcanar9/Coursework/tree/master/Project%205%20Compiler)

This was a semester long project that I worked on to compile code written in GCU-PL, a grammar given by the course. To see the grammar there is a word doc with the [rules](https://github.com/dcanar9/Coursework/blob/master/Project%205%20Compiler/C--Grammar.docx) , but in the code I use single characters for each left hand side of a rule ([excel sheet](https://github.com/dcanar9/Coursework/blob/master/Project%205%20Compiler/C--AbbreviatedGrammar.xlsx)). The compiler has many componenets to it, each step of the compiler process has its own header file and some have accompanying documentation. Unfortunately the Compiler is not completley finished, the compiler is able to produce some MIPS code but not all. Additionally if you would like to try it with your own grammar you could edit the [syntax analyzer](https://github.com/dcanar9/Coursework/blob/master/Project%205%20Compiler/SyntaxAnalyzer.h) initialize method to use different rules, this should work with proper LL1 grammar and if your grammar needs some fixes there are methods in there to apply those fixes to the firsts and follows sets of the grammar. 

---
<br/>

# [OpenGL: Fractal Forest](https://github.com/dcanar9/Coursework/tree/master/OpenGL_Fractal_Forest)

<img src="https://github.com/dcanar9/Coursework/blob/master/images/FractalForest1.png?raw=true" width="450" height="250">
<img src="https://github.com/dcanar9/Coursework/blob/master/images/FractalForest2.png?raw=true" width="450" height="250">

This project was a more difficult one created for a Computer Graphics class. The projects objective was to create realistic trees from L-systems in C++. L-Systems are a rewriting system where a string and production rules are used to create larger strings. These strings are comprised of a recursive pattern that is then used to construct shapes that look similar to models in nature (trees, weeds, plants). 

---
<br/>

# On-Going Project: Python Load Board Web Application Powered by Selenium 

This is what I am working on for my Capstone project, it is a web application that will assist my mom looking for loads as a truck dispatcher. The web application will launch a browser that will be controlled by Selenium to automate the various loadboard searches and gather their results to display on its own GUI. I cant link any code right now becasue its still in progress, but once its done I might. 

[Here](https://padlet.com/isac_artzi/62exyzmr15qznyc3) is a more in depth page of the planning and a sample of the working web app.


<br/><br/><br/><br/><br/>
...
**MORE WORK TO BE POSTED SOON**
