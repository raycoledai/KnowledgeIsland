//

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Date.h"


typedef struct _game {
   int currentTurn;
   Map map //(map is a struct we'll make later)
   University universities[NUM_UNIS] //(university is also another struct for later)
} game;



// make the specified action for the current player and update the
// game state accordingly.
// The function may assume that the action requested is legal.
// START_SPINOFF is not a legal action here
void makeAction (Game g, action a) {

}

// advance the game to the next turn,
// assuming that the dice has just been rolled and produced diceScore
// the game starts in turn -1 (we call this state "Terra Nullis") and
// moves to turn 0 as soon as the first dice is thrown.
void throwDice (Game g, int diceScore) {

}

/* **** Functions which GET data about the game aka GETTERS **** */

// what type of students are produced by the specified region?
// regionID is the index of the region in the newGame arrays (above)
// see discipline codes above
int getDiscipline (Game g, int regionID) {

}

// what dice value produces students in the specified region?
// 2..12
int getDiceValue (Game g, int regionID) {

}

// which university currently has the prestige award for the most ARCs?
// this is NO_ONE until the first arc is purchased after the game
// has started.
int getMostARCs (Game g) {

}

// which university currently has the prestige award for the most pubs?
// this is NO_ONE until the first publication is made.
int getMostPublications (Game g) {

}

// return the current turn number of the game -1,0,1, ..
int getTurnNumber (Game g) {

}

// return the player id of the player whose turn it is
// the result of this function is NO_ONE during Terra Nullis
int getWhoseTurn (Game g) {

}

// return the contents of the given vertex (ie campus code or
// VACANT_VERTEX)
int getCampus(Game g, path pathToVertex) {

}

// the contents of the given edge (ie ARC code or vacent ARC)
int getARC(Game g, path pathToEdge);

// returns TRUE if it is legal for the current
// player to make the specified action, FALSE otherwise.
//
// "legal" means everything is legal:
//   * that the action code is a valid action code which is legal to
//     be made at this time
//   * that any path is well formed and legal ie consisting only of
//     the legal direction characters and of a legal length,
//     and which does not leave the island into the sea at any stage.
//   * that disciplines mentioned in any retraining actions are valid
//     discipline numbers, and that the university has sufficient
//     students of the correct type to perform the retraining
//
// eg when placing a campus consider such things as:
//   * is the path a well formed legal path
//   * does it lead to a vacent vertex?
//   * under the rules of the game are they allowed to place a
//     campus at that vertex?  (eg is it adjacent to one of their ARCs?)
//   * does the player have the 4 specific students required to pay for
//     that campus?
// It is not legal to make any action during Terra Nullis ie
// before the game has started.
// It is not legal for a player to make the moves OBTAIN_PUBLICATION
// or OBTAIN_IP_PATENT (they can make the move START_SPINOFF)
// you can assume that any pths passed in are NULL terminated strings.
int isLegalAction (Game g, action a) {

}

// --- get data about a specified player ---

// return the number of KPI points the specified player currently has
int getKPIpoints (Game g, int player){

}

// return the number of ARC grants the specified player currently has
int getARCs (Game g, int player) {

}

// return the number of GO8 campuses the specified player currently has
int getGO8s (Game g, int player) {

}

// return the number of normal Campuses the specified player currently has
int getCampuses (Game g, int player) {

}

// return the number of IP Patents the specified player currently has
int getIPs (Game g, int player) {

}

// return the number of Publications the specified player currently has
int getPublications (Game g, int player) {

}

// return the number of students of the specified discipline type
// the specified player currently has
int getStudents (Game g, int player, int discipline) {

}

// return how many students of discipline type disciplineFrom
// the specified player would need to retrain in order to get one
// student of discipline type disciplineTo.  This will depend
// on what retraining centers, if any, they have a campus at.
int getExchangeRate (Game g, int player, int disciplineFrom, int disciplineTo) {

}
// free all the memory malloced for the game
void disposeGame (Game g) {
   free (g);
}
