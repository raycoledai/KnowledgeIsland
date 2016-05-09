#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Game.h"
#include "mechanicalTurk.h"

#include <time.h>
#include <assert.h>

//Can switch in different AIs instead of mrPass
//gcc -Wall -std=gnu99 -g -O -fno-omit-frame-pointer -lm runGame runGame.c mrPass.c Game.c
//This sample uses AI for uni a and b and human for player c

//If not all code done can at least compile this file
//but not end up with an executable
//gcc -Wall -std=gnu99 -g -O -fno-omit-frame-pointer -c runGame.c

//STILL ROUGH. Students NEED TO FIX FOR runGame.c activity
//Needs to get human move input
//print helpful output
//Make ways to run different boards (ie disciplines,dice permutations)
//Test it is right!! May be buggy? Use at own risk!


#define WINNING_KPI 150
#define FALSE 0
#define TRUE 1


#define DEFAULT_DISCIPLINES {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV, \
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN, \
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS}
#define DEFAULT_DICE {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5}

int checkForWinner(Game g);
int rollDice(int numRolls, int diceSides);
action getMove(Game g);
action getUserMove(Game g);

int main(int argc, char * argv[]){
    int disciplines[] = DEFAULT_DISCIPLINES;
    int dice[] = DEFAULT_DICE;
    int turnOver = FALSE;
    int winner = NO_ONE;
    int diceValue;
    action move;
    int whoseTurn = NO_ONE;

    srand(time(NULL)); //initialises a seed
    Game g = newGame(disciplines,dice);

    while (winner == NO_ONE) {

        diceValue = rollDice(2,6);

        throwDice(g,diceValue);

        printf("[Turn %d] The turn now belongs to University %d!\n",
            getTurnNumber(g),
            getWhoseTurn(g));
        printf("The dice casted a %d!\n", diceValue);

        whoseTurn = getWhoseTurn(g);
        //loop until player action is PASS
        turnOver = FALSE;
        while (!turnOver) {
            move = getMove(g);
            //print info about move
            printf("Move %d \n",move.actionCode);
            assert(isLegalAction(g, move));

            if (move.actionCode == START_SPINOFF) {

                //decide if outcome is patent or publication
                if (rand() % 3 <= 1) {
                    move.actionCode = OBTAIN_PUBLICATION;
                } else {
                    move.actionCode = OBTAIN_IP_PATENT;
                }
            }

            if(move.actionCode == PASS){
                 turnOver = TRUE;
                 printf("Pass\n");
            }
            makeAction(g, move);


            if(getKPIpoints(g, whoseTurn) >= WINNING_KPI){
                winner = whoseTurn;
                turnOver = TRUE;
                printf("Winner %d\n",whoseTurn);
            }

        }
    }


    // TODO print statistics

    // TODO free memory

    return EXIT_SUCCESS;
}


int checkForWinner(Game g) {
   int winner = NO_ONE;
   int playerIndex;

   playerIndex = UNI_A;
   while (playerIndex <= UNI_C) {
      // check if the player is over or equal the WINNING_KPI
      if (getKPIpoints(g, playerIndex) >= WINNING_KPI) {
         winner = playerIndex;
      }
      playerIndex++;
   }

   return winner;
}

int rollDice(int numRolls, int diceSides) {

   int diceRollCount = 0;
   int diceValue = 0;

   //"rolls the die"
   while (diceRollCount < numRolls) {
      diceValue += 1 + rand() % diceSides;
      diceRollCount++;
   }

   return diceValue;
}

//Just one possibility for mixing in AI and human
//Students can modify however they like.
action getMove(Game g){

  action move;
  if(getWhoseTurn(g) == UNI_A || getWhoseTurn(g) == UNI_B){
     move = decideAction(g); //AI
  } else {
      move = getUserMove(g);
  }
  return move;
}

//gets next move from user
//Leave for them to do
action getUserMove(Game g) {

   action move;

  //TODO user interaction

   return move;
}
