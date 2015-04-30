//Implementation of the Game of Life in C for Unix systems
//See http://en.wikipedia.org/wiki/Conway's_Game_of_Life for explaination
//Free for use under the GNU Public Licence v.3
//Troy Daniels  27/04/15

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "colourFull.h"

#define GRE "\x1B[32m"
#define BLU "\x1B[34m"
#define RED "\x1B[31m"
#define RES "\033[0m"    //Reset cursor

#define TIMESTEP 1       //In integer seconds
#define SYMBOL "x"       //Shape of lifeform
#define SPACE " "        //Defines empty plane
#define HEAD 5           //Width of header

struct winsize w;

int main(int args, char *argv[]){
   if(args!=1){
      printf("Unrecognised argument: %s\n", argv[1]);
      exit(1);
   }

   //Some magic to get the size of the terminal
   ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

   int board[w.ws_row*(w.ws_col-HEAD)];

   time_t seconds;
   srand((unsigned)time(&seconds));

   initBoard(board);
   runSimulation(board);

   return 0;
}

void runSimulation(int *board){
   int itter;
   for(itter=0; ;itter++){
      system("clear");
      printBoard(board, itter);
      waitFor(TIMESTEP);
      updateBoard(board);
   }
}

//Initialises random board, to fit nicely inside the current terminal
void initBoard(int *board){
   int i;
   int width=w.ws_col;
   int height=w.ws_row-HEAD;
   int area=width*height;

   for(i=0; i<area; i++){
       board[i]=rand()%2;    //Populate with lifeforms
      printf("%d ", board[i]);
   }
   printf("\n");
}

//Prints board
void printBoard(int *board, int itter){
   int i;
   int width=w.ws_col;
   int height=w.ws_row-HEAD;
   int area=width*height;

   printf("Generation: %d\n", itter);

   for(i=0; i<area; i++){
      switch(board[i]){
         case(0):
            printf(SPACE);
            break;
         case(1):
            printf(SYMBOL);
            break;
         case(2):
            printf(RED SYMBOL RES);
            break;
         case(3):
            printf(GRE SYMBOL RES);
            break;
         default:
            printf(BLU SYMBOL RES);
            break;
     }
   if(i && (i+1)%width==0)
      printf("\n");
   }
   printf("\n");
}

//Calculates next generation, and populates board accordingly
void updateBoard( int *board){
   int i;
   int width=w.ws_col;
   int height=w.ws_row-HEAD;
   int area=width*height;
   int *tally;
   tally=calloc(area, sizeof(int));

   //Tally adjacent lifeforms
   //this gave strange float exceptions when abreviating to x^y notation
   for(i=0; i<area; i++){
      tally[i]+=(_Bool)board[(i+width+area)%area];  //down
      tally[i]+=(_Bool)board[(i-width+area)%area];  //up
      tally[i]+=(_Bool)board[(i+1+area)%area];     //right
      tally[i]+=(_Bool)board[(i-1+area)%area];     //left
      tally[i]+=(_Bool)board[(i+width+1+area)%area];//down right
      tally[i]+=(_Bool)board[(i+width-1+area)%area];//down left
      tally[i]+=(_Bool)board[(i-width+1+area)%area];//up right
      tally[i]+=(_Bool)board[(i-width-1+area)%area];//up left
   }

   //update board based on tally
   for(i=0; i<area; i++){
      if(board[i]==0 && tally[i]==3){
         board[i]=1;
      }else if(board[i]>0 && (tally[i]<2 || tally[i]>3)){
         board[i]=0;
      }else if(board[i]!=0){
         board[i]=(board[i]+1)%INT_MAX;
      }
   }
   free(tally);
}

void waitFor (unsigned int secs) {
    int retTime = time(0) + secs; // finishing time
    while (time(0) < retTime);    // spin
}
