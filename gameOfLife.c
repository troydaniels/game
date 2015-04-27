//Implementation of the Game of Life in C for Unix systems
//See http://en.wikipedia.org/wiki/Conway's_Game_of_Life for explaination
//Free for use under the GNU Public Licence v.3
//Troy Daniels  27/04/15

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "gameOfLife.h"

#define timeStep 1       //In seconds
#define maxSize 15

int size;
bool **board;

int main(int args, char *argv[]){

   if(args!=2 || sscanf (argv[1], "%i", &size)!=1 || size>maxSize){
      printf("Correct usage is %s int (0<int<=%d)\n", argv[0], maxSize);
      exit(1);
   }

   time_t seconds;
   srand((unsigned)time(&seconds));

   initBoard();

   while(1){
      waitFor(timeStep);
      updateBoard();
   }

   return 0;
}

//Initiates random board of size argv[1]*argv[1], and prints initial config
void initBoard(void){
   int i,j;
   board=malloc(size*sizeof(bool));
   for(i=0; i<size; i++){
      board[i]=malloc(size*sizeof(bool));
      for (j=0; j<size; j++){
          board[i][j]=rand()%2;    //populate with lifeforms
      }
   }
   printBoard();
}

//Prints board
void printBoard(void){
   int i,j;
   for(i=0; i<size; i++){
      for (j=0; j<size; j++){
         if(board[i][j]){
            printf("x");
         }else{
            printf(" ");
         }
      }
      printf("\n");
   }
}

//Updates board, clears screen and prints new layout
void updateBoard(void){
   
   system("clear");
   printBoard();
}

void waitFor (unsigned int secs) {
    int retTime = time(0) + secs;     // finishing time
    while (time(0) < retTime);    // spin
}
