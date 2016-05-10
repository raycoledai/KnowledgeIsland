/*
 *
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Game.h"
#include "mechanicalTurk.h"

typedef struct _StudentCount {
   int thd;
   int bps;
   int bqn;
   int mj;
   int mtv;
   int mmoney;
} StudentCount;

void getAllStudents (StudentCount s);
char findNormalPath (Game g);
char findGO8Path (Game g);

action decideAction (Game g) {
   action nextAction;
   int uniID = getWhoseTurn (Game g);
   StudentCount myStudents;
   getAllStudents(myStudents);
   if (getARCs(g, uniID) >= 2) {
      //ADVANCED:Check Owned ARCs and see what students their vertex's produce
      if (g.totalGo8CampusCount < 8 && getCampuses(g, uniID) > 0 &&
          myStudents.mj >= 2 && myStudents.mmoney >= 3) {
         nextAction.actionCode = BUILD_GO8;
         nextAction.path = findGO8Path(g);
      } else if (myStudents.bps >= 1 && myStudents.bqn >= 1 &&
                 myStudents.mj >=1 && myStudents.mtv >= 1){
         nextAction.actionCode = BUILD_CAMPUS;
         nextAction.path = findNormalPath(g);
      }
   } else if (myStudents.mj >= 1 && myStudents.mtv >= 1 &&
              myStudents.mmoney >= 1) {
      nextAction.actionCode = START_SPINOFF;
   } else {
      nextAction.actionCode = PASS;
   }
   return nextAction;
}

void getAllStudents(StudentCount s) {
   s->thd    = getStudents(g, uniID, STUDENT_THD);
   s->bps    = getStudents(g, uniID, STUDENT_BPS);
   s->bqn    = getStudents(g, uniID, STUDENT_BQN);
   s->mj     = getStudents(g, uniID, STUDENT_MJ);
   s->mtv    = getStudents(g, uniID, STUDENT_MTV);
   s->mmoney = getStudents(g, uniID, STUDENT_MMONEY);
}
char findNormalPath (Game g) {
   //Search through owned ARCs && finds path to it
   return "";
}

char findGO8Path (Game g) {
   //Search for Campuses && Find path to it
   return "";
}
