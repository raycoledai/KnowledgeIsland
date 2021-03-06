//Name: Raycole Dai, Jeremy Hamann, Acacia Mok, Samantha Pulikottil, Adam Chen
//Date: 03/05/16
//Desc: Tests to see if the game works and no cheating

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Game.h"

#define DEFAULT_DISCIPLINES {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV, \
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN, \
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS}
#define DEFAULT_DICE {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5}

void testInitialState(Game g);
void testLegalAction(Game g, int player);

int main(int argc, char * argv[]){
   //Creates a new game, based on default map
   int discipline[] = DEFAULT_DISCIPLINES;
   int dice[] = DEFAULT_DICE;
   printf ("Building Game!\n");
   Game g = newGame (discipline, dice);
   printf ("Game built!\n");
   //Tests the game
   testInitialState(g);
   throwDice(g, 7);
   //Tests each actiom to see if they are legal
   testLegalAction(g, UNI_A);
   testLegalAction(g, UNI_B);
   testLegalAction(g, UNI_C);
   //Free's memory from game
   disposeGame(g);

   printf("All tests passed, you are Awesome!\n");
   return EXIT_SUCCESS;
}

//Sample out testing simple things
void testInitialState(Game g){
   printf("Testing initialState!\n");
   int disciplines[] = DEFAULT_DISCIPLINES;
   int dice[] = DEFAULT_DICE;
   //Check each region produces correct disciplines
   int regionID = 0;
   while (regionID < NUM_REGIONS) {
      assert(getDiscipline(g,regionID) == disciplines[regionID]);
      regionID ++;
   }

   //Check what dice value produces students in the specified region
   regionID = 0;
   while (regionID < NUM_REGIONS) {
      assert(getDiceValue(g,regionID) == dice[regionID]);
      regionID ++;
   }

   assert(getMostARCs(g) == NO_ONE);
   assert(getMostPublications(g) == NO_ONE);
   assert(getTurnNumber(g) == -1);
   assert(getWhoseTurn(g) == NO_ONE);

   //Check individual uni values
   int uni = UNI_A;
   while(uni <= UNI_C){

      //Check Uni's exchange rates
      int testStudentFrom = STUDENT_BPS;
      int testStudentTo = STUDENT_BPS;
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

      uni++;
   }
   //Test isLegalAction at Terra Nullis
   action passAction;
   action CampusAction;
   action GO8Action;
   action ARCAction;
   action spinoffAction;
   action publicationAction;
   action patentAction;
   action retrainAction;
   passAction.actionCode = PASS;
   CampusAction.actionCode = BUILD_CAMPUS;
   GO8Action.actionCode = BUILD_GO8;
   ARCAction.actionCode = OBTAIN_ARC;
   spinoffAction.actionCode = START_SPINOFF;
   publicationAction.actionCode = OBTAIN_PUBLICATION;
   patentAction.actionCode = OBTAIN_IP_PATENT;
   retrainAction.actionCode = RETRAIN_STUDENTS;
   assert(isLegalAction(g, passAction) == FALSE);
   assert(isLegalAction(g, CampusAction) == FALSE);
   assert (isLegalAction(g, GO8Action) == FALSE);
   assert(isLegalAction(g, ARCAction) == FALSE);
   assert(isLegalAction(g, spinoffAction) == FALSE);
   assert(isLegalAction(g, publicationAction) == FALSE);
   assert(isLegalAction(g, patentAction) == FALSE);
   assert(isLegalAction(g, retrainAction) == FALSE);
   printf ("All initialState tests passed!\n");
}

//Tests each action to see if they are legal, int isLegalAction (Game g, action a);
void testLegalAction(Game g, int player){
   printf("Testing isLegalAction!\n");
   action passAction;
   action CampusAction;
   action GO8Action;
   action ARCAction;
   action spinoffAction;
   action publicationAction;
   action patentAction;

   passAction.actionCode = PASS;
   CampusAction.actionCode = BUILD_CAMPUS;
   GO8Action.actionCode = BUILD_GO8;
   ARCAction.actionCode = OBTAIN_ARC;
   spinoffAction.actionCode = START_SPINOFF;
   publicationAction.actionCode = OBTAIN_PUBLICATION;
   patentAction.actionCode = OBTAIN_IP_PATENT;

   assert(isLegalAction(g, passAction) == TRUE);
   assert(isLegalAction(g, CampusAction) == TRUE &&
          getStudents(g, player, STUDENT_BPS) >= 1 &&
          getStudents(g, player, STUDENT_BQN) >= 1 &&
          getStudents(g, player, STUDENT_MJ) >= 1 &&
          getStudents(g, player, STUDENT_MTV) >= 1);
   assert((isLegalAction(g, CampusAction) == FALSE) &&
          ((getStudents(g, player, STUDENT_BPS) < 1) ||
          (getStudents(g, player, STUDENT_BQN) < 1) ||
          (getStudents(g, player, STUDENT_MJ) < 1) ||
          (getStudents(g, player, STUDENT_MTV) < 1)));

   assert (isLegalAction(g, GO8Action) == TRUE &&
          getStudents(g, player, STUDENT_MJ) >= 2 &&
          getStudents(g, player, STUDENT_MMONEY) >= 3);
   assert ((isLegalAction(g, GO8Action) == FALSE) &&
          ((getStudents(g, player, STUDENT_MJ) < 2) ||
          (getStudents(g, player, STUDENT_MMONEY) < 3)));

   assert(isLegalAction(g, ARCAction) == TRUE &&
          getStudents(g, player, STUDENT_BQN) >= 1 &&
          getStudents(g, player, STUDENT_BPS) >= 1);
   assert((isLegalAction(g, ARCAction) == FALSE) &&
         ((getStudents(g, player, STUDENT_BQN) < 1) ||
         (getStudents(g, player, STUDENT_BPS) < 1)));

   assert(isLegalAction(g, spinoffAction) == TRUE &&
          getStudents(g, player, STUDENT_MJ) >= 1 &&
          getStudents(g, player, STUDENT_MTV) >= 1 &&
          getStudents(g, player, STUDENT_MMONEY) >= 1);
   assert((isLegalAction(g, spinoffAction) == FALSE) &&
         ((getStudents(g, player, STUDENT_MJ) < 1) ||
         (getStudents(g, player, STUDENT_MTV) < 1) ||
         (getStudents(g, player, STUDENT_MMONEY) < 1)));

   assert(isLegalAction(g, publicationAction) == FALSE);
   assert(isLegalAction(g, patentAction) == FALSE);

   //Test Retrain_Students
   int testStudentFrom = STUDENT_BPS;
   int testStudentTo = STUDENT_BPS;
   action retrainAction;
   retrainAction.actionCode = RETRAIN_STUDENTS;
   retrainAction.disciplineFrom = testStudentFrom;
   retrainAction.disciplineTo = testStudentTo;
   while (testStudentFrom <= STUDENT_MMONEY) {
      while (testStudentTo <= STUDENT_MMONEY) {
         if (testStudentFrom != testStudentTo) {
            assert(isLegalAction(g, retrainAction) == TRUE &&
                   getStudents(g, player, testStudentFrom) - \
                   getExchangeRate (g, player, testStudentFrom, testStudentTo) >= 0);
         }
         testStudentTo ++;
         retrainAction.disciplineTo = testStudentTo;
      }
      testStudentFrom ++;
      retrainAction.disciplineFrom = testStudentFrom;
      retrainAction.disciplineTo = STUDENT_BPS;
   }
   printf ("All isLegalAction() tests passed!\n");
}
