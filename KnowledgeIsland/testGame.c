//Name: Jeremy Hamann (z5112913)
//Date: 03/05/16
//Desc: Tests to see if the game works and no cheating

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Game.h"


#define BADMAP_DISCIPLINES {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV, \
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN, \
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS}

#define BADMAP_DICE {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5}

#define GOODMAP_DISCIPLINES {STUDENT_MMONEY, STUDENT_MJ, STUDENT_MJ, \
                STUDENT_BQN, STUDENT_MTV, STUDENT_MJ, STUDENT_BPS, \
                STUDENT_BPS, STUDENT_MTV,STUDENT_THD, STUDENT_BQN, \
                STUDENT_BPS, STUDENT_MMONEY, STUDENT_MJ, STUDENT_MMONEY, \
                STUDENT_MTV, STUDENT_BQN, STUDENT_BQN, STUDENT_BQN}

#define GOODMAP_DICE {6,10,8,3,4,9,6,11,11,7,12,3,5,2,5,10,9,4,8}




void testInitialState(Game g);

int main(int argc, char * argv[]){
   //Creates a new game, based on bad map
   int disciplines[] = BADMAP_DISCIPLINES;
   int dice[] = BADMAP_DICE;
   Game badMapGame = newGame (disciplines, dice);
   //Tests the game
   testInitialState(badMapGame);
   //Free's memory from game
   disposeGame(badMapGame);
   
   //Creates a new game, based on good map
   int disciplines[] = GOODMAP_DISCIPLINES;
   int dice[] = GOODMAP_DICE;
   Game goodMapGame = newGame (disciplines, dice);
   //Tests the game
   testInitialState(goodMapGame);
   //Free's memory from game
   disposeGame(goodMapGame);
   
   printf("All tests passed, you are Awesome!\n");
   return EXIT_SUCCESS;
}

//Sample out testing simple things
void testInitialState(Game g){
   printf("Test initialState!\n");

   assert(getDiscipline(g,0) == STUDENT_BQN);
   assert(getDiceValue(g,0) == 9);


   assert(getMostARCs(g) == NO_ONE);
   assert(getMostPublications(g) == NO_ONE);
   assert(getTurnNumber(g) == -1);
   assert(getWhoseTurn(g) == NO_ONE);
   
   //Check individual uni values
   int uni = UNI_A;
   while(uni <= UNI_C){
      assert(getKPIpoints(g,uni) == 20);
      assert(getARCs(g,uni) == 0);
      assert(getGO8s(g,uni) == 0);
      assert(getCampuses(g,uni) == 2);
      assert(getPublications(g,uni) == 0);
      assert(getIPs(g,uni) == 0);
      
      //Check initial student values 
      assert(getStudents(g,uni,STUDENT_THD)==0);
      assert(getStudents(g,uni,STUDENT_BPS)==3);
      assert(getStudents(g,uni,STUDENT_BQN)==3);
      assert(getStudents(g,uni,STUDENT_MJ)==1);
      assert(getStudents(g,uni,STUDENT_MTV)==1);
      assert(getStudents(g,uni,STUDENT_MMONEY)==1);
       
      //Check Uni's exchange rates
      int exchangeRateCheck = STUDENT_BPS;
      while(exchangeRateCheck <= STUDENT_MMONEY){
         assert(getExchangeRate(g,uni,exchangeRateCheck,STUDENT_BPS) == 3);
         assert(getExchangeRate(g,uni,exchangeRateCheck,STUDENT_BQN) == 3);
         assert(getExchangeRate(g,uni,exchangeRateCheck,STUDENT_MJ) == 3);
         assert(getExchangeRate(g,uni,exchangeRateCheck,STUDENT_MTV) == 3);
         assert(getExchangeRate(g,uni,exchangeRateCheck,STUDENT_MMONEY) == 3);
         discipline++;
      }
      uni++;
   }
}
