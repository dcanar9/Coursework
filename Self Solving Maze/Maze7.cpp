//Dennis Canar
//CST - 201
//Maze Project 7
//4 - 29 - 18
//This is my own work

//There are multiple methods that I have my first methods are in the graph 
//class where I use a type of find to find by xy coord or char and there
//are to that return a index and a pointer

//In my maze solver class I have a method to that checks if a rooom is 
//present within the to be checked container. The to be checked container
//is made in the constuctor by pushing on rooms that are not walls. Then
//I have another method for the algorithm to find the vertex with the
//least distance and then delete it from the container. Another method 
//within my maze solver is a method to add adjacent rooms to a vector
//only if they are not a wall or null, this vector then gets reset for 
//every vertex

//finally within my main I use all those functions to run dijkstra's alg
//and when there is a vertex with higher distance the predecessor is 
//changed to the coordinates of the vertex with smaller distance.

#include<iostream>
#include<fstream>
#include<string>
#include<vector>

class MazePrinter;
class MazeSolver;

using namespace std;

///////////////////////////////////////////////////////////////////////////////
class Vertex{

    public:
    char what;
    int xCord;
    int yCord;
    int dist;
    //rather than storing an ID number I will store the coordinates of the 
    //predecessor because I am using pointer and I dont want to use ID #'s
    pair<int,int> pred;
    
    //Constructor for my vertex class
    Vertex(char c, int x, int y, int d){
       what = c;
       xCord = x;
       yCord = y;
       dist = d;
       pred = pair<int,int>(-1,-1);
    }
    
    //To change what exists in that space
    void setWhat(char c){
        what = c;
    }
    
    ~Vertex(){
      delete this;
    }

};


///////////////////////////////////////////////////////////////////////////////


class Edge{
    public:
    Vertex* v1;
    Vertex* v2;
    int dist = 1;
    
    //Edge will contain two pointers to two vertices
    //Distance will be one because our edges will not be greater than one 
    Edge(Vertex* a, Vertex* b){
        v1 = a;
        v2 = b;
    }
    
};

///////////////////////////////////////////////////////////////////////////////

class Graph{

    //Container of pointers to vertices and container of pointers to edges
    public:
    vector<Vertex*> vertices;
    vector<Edge*> edges;
    
    //Creates a vertex and its pointer then pushes the pointer on the 
    //Vertex container
    void createV(char c, int x, int y){
        vertices.push_back(new Vertex(c, x, y, 100));
    }
    
    //Returns sie of vertex container 
    int sizeV(){
        return vertices.size();
    }
    
    //Will return the index of a vertex at a given (x, y) coordinate
    int find(int x, int y){
      for (int i = 0; i < vertices.size(); i++){
          if (vertices[i]->xCord == x && vertices[i]->yCord == y){
              return i;
          }
      }
    }
    
    Vertex* pointTo(int x, int y){
      for (int i = 0; i < vertices.size(); i++){
          if (vertices[i]->xCord == x && vertices[i]->yCord == y){
              return vertices[i];
          }
      }
      
      return NULL;
    }
    
    Vertex* findChar(char c){
        for (int i = 0; i < vertices.size();i++){
            if (vertices[i]->what == c){
               return vertices[i];
            }
        }
    }
    
};

///////////////////////////////////////////////////////////////////////////////

class Maze{
    private:
    int xDim;
    int yDim;
    friend MazePrinter;
    friend MazeSolver;
    Graph rooms;
    
    public:

    //Constructor for maze
    Maze(int x, int y){
        xDim = x;
        yDim = y;
        //Creates the correct amount of vertices
        for (int y = 0; y < yDim; y++){
            for (int x = 0; x < xDim; x++){
                //will create vertex and set as empty space
                rooms.createV('.', x, y);
            }
        }
    }
    
    
    //Will change character of correct room
    void setStartFinish(int sx, int sy, int fx, int fy){
        //Will iterate through all vertices
        for (int i = 0; i < rooms.sizeV(); i++){
            //if correct (x,y) coord. character will change to 'S'
            if (rooms.vertices[i]->xCord == sx){
                if (rooms.vertices[i]->yCord == sy){
                    rooms.vertices[i]->setWhat('S');
                }  
            }
            //If correct (x,y) will change to 'F'
            if (rooms.vertices[i]->xCord == fx){
                if (rooms.vertices[i]->yCord == fy){
                    rooms.vertices[i]->setWhat('F');
                }  
            }
        }
    
    }
    
    //Will change character of vertex to X
    void setWall(int wx, int wy){
        for (int i = 0; i < rooms.sizeV(); i++){
            //If the correct (x,y) found, character will change
            if (rooms.vertices[i]->xCord == wx){
                if (rooms.vertices[i]->yCord == wy){
                    rooms.vertices[i]->setWhat('X');
                }  
            }
        }
    }
    
    //For my own use will print sample of maze
    void print1x1(){
        for (int i = 0; i < rooms.sizeV(); i++){
            cout << rooms.vertices[i]->what;
            if (rooms.vertices[i]->xCord == xDim-1){
                cout << endl;
            }        
        }
    }
    
};

///////////////////////////////////////////////////////////////////////////////

class MazePrinter{
    public:
    MazePrinter(Maze* m) : m(m) {}
    
        //Will print row asked for
    void printRow(int y, ostream& os){
        //will print for a row 3 times
        for (int line = 1; line <= 3; line++){
            //here to reset my index to the beginning of the row
            int j = m->rooms.find(0,y);
            //prints out y coordinate axis
            if (line == 2){
                if (y < 10){
                    os << y << " ";
                }
                else if (y >= 10){
                    os << y;
                }
            }
            else{
                os << "  ";
            }
            //Will print out correct output for each bock
            for (int i = 0; i < m->xDim; i++){
                if (m->rooms.vertices[j]->what == 'S' && line == 2){
                    os << ".S.";
                }
                else if (m->rooms.vertices[j]->what == 'F' && line == 2){
                    os << ".F.";
                }
                else if (m->rooms.vertices[j]->what == 'X'){
                    os << "XXX";
                }
                else if (m->rooms.vertices[j]->what == '<'){
                    os << "<<<";
                }
                else if(m->rooms.vertices[j]->what == '>'){
                    os << ">>>";
                }
                else if(m->rooms.vertices[j]->what == '^'){
                    os << "^^^";
                }
                else if(m->rooms.vertices[j]->what == 'v'){
                    os << "vvv";
                }
                else{
                    os << "...";
                }
                j++;
            }
            
            //ends row
            os << endl;
        }
        
        
        
    }
    
    void print(ostream &os){
        //Will print out X axis
        os << "  "; //Takes care of extra space from y axis
        for (int xNum = 0; xNum < m->xDim; xNum++){
            os << " " << xNum << " ";
        } 
        os << endl;
        //Prints out every row in the Maze
        for (int y = 0; y < m->yDim; y++){
            printRow(y,os);
        }
    
    }
    
    private:
    Maze* m;
    

};


///////////////////////////////////////////////////////////////////////////////        


class MazeSolver{
    private:
    Maze* m;
    Vertex* curr;
    vector<Vertex*> toCheck;
    vector<Vertex*> copy;
    
    public:
    MazeSolver(Maze* m) : m(m) {
        curr = getStart();
        curr->dist = 0;
        toCheck.push_back(curr);
        
        //Create to be checked container, make sure walls arent included
        for (int i = 0; i < m->rooms.vertices.size(); i++){
            if (m->rooms.vertices[i]->what != 'X'){
                toCheck.push_back(m->rooms.vertices[i]);
                copy.push_back(m->rooms.vertices[i]);
            }
        }
    };
    
    //Finds the coordinates of the start using graph function 
    Vertex* getStart(){
        Vertex* start;
        start = m->rooms.findChar('S');
        return start;
    }
    
    //will print what is in my to be checked vector for my own use
    void printTC(){
        for (int i = 0; i < toCheck.size(); i++){
            cout << toCheck[i]->xCord << " " << toCheck[i]->yCord << endl;
        }
    }
    
    
    //Will return true if the current position is at the finish
    bool isFinish(){
        return curr->what == 'F';
    }
    
    //returns true or false if a position has been visited before
    bool inTC(Vertex* n){
        for (int i = 0; i < toCheck.size()-1; i++){
            if (toCheck[i] == n){
                return true;
            }
        }
        return false;
    }
    
    //will change the pattern of the maze unless it reaches the finish
    //block 
    void showTrail(Vertex* v){
        int pos = toCheck.size()-2;
        //I want to change all rooms that arent my finish
        if (curr->what != 'F'){
            if (v->xCord > curr->xCord){
                curr->what = '<';
            }
            else if (v->yCord > curr->yCord){
                curr->what = '^';
            }
            else if (v->xCord < curr->xCord){
                curr->what = '>';
            }
            else if (v->yCord < curr->yCord){
                curr->what = 'v';
            }
        }
    }
    
    //will create a vector of rooms left right up down to the room given
    vector<Vertex*> getAdj(Vertex* v){
        
        vector<Vertex*> adj;
        
        Vertex* north = m->rooms.pointTo(v->xCord+1, v->yCord);
        Vertex* east = m->rooms.pointTo(v->xCord, v->yCord+1);
        Vertex* south = m->rooms.pointTo(v->xCord-1, v->yCord);
        Vertex* west = m->rooms.pointTo(v->xCord, v->yCord-1);
        
        if(north != NULL && north->what != 'X'){
            adj.push_back(north);
        }
        if(east != NULL && east->what != 'X'){
            adj.push_back(east);
        }
        if(south != NULL && south->what != 'X'){
            adj.push_back(south);
        }
        if(west != NULL && west->what != 'X'){
            adj.push_back(west);
        }
        
        return adj;
    }
    
    //will get me the vertex with least distance then delete it from the
    //to be checked container
    Vertex* findMin(){
        Vertex* min = toCheck[0];
        int pos = 0;
        
        for(int i = 0; i < toCheck.size(); i++){
            if (min->dist > toCheck[i]->dist){
                min = toCheck[i];
                pos = i;
            }       
        }
        
        toCheck.erase(toCheck.begin()+pos);
        return min;
    }
    
    //for loop in algorithm to tell me if the to be checked is empty
    bool isFin(){
        return toCheck.size() == 0;
    }
    
    //Will start from the end and follow the predecessors until the start
    //While following the predcessors it will change the maze characters
    void solve(){
        curr = m->rooms.findChar('F');
        while (curr->what != 'S'){
            Vertex* newCurr = m->rooms.pointTo(curr->pred.first, curr->pred.second);
            showTrail(newCurr);
            curr = newCurr;
        }
    
    }

};

///////////////////////////////////////////////////////////////////////////////
void clearScreen(){
    for (int i = 0; i < 100; i++){
        cout << "\n";
    }
}

int main(){

     
    //Declare all my values I will be using
    int width, height, startX, startY, finishX, finishY, wallX, wallY;
    char par, com, per; 

    //Opens input file for maze
    ifstream file;
    file.open("MazeInput.txt");

    //Will read the dimension of the maze
    file >> width >> com >> height >> per;

    //Create maze with the correct number of Nodes 
    Maze A(width, height);

    //Get our start point and finish point from the .txt file
    file >> par >> startX >> com >> startY >> par >> per;
    file >> par >> finishX >> com >> finishY >> par >> per;

    A.setStartFinish(startX, startY, finishX, finishY);

    //read and set all the walls, loop will stop when it finds the '.'
    char stop;
    while (stop != '.'){
        file >> par >> wallX >> com >> wallY >> par >> stop;
        A.setWall(wallX, wallY);
    }

    //My output file being set
    ofstream outFile;
    outFile.open("Maze7Output.txt");
    
    
    //Create printer object and call function to print to cout and .txt
    MazePrinter MazeP(&A);
    
    MazeP.print(cout);
    
    MazeSolver MazeS(&A);
    
    //Dijktras algorithm will set vertex with minimum distance in toCheck
    //Will pull out the adjacent rooms and compare their distances
    //wiil create new predecessor and new distance
    while(MazeS.isFin() == false){
    
        Vertex* v = MazeS.findMin();
        vector<Vertex*> adj = MazeS.getAdj(v);
        
        for (int i = 0; i < adj.size(); i++){
            if(adj[i]->dist > v->dist+1 && MazeS.inTC(adj[i])){
                adj[i]->dist = v->dist+1;
                adj[i]->pred = pair<int,int>(v->xCord, v->yCord);
            }
        }
        
    }
    
    cout << endl << endl;
    
    //solve will edit the maze so direction and the path is shown
    MazeS.solve();
    
    //prints to the console and output file
    MazeP.print(cout);
    MazeP.print(outFile);
    
    
    return 0;
}













