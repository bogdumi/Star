#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

// Structs and global variables -------------------------------------
struct Node{
   int x, y;
   void *parent;
   char c;
   char dirs;
   int f,g,h;
};
typedef struct Node Node;

Node *nodes;
int width, height;

int getIndex(int i, int j){
   return j + i * width;
}

// A* functions -----------------------------------------------------
// Calculate heuristic cost
void calculateH(Node *n){
   int goalX = height - 2;
   int goalY = width - 1;
   n -> h = abs(n -> x - goalX) + abs(n -> y - goalY);
   return;
}

// Calculate movement cost
void calculateG(Node *n){
   n -> g = 1;
   return;
}

// Calculate overall cost of movement
void calculateF(Node *n){
   calculateG(n);
   calculateH(n);
   n -> f = n -> g + n -> h;
   return;
}

// Find the lowest cost of movement
int lowestF(Node *list, int len){
   int min = 0;
   for(int i = 0; i < len; i++)
      if(list[i].f < list[min].f)
         min = i;
   return min;
}

// Push a node onto a list
void push(Node n, Node *list, int *len){
   list[*len] = n;
   len++;
}

// Pop the last node
void pop(Node n, Node *list, int *len){
   
}

// A* algorithm
void startAStar(){
   Node openList[width + height];
   Node closedList[width + height];
   int openListLen = 0;
   int closedListLen = 0;
   nodes[getIndex(1,1)].f = 0;
   push(nodes[getIndex(1,1)], openList, &openListLen);
   while(openListLen){
      Node q = openList[lowestF(openList, openListLen)];
      
   }
}

// Maze generation functions ----------------------------------------
// Initiate the maze 
void init() {
   int i, j;
   Node *n;
   nodes = calloc(width*height, sizeof(Node));
   for (i = 0; i < width; i++){
      for (j = 0; j < height; j++){
         n = nodes + i + j * width;
         if (i * j % 2) {
            n -> x = i;
            n -> y = j;
            n -> dirs = 15; //Assume that all directions can be explored (4 youngest bits set)
            n -> c = ' '; 
         }
         else 
            n -> c = '#'; //Add walls between nodes
      }
   }
   return;
}

// Link a node
Node *link(Node *n) {
   int x, y;
   char dir;
   Node *dest;
   if (n == NULL) 
      return NULL;
   while (n -> dirs) {
      dir = (1 << (rand() % 4));
      if (~n->dirs & dir) 
         continue;
      n->dirs &= ~dir;
      switch (dir){
         //Right
         case 1:
            if (n -> x + 2 < width){
               x = n -> x + 2;
               y = n -> y;
            }
            else continue;
            break;
         
         //Down
         case 2:
            if (n -> y + 2 < height){
               x = n -> x;
               y = n -> y + 2;
            }
            else continue;
            break;
         //Left
         case 4:
            if (n -> x - 2 >= 0){
               x = n -> x - 2;
               y = n -> y;
            }
            else continue;
            break;
         
         //Up
         case 8:
            if (n -> y - 2 >= 0){
               x = n -> x;
               y = n -> y - 2;
            }
            else continue;
            break;
      }
      dest = nodes + x + y * width;
      if (dest ->c == ' ') {
         if (dest -> parent != NULL) 
            continue;
         dest -> parent = n;
         nodes[n -> x + (x - n -> x) / 2 + (n -> y + (y - n -> y) / 2) * width].c = ' ';
         return dest;
      }
   }
   return n -> parent;
}

// Draw the maze to the console
void draw(){
   int i, j;
   for (i = 0; i < height; i++) {
      for (j = 0; j < width; j++) {
         if((i == height - 2 && j == width - 1) || (i == 0 && j == 1)){
            printf("  ");
            continue;
         }
         printf("%c%c", nodes[j + i * width].c, nodes[j + i * width].c);
      }
      printf("\n");
   }
}

// Start the maze generation
void start(){
   Node *start, *last;
   srand(time(NULL));
   init();
   start = nodes + 1 + width;
   start->parent = start;
   last = start;
   while ((last = link(last)) != start);
   draw();
}

// Main function ----------------------------------------------------
int main(int argc, char **argv) {
   if (argc != 3)
   {
      printf("Use: ./star <width> <height>, where <width> and <height> are positive odd integers.\n");
   }
   
   width = atoi(argv[1]);
   height = atoi(argv[2]);

   if (width % 2 != 1 || height % 2 != 1 || width <= 0 || height <= 0)
      printf("Use: ./star <width> <height>, where <width> and <height> are positive odd integers.\n");
   start();   
}
