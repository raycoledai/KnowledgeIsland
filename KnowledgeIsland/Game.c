#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "Game.h"

#define DEFAULT_DISCIPLINES {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV, \
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN, \
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS}
#define DEFAULT_DICE {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5}


#define KPI_PER_ARC 2
#define KPI_PER_CAMPUS 10
#define KPI_PER_GO8 20
#define KPI_PER_PATENT 10
#define KPI_FOR_MOST_PUBS 10
#define KPI_FOR_MOST_ARCS 10

#define START_NUM_THD 0
#define START_NUM_BPS 3
#define START_NUM_BQN 3
#define START_NUM_MJ 1
#define START_NUM_MTV 1
#define START_NUM_MMONEY 1
#define START_NUM_PUBLICATIONS 0
#define START_NUM_PATENTS 0

#define NUM_VERTICES 54
#define NUM_EDGES 72
#define SIDE_VERTICAL 0 //"|"
#define SIDE_FORWARDSLASH 1 //"/"
#define SIDE_BACKSLASH 2    //"\"

typedef struct _StudentCount {
   int thd;
   int bps;
   int bqn;
   int mj;
   int mtv;
   int mmoney;
   //COMPLETED
} StudentCount;

typedef struct _University {
   int playerId;
   StudentCount studentCount;
   int publicationCount;
   int patentCount;
   int ownedCampusCount;
   //Add something to STORE ownedCampuses;
   int ownedArcCount;
   //Add something to STORE ownedArcs;
} University;

typedef struct _regionLoc {
   int x;
   int y;
} regionLoc;

typedef struct _edgeLoc {
   regionLoc region0;
   regionLoc region1;
} edgeLoc;

typedef struct _vertexLoc {
   regionLoc region0;
   regionLoc region1;
   regionLoc region2;
} vertexLoc;

typedef struct _Region {
    regionLoc location;
    int regionID;
    int disciplineValue;
    int diceValue;
} Region;

typedef struct _Edge {
   edgeLoc location;
   int edgeType;
   int isOwned;
   int uniID;
} Edge;

typedef struct _Vertex {
   regionLoc location;
   regionLoc regions[3];
   int isOwned;
   int uniID;
} Vertex;

typedef struct _Map {
   Region regions[NUM_REGIONS];
   Edge edges[NUM_EDGES];
   Vertex vertices[NUM_VERTICES];
} Map;

typedef struct _game {
   int currentTurn;
   Map map;
   University universities[NUM_UNIS];
   int totalGo8CampusCount;
   int mostPublications;
   int mostPublicationsPlayer;
   int mostArcs;
   int mostArcsPlayer;
} game;

void initUniversity(University* university, int player);
void initMap (Map* map, int discipline[], int dice[]);
void initRegions (Region* r, int discipline[], int dice[]);
void initEdges (Edge* e);
int findEdgeType (int x, int y);
void initVertices (Vertex* v);
void addRegions(regionLoc* regions, int x, int y);
int isRegion (int x, int y);
int checkPoint (int x, int y);
void initGame(Game game, int discipline[], int dice[]);

Game newGame(int discipline[], int dice[]) {
    Game g = malloc(sizeof(Game));
    assert(g != NULL);
    initGame (g, discipline, dice);
    return g;
}

void initGame(Game g, int discipline[], int dice[]) {
   g->currentTurn = -1;
   g->totalGo8CampusCount = 0;
   g->mostPublications = 0;
   g->mostPublicationsPlayer = NO_ONE;
   g->mostArcs = 0;
   g->mostArcsPlayer = NO_ONE;
   initMap(&g->map, discipline, dice);
   initUniversity(&g->universities[0], UNI_A);
   initUniversity(&g->universities[1], UNI_B);
   initUniversity(&g->universities[2], UNI_C);
}

void initMap (Map* map, int discipline[], int dice[]) {
   //MEDIUM-HARD
   initRegions (map->regions, discipline, dice);
   initEdges (map->edges);
   initVertices (map->vertices);
}

void initRegions (Region* r, int discipline[], int dice[]) {
   int regionNum = 0;
   int realignNum = 0;
   int realignRegionNum[NUM_REGIONS] = {7,12,16,3,8,13,17,0,4,9,14,18,
                                        1,5,10,15,2,6,11};
   int x = -2;
   int y = 4;
   int end_x = x*(-1);
   regionLoc loc;
   while (y >= -4) {
      loc.y = y;
      while (x <= end_x) {
         loc.x = x;
         realignNum = realignRegionNum[regionNum];
         r[realignNum].location = loc;
         r[realignNum].regionID = realignNum;
         r[realignNum].disciplineValue = discipline[realignNum];
         r[realignNum].diceValue = dice[realignNum];
/*         printf("Region %d, (%d,%d), deg: %d, dice: %d\n", r[realignNum].regionID,
      r[realignNum].location.x, r[realignNum].location.y, r[realignNum].disciplineValue,
      r[realignNum].diceValue);*/
         x = x+2;
         regionNum++;
      }
      if (y==4||y==0) {
         x = -3;
      } else if (y==2) {
         x = -4;
      } else if (y==-2) {
         x = -2;
      }
      end_x = x*(-1);
      y = y-2;
   }
}

void initEdges (Edge* e) {
   int edgeNum = 0;
   int edgeType;
   int x = -3;
   int y = 5;
   int end_x = x*(-1);
   regionLoc loc;
   while (y >= -5) {
      loc.y = y;
      if (y%2== 1||y%2==-1) {
         while (x < end_x) {
            loc.x = x;
            edgeType = findEdgeType(x,y);
            e[edgeNum].location.region0 = loc;
            loc.x = x+1;
            e[edgeNum].location.region1 = loc;
            e[edgeNum].edgeType = edgeType;
            e[edgeNum].isOwned = VACANT_ARC;
            e[edgeNum].uniID = NO_ONE;
            x++;
/*            printf("Edge (%d,%d), (%d,%d), type: %d\n",
   e[edgeNum].location.region0.x, e[edgeNum].location.region0.y,
   e[edgeNum].location.region1.x, e[edgeNum].location.region1.y,
   e[edgeNum].edgeType);
*/
         }
      } else {
         while (x <= end_x) {
            loc.x = x;
            loc.y = y+1;
            edgeType = SIDE_VERTICAL;
            e[edgeNum].location.region0 = loc;
            loc.y = y-1;
            e[edgeNum].location.region1 = loc;
            e[edgeNum].edgeType = edgeType;
            e[edgeNum].isOwned = VACANT_ARC;
            e[edgeNum].uniID = NO_ONE;
            x = x+2;
/*            printf("Edge (%d,%d), (%d,%d), type: %d\n",
   e[edgeNum].location.region0.x, e[edgeNum].location.region0.y,
   e[edgeNum].location.region1.x, e[edgeNum].location.region1.y,
   e[edgeNum].edgeType);
*/
         }
      }
      if (y==5||y==-3||y==-4) {
         x = -3;
      } else if (y==4||y==3||y==-1||y==-2) {
         x = -4;
      } else if (y==2||y==1||y==0) {
         x = -5;
      }
      end_x = x*(-1);
      y--;
   }
}

int findEdgeType (int x, int y) {
   int result;
   y--;
   if (((x%2==1||x%2==-1) && y%4==0) || (x%2==0 && (y%4==2||y%4==-2))) {
      result = SIDE_FORWARDSLASH;
   } else {
      result = SIDE_BACKSLASH;
   }
   return result;
}

void initVertices (Vertex* v) {
   int vertexNum = 0;
   int isOwned;
   int uniID;
   int x = -3;
   int y = 5;
   int end_x = x*(-1);
   regionLoc loc;
   while (y >= -5) {
      loc.y = y;
      while (x <= end_x) {
         loc.x = x;
         v[vertexNum].location = loc;
         v[vertexNum].isOwned = VACANT_VERTEX;
         v[vertexNum].uniID = NO_ONE;
         addRegions(v[vertexNum].regions, x, y);
         x++;
         /*printf("Vertex: (%d,%d), R1:(%d,%d), R2:(%d, %d), R3: (%d, %d)\n",
         v[vertexNum].location.x, v[vertexNum].location.y,
         v[vertexNum].regions[0].x, v[vertexNum].regions[0].y,
         v[vertexNum].regions[1].x, v[vertexNum].regions[1].y,
         v[vertexNum].regions[2].x, v[vertexNum].regions[2].y); */
         vertexNum++;
      }
      if (y==5||y==-1) {
         x = -4;
      } else if (y==3||y==1) {
         x = -5;
      } else if (y==-3) {
         x = -3;
      }
      end_x = x*(-1);
      y = y-2;
   }
}

void addRegions(regionLoc* regions, int x, int y) {
   int regionCount = 0;
   regionLoc loc;
   if (isRegion(x-1, y+1) && checkPoint (x-1, y+1) && regionCount<3) {
      loc.x = x-1;
      loc.y = y+1;
      regions[regionCount] = loc;
      regionCount++;
      //printf("1");
   }
   if (isRegion(x, y+1) && checkPoint (x, y+1) && regionCount<3) {
      loc.x = x;
      loc.y = y+1;
      regions[regionCount] = loc;
      regionCount++;
      //printf("2");
   }
   if (isRegion(x+1,y+1) && checkPoint (x+1, y+1) && regionCount<3) {
      loc.x = x+1;
      loc.y = y+1;
      regions[regionCount] = loc;
      regionCount++;
      //printf("3");
   }
   if (isRegion(x-1,y-1) && checkPoint (x-1, y-1) && regionCount<3) {
      loc.x = x-1;
      loc.y = y-1;
      regions[regionCount] = loc;
      regionCount++;
      //printf("4");
   }
   if (isRegion(x,y-1) && checkPoint (x, y-1) && regionCount<3) {
      loc.x = x;
      loc.y = y-1;
      regions[regionCount] = loc;
      regionCount++;
      //printf("5");
   }
   if (isRegion(x+1,y-1) && checkPoint (x+1, y-1) && regionCount<3) {
      loc.x = x+1;
      loc.y = y-1;
      regions[regionCount] = loc;
      regionCount++;
      //printf("6");
   }
}

int isRegion (int x, int y) {
   int result = FALSE;
   if (((x%2==1||x%2==-1) && (y%4==2||y%4==-2)) || (x%2==0 && y%4==0)) {
      result = TRUE;
   }
   return result;
}

int checkPoint (int x, int y) {
   int point = FALSE;
   if (abs(x) <=3 && abs(y) <=5) {
      point = TRUE;
   } else if (abs(x) <= 4 && abs(y) <= 3) {
      point = TRUE;
   } else if (abs(x) <= 5 && abs(y) <= 1) {
      point = TRUE;
   }
   return point;
}

void initUniversity(University* university, int player) {
   university->playerId = player;
   university->studentCount.thd = START_NUM_THD;
   university->studentCount.bps = START_NUM_BPS;
   university->studentCount.bqn = START_NUM_BQN;
   university->studentCount.mj = START_NUM_MJ;
   university->studentCount.mtv = START_NUM_MTV;
   university->studentCount.mmoney = START_NUM_MMONEY;
   university->publicationCount = START_NUM_PUBLICATIONS;
   university->patentCount = START_NUM_PATENTS;
   university->ownedCampusCount = 2;
   """Add something to initialise owned campuses""";
   university->ownedArcCount = 0;
   """Add something to initialise owned ARCs""";
}

// advance the game to the next turn,
// assuming that the dice has just been rolled and produced diceScore
// the game starts in turn -1 (we call this state "Terra Nullis") and
// moves to turn 0 as soon as the first dice is thrown.
void throwDice (Game g, int diceScore) {
//MEDIUM
   """MISLEADING FUNCTION NAME-NOT ACTUALLY THROWING DICE""";
   """Dice has already been thrown, which is given in diceScore,\
      Make stuff happen after the Dice is thrown:\
         - Go through the map and give students to unis which have\
           campuses on regions which give students corresponding to\
           diceScore.\
         - Implement Roll 7 ThD Rule""";
   g->currentTurn++; //Increases current turn by 1 FIRST
   //ADD STUFF HERE
}

//"""@@@@ Functions which GET data about the game aka GETTERS @@@@"""

// what type of students are produced by the specified region?
// regionID is the index of the region in the newGame arrays (above)
// see discipline codes above
int getDiscipline (Game g, int regionID) {
   return g->map.regions[regionID].disciplineValue;
   //COMPLETED
}

// what dice value produces students in the specified region?
// 2..12
int getDiceValue (Game g, int regionID) {
   return g->map.regions[regionID].diceValue;
   //COMPLETED
}

// return the current turn number of the game -1,0,1, ..
int getTurnNumber (Game g) {
   return g->currentTurn;
   //COMPLETED
}

// return the player id of the player whose turn it is
// the result of this function is NO_ONE during Terra Nullis
int getWhoseTurn (Game g) {
   if (g->currentTurn == -1) {
      return NO_ONE;
   } else {
      return (g->currentTurn + 3) % 3;
   }
   //COMPLETED
}

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
// you can assume that any pths passed in are 0 terminated strings.
int isLegalAction (Game g, action a) {
//HARD
// --- get data about a specified player ---
   return 0;
}

// make the specified action for the current player and update the
// game state accordingly.
// The function may assume that the action requested is legal.
// START_SPINOFF is not a legal action here
void makeAction (Game g, action a) {
//MEDIUM-HARD
/* For easy reference #defined action codes:
   #define PASS 0                #define BUILD_CAMPUS 1
   #define BUILD_GO8 2           #define OBTAIN_ARC 3
   #define START_SPINOFF 4       #define OBTAIN_PUBLICATION 5
   #define OBTAIN_IP_PATENT 6    #define RETRAIN_STUDENTS 7 */
   """Make sure to update Game Data *AFTER* an action is *COMPLETED*\
      perhaps make a function for updating.""";
   """Update mostARCsPlayer""";
   """Update mostPublicationsPlayer""";

}

// return the contents of the given vertex (ie campus code or
// VACANT_VERTEX)
int getCampus(Game g, path pathToVertex) {
//MEDIUM
   """Make this after pathing is done""";
   return 0;
}

// the contents of the given edge (ie ARC code or vacent ARC)
int getARC(Game g, path pathToEdge) {
   """Make this after pathing is done""";
   return 0;
}

// which university currently has the prestige award for the most ARCs?
// this is NO_ONE until the first arc is purchased after the game
// has started.
int getMostARCs (Game g) {
   return g->mostArcsPlayer;
   //COMPLETED
}

// which university currently has the prestige award for the most pubs?
// this is NO_ONE until the first publication is made.
int getMostPublications (Game g) {
   return g->mostPublicationsPlayer;
   //COMPLETED
}

// return the number of ARC grants the specified player currently has
int getARCs (Game g, int player) {
   return g->universities[player].ownedArcCount;
   //COMPLETED
}

// return the number of GO8 campuses the specified player currently has
int getGO8s (Game g, int player) {
//EASY-MEDIUM
   """You will need to make another function that goes through the\
   university's campuses and check to see if they are GO8 or normal""";
   return 0;
}

// return the number of normal Campuses the specified player currently has
int getCampuses (Game g, int player) {
//EASY-MEDIUM
   """You will need to make another function that goes through the\
   university's campuses and check to see if they are GO8 or normal""";
   return 0;
}

// return the number of IP Patents the specified player currently has
int getIPs (Game g, int player) {
   return g->universities[player].patentCount;
   //COMPLETED
}

// return the number of Publications the specified player currently has
int getPublications (Game g, int player) {
   return g->universities[player].publicationCount;
   //COMPLETED
}

// return the number of KPI points the specified player currently has
int getKPIpoints (Game g, int player){
//EASY-MEDIUM
/* For easy reference: #defines used for scoring:
   #define KPI_PER_ARC 2         #define KPI_PER_PATENT 10
   #define KPI_PER_CAMPUS 10     #define KPI_PER_GO8 20
   #define KPI_FOR_MOST_PUBS 10  #define KPI_FOR_MOST_ARCS 10 */
   return 0;
}

// return the number of students of the specified discipline type
// the specified player currently has
int getStudents (Game g, int player, int discipline) {
   int students = 0;
   if (discipline == STUDENT_THD) {
      students = g->universities[player].studentCount.thd;
   } else if (discipline == STUDENT_BPS) {
      students = g->universities[player].studentCount.bps;
   } else if (discipline == STUDENT_BQN) {
      students = g->universities[player].studentCount.bqn;
   } else if (discipline == STUDENT_MJ) {
      students = g->universities[player].studentCount.mj;
   } else if (discipline == STUDENT_MTV) {
      students = g->universities[player].studentCount.mtv;
   } else if (discipline == STUDENT_MMONEY) {
      students = g->universities[player].studentCount.mmoney;
   }
   return students;
   //COMPLETED
}

// return how many students of discipline type disciplineFrom
// the specified player would need to retrain in order to get one
// student of discipline type disciplineTo.  This will depend
// on what retraining centers, if any, they have a campus at.
int getExchangeRate (Game g, int player, int disciplineFrom, int disciplineTo) {
//MEDIUM
   """Can start but will need MAP to finish""";
   return 0;
}

// free all the memory malloced for the game
void disposeGame (Game g) {
   free (g);
}
