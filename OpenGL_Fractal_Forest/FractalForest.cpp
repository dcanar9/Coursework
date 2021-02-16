/*
Dennis Canar
6 - 15 - 19
CST - 310 
Ricardo Citro
*/

//Standard OpenGL
#include <GL/glut.h>
#include<GL/glu.h>

//To use strings
#include<string>

//To read from files
#include<fstream>

//To Print
#include<iostream>

//To use Sin and Cos
#include<math.h>

//to use vectors
#include<vector>

//To use blank space remover
#include<algorithm>

//For stochastic system random number generation 
#include <stdlib.h>     
#include <time.h>       

//Use when drawing cylinders
#define PI 3.14159265359

using namespace std;

struct TurtData{        //To save turtle data when pushMatrix is called 
   float x;
   float y;
   float z;
   float deg;
   float theta;
};

struct treeData{        //To load individual tree settings 
   float len;
   float deg;
   float maxDist;
   float maxRad;
};
                           //Holds data for sting replacement 
vector<string> lookFor;    //Will hold characters to look for
vector<string> changeTo;   //Will hold string to make replacement
vector<float> stochVals;   //To hold probabilities if used

vector <treeData> trees;   //Holds each trees variables (length, radius, degree)

//Information is pulled from .txt and used in replacement function & main
int iter; 
string start;


//Here are all the files that i use to create the objects in my scene
vector<string> files{"lsys1.txt", "lsys3.txt", "lsys4.txt", "lsys2.txt" 
,"lsys_stochastic.txt", "lsys5.txt", "lsys6.txt", "lsys_stochastic2.txt"
, "lsys_stochastic3.txt", "lsys7.txt"};

vector<string> lSys;

float treePos[10][3] = { {200.0, -150.0, -550.0}, {150.0, -150.0, -550.0}
,{450.0, -150.0, -550.0}, {50.0, -150.0, -550.0}, {85.0, -150.0, -650.0}
,{280.0, -150.0, -750.0}, {-85.0, -150.0, -550.0}, {310.0, -150.0, -550.0}
,{360.0, -150.0, -550.0}, {-150.0, -150.0, -550.0}
};


////////////////////////////////////////////////////////////////////////////////

                              /*Camera Variables*/

// angle of rotation for the camera direction
float angle=0.0;
// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f;
// XZ position of the camera
//float x=-20,z=44.6f, height=33.2;
float x=-30.0f,z=2.79259f, height=2.62;

////////////////////////////////////////////////////////////////////////////////

                              /*openGL settings */

//Applying settings to my scene
void init(){

   glClearColor(0.74, 0.84, 1.0, 1.0);   //Setting background color to light blue
   glEnable(GL_DEPTH_TEST);               //For 3D objects
   glMatrixMode(GL_MODELVIEW);      //Tells openGL to maniupulate actual object matrix
}



//This is a function that will run automatically when the window is resized
void reshape(GLsizei width, GLsizei height){
      // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
 
   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);
 
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();            
   // Enable perspective projection with fovy, aspect, zNear and zFar
   gluPerspective(45.0f, aspect, 0.1f, 1000.0f);
}

////////////////////////////////////////////////////////////////////////////////

                           /*User Functions*/

//Function to end program and move camer height
void exitKey(unsigned char key, int x2, int y){
   if (key == 27){
       exit(0);                //to end program 
   }
   
   else if (key == 'q'){       //to raise height of camer
       height += 0.1;
   }
  
   else if (key == 'e'){       //to lower height of camera
       height -= 0.1;
   }
   
}
//For camera movement using arrow keys
void processSpecialKeys(int key, int xx, int yy) {

	float fraction = 0.1f;     //Speed of camera movement

	switch (key) {              //left and right move angle of camera
		case GLUT_KEY_LEFT :
			angle -= 0.01f;
			lx = sin(angle);
			lz = -cos(angle);
			break;
		case GLUT_KEY_RIGHT :
			angle += 0.01f;
			lx = sin(angle);
			lz = -cos(angle);
			break;
		case GLUT_KEY_UP :          //up and down will move the camera back and forth 
			x += lx * fraction;
			z += lz * fraction;
			break;
		case GLUT_KEY_DOWN :
			x -= lx * fraction;
			z -= lz * fraction;
			break;
	}
}


////////////////////////////////////////////////////////////////////////////////

//Will Draw a cylinder given radius of each end, slices increment, and height of tube
void drawCylinder(float increment, float height, float smallRad, float bigRad){
  
   float x, y;    //To store values
  
   glColor3f(0.48, 0.25, 0.2);   //Setting Cylinder color to brown
   
   //Drawing Bottom Surface
   glBegin(GL_TRIANGLE_FAN);  
   glVertex3f(0.0, 0.0, 0.0);       //Set origin
   
   for (float i = 0; i < 360; i+=increment){     //Finds x & y given degree
      x = bigRad * cos(i * PI /180);             //will place points at radius length 
      y = bigRad * sin(i * PI /180);
      glVertex3f(x, y, 0.0);                    //Create Point
   }
   glEnd();
   
   //Drawing Top Surface
   glBegin(GL_TRIANGLE_FAN);  
   glVertex3f(0.0, 0.0, 0.0);                   //Set origin
   
   for (float i = 0; i < 360; i+=increment){
      x = smallRad * cos(i * PI /180);          //use math to find point at angle 
      y = smallRad * sin(i * PI /180);
      glVertex3f(x, y, height);                 //Create Point
   }
   glEnd();   
   
   //Tube will make small rectangles from surface to surface to create tube
   glBegin(GL_QUAD_STRIP);     
   for (float i = 0; i < 360; i+=increment){
      x = bigRad * cos(i * PI /180);             //use math to find point at angle 
      y = bigRad * sin(i * PI /180);      
      glVertex3f(x, y, 0.0);
      
      x = smallRad * cos(i * PI /180);             //use math to find point at angle 
      y = smallRad * sin(i * PI /180);    
      glVertex3f(x, y, height);           //Create Points
   }
   glEnd();        
   
}

////////////////////////////////////////////////////////////////////////////////

       /*=====All the turtle functions requested by the word doc======*/
       /*=============And functions used to draw trees============*/

class Turtle{
   
   //Holds current position and angle of turtle & old positions pushed onto stack  
   private:
   float xPos = 0, yPos = 0, zPos = 0, zAngle = 0;
   float xAngle = (90 * PI) / 180;    
   vector<TurtData> stack;       //used for turtle to return to old state
   treeData T;             //Is used so turtle knows how far to go and turn
   
   public:
   
   //Will return turtle to the bottom of the tree
   void clearTurtle(){
      //only clear these vars becasue others can be over written
      xPos = 0, yPos = 0, zPos = 0, zAngle = 0;
      float xAngle = (90 * PI) / 180;
      stack.clear();
   }
   
   //Use trig to get position of the turtle after a translation
   void updateCoords(){
      xPos += T.len * cos(xAngle);
      yPos += T.len * sin(xAngle);      
      zPos += T.len * sin(zAngle);
   }
   
   pair<float, float> genRadius(){
   
      //used to return to values to other function
      pair<float, float> radius;    
      
      //Distance formula to know how far a point is from the origin
      float dist = T.maxDist - sqrt(pow(xPos, 2) + pow(yPos, 2) + pow(zPos, 2));
      
      //Get bottom surface radius using scaling formula then move and find 
      //The top surfaces radius
      radius.first = ((dist / T.maxDist) * (T.maxRad - 0.3)) + 0.1;
      updateCoords();
      radius.second = ((dist / T.maxDist) * (T.maxRad - 0.3)) + 0.1;

      return radius;
   
   }
   
   void moveDraw(){
      drawLeaves();  //Draws leaves before branch is drawn to use current pos 
      pair<float, float>rad = genRadius();   //Generate radius size for cylinder
      glTranslatef(0.0, 0.0, -(T.len));      //Move 
      drawCylinder(0.5, T.len, rad.second, rad.first);   //Draw cylinder toward old pos
   }

   void move(){
      glTranslatef(0.0, 0.0, -(T.len));   //move
      updateCoords();      //update turtles position
   }

   void posXRot(){
      glRotatef(T.deg, 1.0, 0.0, 0.0);    //rotate
      xAngle += (T.deg*PI)/180;           //update turtle variable
   }

   void negXRot(){
      glRotatef(-(T.deg), 1.0, 0.0, 0.0);    //The rest below is the same concept
      xAngle -= (T.deg*PI)/180;
   }

   void posYRot(){
      glRotatef(T.deg, 0.0, 1.0, 0.0);      
      zAngle += (T.deg*PI)/180;
   }

   void negYRot(){
      glRotatef(-(T.deg), 0.0, 1.0, 0.0); 
      zAngle -= (T.deg*PI)/180;     
   }

   void posZRot(){
      glRotatef(T.deg, 0.0, 0.0, 1.0);
   }

   void negZRot(){
      glRotatef(-(T.deg), 0.0, 0.0, 1.0);
   }

   void turnAround(){
      glRotatef(180.0, 1.0, 0.0, 0.0);
      xAngle += 180.0;
   }

   void push(){
      glPushMatrix(); //Actually push to openGL stack
      
      TurtData point;      //Data structure at top of file
      
      point.x = xPos;      //Set variables data to current position of turtle
      point.y = yPos;
      point.z = zPos;
      point.deg = xAngle;
      point.theta = zAngle;
      
      stack.push_back(point); //Save turtles pos and angle vector stack
   }

   void pop(){
      glPopMatrix();    //Pop from openGL stack
      
      int i = stack.size()-1;     //Get position of last pushed data 
      
      xPos = stack[i].x;         //make saved data current turtle data
      yPos = stack[i].y;
      zPos = stack[i].z;
      xAngle = stack[i].deg;
      zAngle = stack[i].theta;
     
      stack.pop_back();          //remove data just used
      stack.shrink_to_fit();
      
   }
   
   void drawLeaves(){
   
      glPushMatrix();      //To apply transformation to the leaf and not the tree
      
      glColor3f(0.0, 1.0, 0.0);  //Setting color to green

      
      float leftY = 0, rightY, j, pos, rot, Lx, Ly, Lz;     //leafs variables
      int L = round(T.len);
      pos = (rand() % L  +1) / 100.0;    //Getting random position on branch
      rot = rand() % 360 + 1;       //Random orientation for leaf
         
      Lx = (pos * cos(xAngle)) + xPos;    //To calculate distace leaf is from center
      Ly = (pos * sin(xAngle)) + yPos;
      Lz = (pos * sin(zAngle)) + zPos;
      
      //Get distance and calculate radius of cylinder at that position 
      float dist = T.maxDist - sqrt(pow(Lx, 2) + pow(Ly, 2) + pow(Lz, 2));
      float offset = (dist / T.maxDist) * (T.maxRad - 0.1) + 0.1;
      
      //Move leaf so its at the edge of the branch, pos,  & rotate
      glTranslatef(offset, 0.0, pos);
      glRotatef(rot, 0.0, 0.0, 1.0);      
      
      glBegin(GL_TRIANGLE_STRIP);
         
         glVertex3f(0.0, 0.0, 0.0);    //Start at origin
         
         for(float i = 0; i < 1.5; i += 0.01){     //Loop until lenght has been reached            
            if(i <= (1.5 / 2)){              //Drawing bottom portion of leaf
               leftY = 5 * pow(i, 2); 
               rightY = 5 * pow(i, 2); 
               j = i;                   // Drawn using parabolic function 
            }
            
            else if(i > (1.5/2)){          //Drawing top portion 
               leftY = -5 * pow(j, 2) + 2*(5 * pow(1.5/2, 2)); 
               rightY = -5 * pow(j, 2) + 2*(5 * pow(1.5/2, 2)); 
               j -= 0.01 ;
               
            }          
            
            glVertex3f(-j, leftY, 0.0);   //Draw point at left and right of leaf
            glVertex3f(j, rightY, 0.0);                      
         }         
      glEnd();         
      glPopMatrix();              //go back to branch position 
   }
   
   void loadData(treeData Tree){    //For turtle to draw multiple trees
      T = Tree;
   }
   
};

////////////////////////////////////////////////////////////////////////////////

//Function that looks through a string and compares to rules, then replaces
string swap(string s, bool stoch){

   string newString;    //Empty new string

   for(int i = 0; i < s.length(); i++){      //iterate through the string
     bool replaced = false;
   
      for(int j = 0; j < lookFor.size(); j++){ //Compare char to rule

         if(lookFor[j][0] == s[i]){    //If we find the char 
            
            if (stoch){    //If file type is stochastic
               float sVal = (rand() % 100 + 1) / 100.0; //random number betwen 1 and 100
               
               bool range = true;
               replaced = true;
               int index = 0;
               
               //Running until the range it belongs to is found
               while(range){                  
                  if(sVal <= stochVals[index]){     //iterate through probabilitiy vals
                     newString.append(changeTo[index]);
                     range = false;
                  }
                  index+=1;   //increment counter to iterate through vector
               }
               
            }         
            if(!stoch){    //if match found but not stochastic
               newString.append(changeTo[j]);    //Add phrase to new string in position of char  
               replaced = true;             
            }          
         }
      }
         
   if (replaced == false){    //if character is not one were looking for
      newString.push_back(s[i]);    //Add normal character to new string
   }
      
      
   }
   
   return newString;       //return the string with replacements
   

}
////////////////////////////////////////////////////////////////////////////////

                       /*=======Drawing Scene========*/

void renderScene(void) {

    srand(1998);     //Seed to give same random numbers in order 
    
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear buffers
   glMatrixMode(GL_MODELVIEW);    //changes matrix so we can edit the shapes made
   glLoadIdentity();             //ensure matrix is clear
   
   Turtle turtle;          //Turtle to draw trees
   
   gluLookAt(x, height, z, x+lx,  height,  z+lz, 0.0f, 1.0f, 0.0f);   //camera
   
   glPushMatrix();
   
   //Move scene into frame
   glTranslatef(-200.0, 0.0, 0.0); 
   
   glColor3f(0.18, 0.3, 0.18);         //Set color to dark green
   
   //Drawing ground
   glBegin(GL_QUADS);
      glVertex3f(-600.0, -150.0, 0.0);
      glVertex3f(-600.0, -150.0, -1000.0);
      glVertex3f(900.0, -150.0, -1000.0);
      glVertex3f(900.0, -150.0, 0.0);
   glEnd();
     
   for( int i = 0 ; i < lSys.size(); i++){      //Iterate through tree strings
   
         turtle.clearTurtle();         //reset turtle and load data needed
         turtle.loadData(trees[i]);
         
         glPushMatrix();            //To apply translation to individual trees         
         glTranslatef(treePos[i][0], treePos[i][1], treePos[i][2]); //custom position
         glRotatef(90.0, 0.0, 1.0, 0.0);     //Making object face camera
         glRotatef(90.0, 1.0, 0.0, 0.0);      //Making object stand up     
              
      for(int j = 0; j < lSys[i].length(); j++){   //Iterate through strings characters
         
         //If a certain character is found then perform a function
         //The grammar used is the same from the resource guide of project 6
         
         if(lSys[i][j] == 'F'){
            turtle.moveDraw();        
         }
         
         else if(lSys[i][j] == 'f'){
            turtle.move();
         }
         
         else if(lSys[i][j] == '+'){
            turtle.posXRot();
         }
         
         else if(lSys[i][j] == '-'){
            turtle.negXRot();
         }
         
         else if(lSys[i][j] == '['){
            turtle.push();
         }
         
         else if(lSys[i][j] == ']'){
            turtle.pop();
         }
         
         else if(lSys[i][j] == '&'){
            turtle.posYRot();
         }
         
         else if(lSys[i][j] == '^'){
            turtle.negYRot();
         }
         
         else if(lSys[i][j] == '\\'){
            turtle.posZRot();
         }
         
         else if(lSys[i][j] == '/'){
            turtle.negZRot();
         }
         
         else if(lSys[i][j] == '|'){
            turtle.turnAround();
         }         
         
      }
      
      glPopMatrix();    //pop individual tree matrix
   }
      
   glPopMatrix();    //To get back to original matrix
   
   glutSwapBuffers();         //Makes objects appear, similar to glFlush()
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv) {
      
   //To store the values each tree uses (angle, length, radius....)   
   treeData tree;

   for(int i = 0; i < files.size(); i++){
   
      lookFor.clear();     //So prevoius replacement rules do not apply to 
      changeTo.clear();    //new l-system drawing
      stochVals.clear();

      //will be used to get stochastic probability values 
      bool stochastic = false;
      
      //Openning the text file
      fstream myFile;
      myFile.open(files[i]);    //Open files in order of vector position 

      string line;         //To read file line by line
      int lineCount =1;    //To know line number in file
      
      
      //Extracts information from file 
      while(getline(myFile, line)){
         
         //Removes all white space of string pulled in
         line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
         
         //Getting variables accroding to line number 
         if (line == "Stochastic" || line == "stochastic"){stochastic = true; continue;}
         
         if (lineCount == 1){      //get variables by knowing line num
            tree.len = stof(line.substr(4,(line.length()) - 4), nullptr);   
         }
         
         else if (lineCount == 2){        //getting degree
            tree.deg = stof(line.substr(4, (line.length()) - 4), nullptr);
         }

         else if (lineCount == 3){        //Getting iteration string
            iter = stoi(line.substr(5, (line.length()) - 5), nullptr);
         }  
         
         else if (lineCount == 4){
            tree.maxDist = stof(line.substr(8, (line.length()) - 8), nullptr);
         }
         
         else if (lineCount == 5){        //Getting iteration string
            tree.maxRad = stof(line.substr(7, (line.length()) - 7), nullptr);
         }
                
         else if (lineCount == 6){        //Getting initial string
            start = line;     
         }
         
         else{  //Getting rules
            
            //If file is not a stochastic l-system 
            if(!stochastic){
            
               //Finding pos of arrow in string to seperate character and replacement
               for(int i = 0; i < line.length(); i++){
                  
                  if (line[i] == '-' && line[i+1] == '>'){     //Once the arrow is found
                     lookFor.push_back(line.substr(0,i));     //Left of arrow
                     changeTo.push_back(line.substr(i+2, line.length()-i)); //right
                  }
               }
            }
            
            //Will not look for arrow becasue file is in stochastic format
            else if (stochastic){
               lookFor.push_back(line.substr(0, 1));   //get character             
               stochVals.push_back(stof(line.substr(2, 4)));   //probability
               changeTo.push_back(line.substr(8, line.length()- 8)); //replacement
            }
         }
         
         //To keep track of lines increment counter
         lineCount++;
      }
      
      
               /*======Doing recursive replacements in string=====*/

      //Will do replacements iter many time (comes from .txt file)
      for(int step = 0; step < iter; step++){  
         
         //create range for each probability if stochastic (e.g. 33 -> 66 -> 100)
         if(step == 0 && stochastic){
            for(int i = 1; i < stochVals.size(); i++){
               stochVals[i]+=stochVals[i-1];               
            } 
         }     
         
         //string = string after one iteration of replacements
         start = swap(start, stochastic);
         
       }
       
       lSys.push_back(start);    //Tree Drawing instructions stored
       trees.push_back(tree);    //Store trees settins
       
}       
   
   
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGBA);
   glutInitWindowPosition(100,100);
   glutInitWindowSize(1700,1000);
   glutCreateWindow("Fractal Forest");
   glutDisplayFunc(renderScene);
   glutReshapeFunc(reshape);
   glutIdleFunc(renderScene);
   glutKeyboardFunc(exitKey);
   glutSpecialFunc(processSpecialKeys);
   init();        //calling my own initialization function 
   glutMainLoop();
   
   return 0;
}
