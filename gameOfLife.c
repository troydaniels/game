//Implementation of the Game of Life in C for Unix systems
//See http://en.wikipedia.org/wiki/Conway's_Game_of_Life for explaination
//Free for use under the GNU Public Licence v.3
//Troy Daniels  27/04/15

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "gameOfLife.h"

#define TIMESTEP 1       //In integer seconds
#define MAXSIZE 50       //Size of side of living plane

int main(int args, char *argv[]){
   int size;

   if(args!=2 || sscanf (argv[1], "%i", &size)!=1 || size>MAXSIZE){
      printf("Correct usage is %s int (0<int<=%d)\n", argv[0], MAXSIZE);
      exit(1);
   }

   _Bool board[size*size];

   time_t seconds;
   srand((unsigned)time(&seconds));

   initBoard(size, board);
   printBoard(size, board);

   while(1){
      waitFor(TIMESTEP);
      updateBoard(size, board);
      printBoard(size, board);
   }

   return 0;
}

//Initialises random board of size argv[1]*argv[1], and prints initial config
void initBoard(int size, _Bool *board){
   int i;
   for(i=0; i<(size*size); i++){
       board[i]=rand()%2;    //populate with lifeforms
   }
   printf("\n");
}

//Prints board
void printBoard(int size, _Bool *board){
   int i;
   for(i=0; i<(size*size); i++){
      printf(board[i] ? "x" : " ");    //Defines organism shape
      if(i && (i+1)%size==0)
         printf("\n");
   }
   printf("\n");
}

//Updates board layout & clears screen
void updateBoard(int size, _Bool *board){
   int i,j;
   int *tally;
   tally=calloc(size*size, sizeof(int));

   //Tally adjacent lifeforms
   //this gave strange float exceptions when abreviating to x^y notation
   for(i=0; i<(size*size); i++){
      tally[i]+=board[(i+size+size*size)%(size*size)];  //down
      tally[i]+=board[(i-size+size*size)%(size*size)];  //up
      tally[i]+=board[(i+1+size*size)%(size*size)];     //right
      tally[i]+=board[(i-1+size*size)%(size*size)];     //left
      tally[i]+=board[(i+size+1+size*size)%(size*size)];//down right
      tally[i]+=board[(i+size-1+size*size)%(size*size)];//down left
      tally[i]+=board[(i-size+1+size*size)%(size*size)];//up right
      tally[i]+=board[(i-size-1+size*size)%(size*size)];//up left
   }

   //update board based on tally
   for(j=0; j<(size*size); j++){
      if(tally[j]<2 || tally[j]>3){
         board[j]=0;
      }else if(tally[j]==3){
         board[j]=1;
      }
   }
   free(tally);
   system("clear");
}

void waitFor (unsigned int secs) {
    int retTime = time(0) + secs;     // finishing time
    while (time(0) < retTime);    // spin
}
