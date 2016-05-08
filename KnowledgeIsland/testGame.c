+//Name: Jeremy Hamann, Raycole Dai, Acacia Mok
+//      Samantha Pulikottil, Adam Chen
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
void testActions(Game g);
void testLegalAction(Game g);

int main(int argc, char * argv[]){
   //Creates a new game, based on bad map
   int disciplines[] = BADMAP_DISCIPLINES;
   int dice[] = BADMAP_DICE;
   Game badMapGame = newGame (disciplines, dice);
   //Tests the game
   testInitialState(badMapGame);
   //Tests each actiom to see if they are legal
   testLegalAction(badMapGame);
   //Tests each action
   testActions(badMapGame);
   //Free's memory from game
   disposeGame(badMapGame);
   
   //Creates a new game, based on good map
   int disciplines[] = GOODMAP_DISCIPLINES;
   int dice[] = GOODMAP_DICE;
   Game goodMapGame = newGame (disciplines, dice);
   //Tests the game
   testInitialState(goodMapGame);
   //Tests each action to see if they are legal
   testLegalAction(goodMapGame);
   //Tests each action
   testActions(goodMapGame);
   //Free's memory from game
   disposeGame(goodMapGame);
   
   printf("All tests passed, you are Awesome!\n");
   return EXIT_SUCCESS;
}


//Test each "void makeAction (Game g, action a)"
//makeAction() makes the specified action for the current player and updates the game state accordingly.
void testActions(Game g){
  /*  #define PASS 0
      #define BUILD_CAMPUS 1
      #define BUILD_GO8 2
      #define OBTAIN_ARC 3
      #define START_SPINOFF 4
      #define OBTAIN_PUBLICATION 5
      #define OBTAIN_IP_PATENT 6
      #define RETRAIN_STUDENTS 7 */
  //You can assume that the action is LEGAL, no need to check
  
  
}

//Tests each action to see if they are legal, int isLegalAction (Game g, action a);
void testLegalAction(Game g, int player){
  assert(isLegalAction(g, PASS) == TRUE);
  assert(isLegalAction(g, BUILD_CAMPUS) == TRUE &&
          getStudents(g, player, STUDENT_BPS) >= 1 && 
          getStudents(g, player, STUDENT_BQN) >= 1) &&
          getStudents(g, player, STUDENT_MJ) >= 1 && 
          getStudents(g, player, STUDENT_MTV) >= 1);
  assert (isLegalAction(g, BUILD_GO8) == TRUE && 
          getStudents(g, player, STUDENT_MJ) >= 2 && 
          getStudents(g, player, STUDENT_MMONEY) >= 3);
  assert(isLegalAction(g, OBTAIN_ARC) == TRUE &&
          getStudents(g, player, STUDENT_BQN) >= 1 && 
          getStudents(g, player, STUDENT_BPS) >= 1);
  assert(isLegalAction(g, START_SPINOFF) == TRUE &&
          getStudents(g, player, STUDENT_MJ) >= 1 && 
          getStudents(g, player, STUDENT_MTV) >= 1 && 
          getStudents(g, player, STUDENT_MMONEY) >= 1);
  assert(isLegalAction(g, OBTAIN_PUBLICATION) == FALSE);
  assert(isLegalAction(g, OBTAIN_IP_PATENT) == FALSE);

  //Test Retrain_Students
  int testStudentFrom = STUDENT_BPS;
  int testStudentTo = STUDENT_BPS
  while (testStudent <= STUDENT_MMONEY) {
    while (testStudentFrom <= STUDENT_MMONEY) {
      if (testStudentFrom != testStudentTo) {
        assert(isLegalAction(g, RETRAIN_STUDENTS) == TRUE &&
              getStudents(g, player, testStudentFrom) - \
              getExchangeRate (g, player, testStudentFrom, testStudentTo) >= 0);
      }  
      testStudentTo ++;
    }
    testStudentFrom ++;
    testStudentTo = STUDENT_BPS;
  }
  printf ("All isLegalAction() tests passed!\n");
}


//Sample out testing simple things
void testInitialState(Game g){
   printf("Test initialState!\n");

  //Check each region produces correct disciplines
   int regionID = 0;
   while (regionID < NUM_REGIONS) {
      assert(getDiscipline(g,0) == disciplines[0]);
   }

   //Check what dice value produces students in the specified region
   regionID = 0;
   while (regionID < NUM_REGIONS) {
      assert(getDiceValue(g,0) == dice[0]);
   }

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
      int testStudentFrom = STUDENT_BPS;
      int testStudentTo = STUDENT_BPS
      while (testStudentFrom <= STUDENT_MMONEY) {
        while (testStudentTo <= STUDENT_MMONEY) {
          if (testStudentFrom != testStudentTo) {
            assert(getExchangeRate (g, uni, testStudentFrom, testStudentTo) == 3);
          }
          testStudentTo ++;
        }
        testStudentFrom ++;
        testStudentTo = STUDENT_BPS;
      }
      uni++;
   }
   printf ("All initialState tests passed!\n");
}
