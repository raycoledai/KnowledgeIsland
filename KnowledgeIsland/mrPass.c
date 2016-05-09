/*
 *  Mr Pass.  Brain the size of a planet!
 *
 *  Proundly Created by Richard Buckland
 *  Share Freely Creative Commons SA-BY-NC 3.0.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Game.h"
#include "mechanicalTurk.h"

action decideAction (Game g) {
   action nextAction;
   int uniID = getWhoseTurn (Game g);
   if (g.universities[uniID].studentCount.mj >= 1 &&
       g.universities[uniID].studentCount.mtv >= 1 &&
       g.universities[uniID].studentCount.mmoney >= 1) {
      nextAction.actionCode = START_SPINOFF;
   } else {
      nextAction.actionCode = PASS;
   }
   return nextAction;
}
