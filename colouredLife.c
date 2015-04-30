//Implementation of the Game of Life in C for Unix systems
//See http://en.wikipedia.org/wiki/Conway's_Game_of_Life for explaination
//Free for use under the GNU Public Licence v.3
//Troy Daniels  27/04/15

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "colouredLife.h"

#define GRE "\x1B[32m"
#define BLU "\x1B[34m"
#define RED "\x1B[31m"
#define RES "\033[0m"    //Reset

#define TIMESTEP 1       //In integer seconds
#define MAXSIZE 50       //Size of side of living plane
#define SYMBOL "x"       //Shape of lifeform

int main(int args, char *argv[]){
   int size;

   if(args!=2 || sscanf (argv[1], "%i", &size)!=1 || size>MAXSIZE){
      printf("Correct usage is %s int (0<int<=%d)\n", argv[0], MAXSIZE);
      exit(1);
   }

   int board[size*size];

   time_t seconds;
   srand((unsigned)time(&seconds));

   initBoard(size, board);

   while(1){
      system("clear");
      printBoard(size, board);
      waitFor(TIMESTEP);
      updateBoard(size, board);
   }

   return 0;
}

//Initialises random board of size argv[1]*argv[1], and prints initial config
void initBoard(int size, int *board){
   int i;
   for(i=0; i<(size*size); i++){
       board[i]=rand()%2;    //populate with lifeforms
   }
   printf("\n");
}

//Prints board
void printBoard(int size, int *board){
   int i;
   for(i=0; i<(size*size); i++){
      switch(board[i]){
         case(0):
            printf(" ");
            break;
         case(1):
            printf(SYMBOL);
            break;
         case(2):
            printf(BLU SYMBOL RES);
            break;
         case(3):
            printf(GRE SYMBOL RES);
            break;
         default:
            printf(RED SYMBOL RES);
            break;
     }

   if(i && (i+1)%size==0)
      printf("\n");
   }
   printf("\n");
}

//Updates board layout & clears screen
void updateBoard(int size, int *board){
   int i;
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
   for(i=0; i<(size*size); i++){
      if(tally[i]<2 || tally[i]>3){
         board[i]=0;
      }else if(tally[i]==3 || (tally[i]==2 && board[i])){
         board[i]+=1;
      }
   }
   free(tally);
}

void waitFor (unsigned int secs) {
    int retTime = time(0) + secs;     // finishing time
    while (time(0) < retTime);    // spin
}
