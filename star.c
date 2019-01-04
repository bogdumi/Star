#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <assert.h>

// Structs and global variables -------------------------------------
struct Node{
   int x, y;
   void *parent;
   char c;
   char dirs;
   int f,g,h;
   int originX, originY;
};
typedef struct Node Node;

Node *nodes, *sentinel;
int width, height, goalX, goalY;
int dx[4] = {0, 1, 0, -1};
int dy[4] = {1, 0, -1, 0};

int getIndex(int i, int j){
   return j + i * width;
}

// A* functions -----------------------------------------------------
// Calculate heuristic cost
int calculateH(Node *n){
   int h = abs(n -> x - goalX) + abs(n -> y - goalY);
   return h;
}

// Calculate movement cost
int calculateG(Node *n){
   return 1;
}

// Calculate overall cost of movement
int calculateF(Node *n){
   int g = calculateG(n);
   int h = calculateH(n);
   return g + h;
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
   *len = *len + 1;
   return;
}

// Pop the last node
void pop(Node *list, int *len, int index){
   for(int i = index; i < (*len) - 1; i++)
      list[i] = list[i+1];
   *len = *len - 1;;
   return;
}

// Search the list for a node with a lower oveerall cost
int searchList(Node n, Node *list, int len){
   for(int i = 0; i < len; i++)
      if(n.x == list[i].x && n.y == list[i].y)
         if(n.f > list[i].f)
            return 1;
   return 0;
}

// Evaluate a successor
void evalSucc(Node *n, Node *q, Node *openList, Node *closedList, int *openListLen, int *closedListLen){
   if(n -> c != ' ' || n -> x == 0 || n -> y == 0)
      return;
   if(n -> x == goalX && n -> y == goalY){
      n -> originX = q -> x;
      n -> originY = q -> y;
      return;
   }
   n -> g = calculateG(n) + q -> g;
   n -> h = calculateH(n);
   n -> f = n -> g + n -> h;
   if(searchList(*n, openList, *openListLen))
      return;
   if(searchList(*n, closedList, *closedListLen))
      return;
   n -> originX = q -> x;
   n -> originY = q -> y;
   push(*n, openList, openListLen);
}

// Trace the path back to the entrance
void trace(Node *n){
   n -> c = 'o';
   if(n -> originX == 0 && n -> originY == 1)
      return;
   trace(&nodes[getIndex(n -> originX, n -> originY)]);
}

// A* algorithm
void startAStar(){
   Node openList[width * height];
   Node closedList[width * height];
   int openListLen = 0;
   int closedListLen = 0;
   nodes[getIndex(1,1)].f = 0;
   nodes[getIndex(1,1)].originX = 0;
   nodes[getIndex(1,1)].originY = 1;
   push(nodes[getIndex(1,1)], openList, &openListLen);
   while(openListLen){
      int qIndex = lowestF(openList, openListLen);
      Node q = openList[qIndex];
      pop(openList, &openListLen, qIndex);
      // Right
      evalSucc(&nodes[getIndex(q.x, q.y + 1)], &q, 
               openList, closedList, 
               &openListLen, &closedListLen);
      // Down
      evalSucc(&nodes[getIndex(q.x + 1, q.y)], &q, 
               openList, closedList, 
               &openListLen, &closedListLen);
      // Left
      evalSucc(&nodes[getIndex(q.x, q.y - 1)], &q, 
               openList, closedList, 
               &openListLen, &closedListLen);
      // Up
      evalSucc(&nodes[getIndex(q.x - 1, q.y)], &q, 
               openList, closedList, 
               &openListLen, &closedListLen);
      
      push(q, closedList, &closedListLen);
   }
   trace(&nodes[getIndex(goalX, goalY)]);
}

// Maze generation functions ----------------------------------------

// Set indices
void indices(){
   for(int i = 0; i < height; i++)
      for(int j = 0; j < width; j++){
         nodes[getIndex(i,j)].x = i;
         nodes[getIndex(i,j)].y = j;
      }
}

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
            n -> dirs = 15;
            n -> c = ' '; 
         }
         else 
            n -> c = '#';
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
            printf("oo");
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
   indices();
   startAStar();
   draw();
}

// Testing ----------------------------------------------------------

void testCalculateH(){
   Node *n = malloc(sizeof(Node));
   goalX = 0, goalY = 0;
   n -> x = 0, n -> y = 0;
   assert(calculateH(n) == 0);
   n -> x = 1, n -> y = 1;
   assert(calculateH(n) == 2);
   n -> x = -1, n -> y = -1;
   assert(calculateH(n) == 2);
   free(n);
}

void testCalculateG(){
   Node *n = malloc(sizeof(Node));
   assert(calculateG(n) == 1);
   free(n);
}

void testCalculateF(){
   Node *n = malloc(sizeof(Node));
   goalX = 0, goalY = 0;
   n -> x = 0, n -> y = 0;
   assert(calculateF(n) == 1);
   n -> x = 1, n -> y = 1;
   assert(calculateF(n) == 3);
   n -> x = -1, n -> y = -1;
   assert(calculateF(n) == 3);
}

void testLowestF(){
   Node n1, n2, nList[2];
   n1.f = 1;
   n2.f = 0;
   nList[0] = n1;
   nList[1] = n2;
   assert(lowestF(nList, 2) == 1);
   n1.f = 0;
   n2.f = 1;
   nList[0] = n1;
   nList[1] = n2;
   assert(lowestF(nList, 2) == 0);
}

void testPush(){
   Node n1, n2, nList[5];
   int nLen = 0;;
   n1.x = 3;
   push(n1, nList, &nLen);
   assert(nList[0].x == n1.x);
   assert(nLen == 1);
   n2.y = 5;
   push(n2, nList, &nLen);
   assert(nList[1].y == n2.y);
   assert(nLen == 2);
}

void testPop(){
   Node nList[5], n1, n2, n3;
   int nLen = 0;
   n1.x = 1;
   n2.x = 2;
   n3.x = 3;
   push(n1, nList, &nLen);
   push(n2, nList, &nLen);
   push(n3, nList, &nLen);
   pop(nList, &nLen, 2);
   assert(nLen == 2);
   assert(nList[0].x == 1);
   assert(nList[1].x == 2);

   pop(nList, &nLen, 0);
   assert(nLen == 1);
   assert(nList[0].x == 2);

   pop(nList, &nLen, 0);
   assert(nLen == 0);
}



void test(){
   testCalculateH();
   testCalculateG();
   testCalculateF();
   testLowestF();
   testPush();
   testPop();
   printf("Tests pass.\n");
}

// Main function ----------------------------------------------------
int main(int argc, char **argv) {
   if(argc == 1)
      test();
   if(argc > 3 || argc == 2)
      printf("Use: ./star <width> <height>, where <width> and <height> are positive odd integers.\n");
   if(argc == 3){
      width = atoi(argv[1]);
      height = atoi(argv[2]);
      goalX = height - 2;
      goalY = width - 2;

      if (width % 2 != 1 || height % 2 != 1 || width <= 0 || height <= 0)
         printf("Use: ./star <width> <height>, where <width> and <height> are positive odd integers.\n");
      start();   
   }
}
