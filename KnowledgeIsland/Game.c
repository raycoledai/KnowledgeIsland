#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include "Game.h"

#define DEFAULT_DISCIPLINES {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV, \
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN, \
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS}
#define DEFAULT_DICE {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5}
#define DEFAULT_TRAININGCENTRES {{-1,5,CENTRE_MMONEY},(0,5,CENTRE_MMONEY),\
                {-4,3,CENTRE_MTV}, {-3,3,CENTRE_MTV}, {4,3,CENTRE_BQN},\
                {4,1,CENTRE_BQN}, {4,-1,CENTRE_MJ}, {4,-3,CENTRE_MJ},\
                {-1,-5,CENTRE_BPS}, {0,-5,CENTRE_BPS}}

//Boundaries
#define NUM_VERTICES 54
#define NUM_EDGES 72
#define VERTICES_PER_REGION 6
#define MAX_OWNED_CAMPUSES 27
#define MAX_OWNED_ARCS 72
#define TYPES_OF_STUDENTS 6

//KPI Point System
#define KPI_PER_ARC 2
#define KPI_PER_CAMPUS 10
#define KPI_PER_GO8 20
#define KPI_PER_PATENT 10
#define KPI_FOR_MOST_PUBS 10
#define KPI_FOR_MOST_ARCS 10

//trainingCentre IDs
#define NO_CENTRE 0
#define CENTRE_BPS 1
#define CENTRE_BQN 2
#define CENTRE_MJ  3
#define CENTRE_MTV 4
#define CENTRE_MMONEY 5

#define DEFAULT_EXCHANGE_RATE 3
#define TRAINING_CENTRE_RATE 2

//Starting students allocated
#define START_NUM_THD 0
#define START_NUM_BPS 3
#define START_NUM_BQN 3
#define START_NUM_MJ 1
#define START_NUM_MTV 1
#define START_NUM_MMONEY 1
#define START_NUM_PUBLICATIONS 0
#define START_NUM_PATENTS 0

//Hexagon side type
#define SIDE_VERTICAL 0 //"|"
#define SIDE_FORWARDSLASH 1 //"/"
#define SIDE_BACKSLASH 2    //"\"

//Path direction
#define SOUTH 1
#define NORTH -1

//Conversion
#define UPGRADE 3

typedef struct _Point {
   //a point struct that can store (x,y) co-ordinates
   int x;
   int y;
} Point;

typedef struct _Region {
    Point location; //(x,y) coord of the centre of the region
    int regionID; //which region it is (indexed form 0-19)
    int disciplineValue; //what students it produces
    int diceValue; //what dice value is required to produce students
} Region;

typedef struct _Edge {
   Point point0; //the first (x,y) coords of the point the edge connects from
   Point point1; //the second (x,y) coords of the point the edge connects to
   int edgeType; //whether it is forwardslash/backslash/vertical
   int isOwned; //whether someone owns an ARC on it or not
} Edge;

typedef struct _Vertex {
   Point location; //(x,y) coord of the vertex
   Point regions[3]; //which regions (up to three) surround it
   int retrainingCentre; //whether it has a retraining centre on it
   int isOwned; //whether someone owns a campus on it or not
} Vertex;

typedef struct _Map {
   Region regions[NUM_REGIONS]; //an array of Regions, NUM_REGIONS long
   Edge edges[NUM_EDGES]; //an array of Edges, NUM_EDGES long
   Vertex vertices[NUM_VERTICES]; // an array of Vertices, NUM_VERTICES long
} Map;

typedef struct _university {
   int playerId;
   int studentCount[TYPES_OF_STUDENTS]; //The number of students they have per degree
   int publicationCount;
   int patentCount;
   int ownedCampusCount;
   Vertex ownedCampuses[MAX_OWNED_CAMPUSES]; //The vertices of campuses they own
   int ownedArcCount;
   Edge ownedArcs[MAX_OWNED_ARCS]; //The edges of ARCs they own
} uni;

typedef struct _game {
   int currentTurn;
   Map map;
   uni unis[NUM_UNIS]; //an array of unis, NUM_UNIS long
   int totalGo8CampusCount;
   int mostPublications;
   int mostPublicationsPlayer;
   int mostArcs;
   int mostArcsPlayer;
} game;

void initUniversity(uni* u, int player);
void initMap (Map* map, int discipline[], int dice[]);
void initRegions (Region* r, int discipline[], int dice[]);
void initEdges (Edge* e);
int findEdgeType (int x, int y);
void initVertices (Vertex* v);
void addRegions(Point* regions, int x, int y);
int isRegion (int x, int y);
int checkPoint (int x, int y);
Point getVertexLocation(Point location, int vertexNum);
Point travelPath(Map* m, path* p);
int swapFacing (int facing);
//int checkPath (Game g, path p);
void initGame(Game game, int discipline[], int dice[]);

Game newGame(int discipline[], int dice[]) {
   //printf (sizeof(game)*3);
   Game g = malloc(sizeof(game)*sizeof(game)); //allocate memory to the game
   assert(g != NULL);
   initGame (g, discipline, dice); //generate initial state of the game
   return g;
}

void initGame(Game g, int discipline[], int dice[]) {
   //initialise the game.
   g->currentTurn = -1;
   g->totalGo8CampusCount = 0;
   g->mostPublications = 0;
   g->mostPublicationsPlayer = NO_ONE;
   g->mostArcs = 0;
   g->mostArcsPlayer = NO_ONE;
   initMap(&g->map, discipline, dice); //initialise the map
   initUniversity(&g->unis[0], UNI_A); //initialise UNI_A
   initUniversity(&g->unis[1], UNI_B); //initialise UNI_B
   initUniversity(&g->unis[2], UNI_C); //initialise UNI_C
}

void initMap (Map* map, int discipline[], int dice[]) {
   //Generate the map
   initRegions (map->regions, discipline, dice); //Generate the regions
   initEdges (map->edges); //Generate the edges
   initVertices (map->vertices); //Generate the vertices
}

void initRegions (Region* r, int discipline[], int dice[]) {
   //Generates all 19 regions in the map
   int regionNum = 0;
   int realignNum = 0;
   int realignRegionNum[NUM_REGIONS] = {7,12,16,3,8,13,17,0,4,9,14,18,
   /* realign to our interpretation of the map*/    1,5,10,15,2,6,11};
   int x = -2;
   int y = 4;
   int end_x = x*(-1);
   Point location; //a point struct we can change
   while (y >= -4) {
      //Loops through each region row in the map
      location.y = y;
      while (x <= end_x) {
         //Loops through each region in the row
         location.x = x; //set location.x to the current value of x
         realignNum = realignRegionNum[regionNum];
         r[realignNum].location = location; //gives the current value of location.x&location.y to the region
         r[realignNum].regionID = realignNum; //gives the regionID from 0-18 based on map
         r[realignNum].disciplineValue = discipline[realignNum]; //gives the discipline stored in region
         r[realignNum].diceValue = dice[realignNum]; //gives the dice value needed to activate region
/*         printf("Region %d, (%d,%d), deg: %d, dice: %d\n", r[realignNum].regionID,
      r[realignNum].location.x, r[realignNum].location.y, r[realignNum].disciplineValue,
      r[realignNum].diceValue);*/
         x = x+2;
         regionNum++;
      }
      //Set x for the next row
      if (y==4||y==0) {
         x = -3;
      } else if (y==2) {
         x = -4;
      } else if (y==-2) {
         x = -2;
      }
      end_x = x*(-1);
      y = y-2; //go to the next region row
   }
}

void initEdges (Edge* e) {
   //Generates all the edges in the map
   int edgeNum = 0;
   int edgeType; //Vertical, backslash or forwardslash
   int x = -3;
   int y = 5;
   int end_x = x*(-1);
   Point location; //a point struct we can change
   while (y >= -5) {
      //Loops through each row in the map
      location.y = y;
      if (y%2== 1||y%2==-1) {   //If it is a horizontal edge
         while (x < end_x) {
            //Loops through each horizontal edge in the row
            location.x = x;
            edgeType = findEdgeType(x,y); //returns whether it is a backslash or forward slash edge
            e[edgeNum].point0 = location; //gives the current value of location.x&location.y to point0
            location.x = x+1; //the second point of the edge is 1 to the right of point0
            e[edgeNum].point1 = location; //point1 is second point of the edge
            e[edgeNum].edgeType = edgeType; //black/fowardslash/vertical
            e[edgeNum].isOwned = VACANT_ARC; //who owns the edge
            x++; //go to the next point
/*           printf("Edge (%d,%d), (%d,%d), type: %d\n",
   e[edgeNum].point0.x, e[edgeNum].point0.y,
   e[edgeNum].point1.x, e[edgeNum].point1.y,
   e[edgeNum].edgeType); */

         }
      } else {
         while (x <= end_x) {
            //Loops through each vertical edge in the row
            location.x = x;
            location.y = y+1; //Since you look at the middle of the edge
            //(vertical edges are 2 units high) the y values +-1 are the
            //two points which the edge connects.
            edgeType = SIDE_VERTICAL;
            e[edgeNum].point0 = location; //gives the current value of location.x&location.y to point0
            location.y = y-1; //the second point is below the middle of the vertical edge
            e[edgeNum].point1 = location; //point1 is second point of the edge
            e[edgeNum].edgeType = edgeType; //Sets the edgetype, (set as SIDE_VERTICAL above)
            e[edgeNum].isOwned = VACANT_ARC;
            x = x+2; //go to next Vertical edge
/*           printf("Edge (%d,%d), (%d,%d), type: %d\n",
   e[edgeNum].point0.x, e[edgeNum].point0.y,
   e[edgeNum].point1.x, e[edgeNum].point1.y,
   e[edgeNum].edgeType); */

         }
      }
      //Set x for the next row
      if (y==5||y==-3||y==-4) {
         x = -3;
      } else if (y==4||y==3||y==-1||y==-2) {
         x = -4;
      } else if (y==2||y==1||y==0) {
         x = -5;
      }
      end_x = x*(-1);
      y--; //go to the next row
   }
}

int findEdgeType (int x, int y) {
   //Finds out whether the edge is a forwardslash one or a backslash one
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
   //Generates all the vertices of the map
   int vertexNum = 0;
   int x = -3;
   int y = 5;
   int end_x = x*(-1);
   Point location; //a point struct we can change
   while (y >= -5) {
      //Loops through each row in the map
      location.y = y;
      while (x <= end_x) {
         //Loops thorugh each point in the row
         location.x = x;
         v[vertexNum].location = location; //gives the current value of location.x&location.y to the vertex
         v[vertexNum].retrainingCentre = NO_CENTRE; //whether it has a retrainingCentre or not
         v[vertexNum].isOwned = VACANT_VERTEX; //whether a uni has built a campus on it or not
         addRegions(v[vertexNum].regions, x, y); //Adds what regions surround it
         x++; //go to next point
/*         printf("Vertex: (%d,%d), R1:(%d,%d), R2:(%d, %d), R3: (%d, %d)\n",
         v[vertexNum].location.x, v[vertexNum].location.y,
         v[vertexNum].regions[0].x, v[vertexNum].regions[0].y,
         v[vertexNum].regions[1].x, v[vertexNum].regions[1].y,
         v[vertexNum].regions[2].x, v[vertexNum].regions[2].y); */
         vertexNum++;
      }
      //Sets the x for the next row
      if (y==5||y==-1) {
         x = -4;
      } else if (y==3||y==1) {
         x = -5;
      } else if (y==-3) {
         x = -3;
      }
      end_x = x*(-1);
      y = y-2; //go to next row
   }
}

void addRegions(Point* regions, int x, int y) {
   //Gives the vertice what regions are surrounding it by checking whether
   //they are a region point and whether or not it's on the map.
   int regionCount = 0;
   Point location;
   //Check the points that are top-left, above, top-right, bottom-left,
   //below, and bottom-right.
   if (isRegion(x-1, y+1) && checkPoint (x-1, y+1) && regionCount<3) {
      location.x = x-1;
      location.y = y+1;
      regions[regionCount] = location;
      regionCount++;
   }
   if (isRegion(x, y+1) && checkPoint (x, y+1) && regionCount<3) {
      location.x = x;
      location.y = y+1;
      regions[regionCount] = location;
      regionCount++;
   }
   if (isRegion(x+1,y+1) && checkPoint (x+1, y+1) && regionCount<3) {
      location.x = x+1;
      location.y = y+1;
      regions[regionCount] = location;
      regionCount++;
   }
   if (isRegion(x-1,y-1) && checkPoint (x-1, y-1) && regionCount<3) {
      location.x = x-1;
      location.y = y-1;
      regions[regionCount] = location;
      regionCount++;
   }
   if (isRegion(x,y-1) && checkPoint (x, y-1) && regionCount<3) {
      location.x = x;
      location.y = y-1;
      regions[regionCount] = location;
      regionCount++;
   }
   if (isRegion(x+1,y-1) && checkPoint (x+1, y-1) && regionCount<3) {
      location.x = x+1;
      location.y = y-1;
      regions[regionCount] = location;
      regionCount++;
   }
   if (regionCount == 1) {
      //If the point only has one region surrounding it, set the second
      //region as a dummy region
      location.x = 10;
      location.y = 10;
      regions[regionCount] = location;
      regionCount++;
   }
   if (regionCount == 2) {
      //If the point only has two regions surrounding it, set the third
      //region as a dummy region
      location.x = 10;
      location.y = 10;
      regions[regionCount] = location;
      regionCount++;
   }
}

int isRegion (int x, int y) {
   //Checks to see if the point is a region coord
   int result = FALSE;
   if (((x%2==1||x%2==-1) && (y%4==2||y%4==-2)) || (x%2==0 && y%4==0)) {
      result = TRUE;
   }
   return result;
}

int checkPoint (int x, int y) {
   //Checks to see if the point is on the map or on the sea
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

void initUniversity(uni* u, int player) {
   //Initialise the unis
   u->playerId = player; //uniID, which is 1,2 or 3
   //Give the unis their starting students
   u->studentCount[STUDENT_THD] = START_NUM_THD;
   u->studentCount[STUDENT_BPS] = START_NUM_BPS;
   u->studentCount[STUDENT_BQN] = START_NUM_BQN;
   u->studentCount[STUDENT_MJ] = START_NUM_MJ;
   u->studentCount[STUDENT_MTV] = START_NUM_MTV;
   u->studentCount[STUDENT_MMONEY] = START_NUM_MMONEY;
   u->publicationCount = START_NUM_PUBLICATIONS;
   u->patentCount = START_NUM_PATENTS;
   u->ownedCampusCount = 2;
   //u->ownedCampuses = malloc(MAX_OWNED_CAMPUSES);
   u->ownedArcCount = 0;
   //u->ownedArcs = malloc(MAX_OWNED_ARCS);
}

// advance the game to the next turn,
// assuming that the dice has just been rolled and produced diceScore
// the game starts in turn -1 (we call this state "Terra Nullis") and
// moves to turn 0 as soon as the first dice is thrown.
void throwDice (Game g, int diceScore) {
//MEDIUM
/*   """MISLEADING FUNCTION NAME-NOT ACTUALLY THROWING DICE""";
   """Dice has already been thrown, which is given in diceScore,\
      Make stuff happen after the Dice is thrown:\
         - Go through the map and give students to unis which have\
           campuses on regions which give students corresponding to\
           diceScore.\
         - Implement Roll 7 ThD Rule""";*/
   g->currentTurn++; //Increases current turn by 1 FIRST
   //int regionNum = 0;
   int uniID;
   int regionNum = 0;
   int discipline = g->map.regions[regionNum].disciplineValue;
   int vertexNum;
   int increaseAmount;
   while (regionNum < NUM_REGIONS) {
      if (g->map.regions[regionNum].diceValue == diceScore) {
         discipline = g->map.regions[regionNum].disciplineValue;
         vertexNum = 0;
         Point regionLocation = g->map.regions[regionNum].location;
         Point location;
         while (vertexNum < VERTICES_PER_REGION) {
            location = getVertexLocation(location, vertexNum);
            int vOwned = getCampus(&g, getPath(&g->map, location));
            if (vOwned) {
               uniID = (vOwned -1) %3;
               increaseAmount = 1;
               if (vOwned > 3) {
                  increaseAmount = 2;
               }
               if (discipline == STUDENT_THD) {
                  g->unis[uniID].studentCount[STUDENT_THD] += increaseAmount;
               } else if (discipline == STUDENT_BPS) {
                  g->unis[uniID].studentCount[STUDENT_BPS] += increaseAmount;
               } else if (discipline == STUDENT_BQN) {
                  g->unis[uniID].studentCount[STUDENT_BQN] += increaseAmount;
               } else if (discipline == STUDENT_MJ) {
                  g->unis[uniID].studentCount[STUDENT_MJ] += increaseAmount;
               } else if (discipline == STUDENT_MTV) {
                  g->unis[uniID].studentCount[STUDENT_MTV] += increaseAmount;
               } else if (discipline == STUDENT_MMONEY) {
                  g->unis[uniID].studentCount[STUDENT_MMONEY] += increaseAmount;
               }
            }
            vertexNum++;
         }
      }
      regionNum++;
   }
   if (diceScore == 7) {
      uniID = 0;
      while (uniID < NUM_UNIS) {
         uni* u = &g->unis[uniID];
         u->studentCount[STUDENT_THD] += u->studentCount[STUDENT_MMONEY];
         u->studentCount[STUDENT_THD] += u->studentCount[STUDENT_MTV];
         u->studentCount[STUDENT_MMONEY] = 0;
         u->studentCount[STUDENT_MTV] = 0;
         uniID++;
      }
   }
}

Point getVertexLocation(Point location, int vertexNum) {
   if (vertexNum == 0) {
      location.x --;
      location.y ++;
   } else if (vertexNum == 1) {
      location.y ++;
   } else if (vertexNum == 2) {
      location.x ++;
      location.y ++;
   } else if (vertexNum == 3) {
      location.x --;
      location.y --;
   } else if (vertexNum == 4) {
      location.y --;
   } else if (vertexNum == 5) {
      location.x ++;
      location.y --;
   }
   return location;
}

Point travelPath (Map* m, path* p) {
   int facing = SOUTH;
   Point currentPoint;
   Point previousPoint;
   previousPoint.x = -4;
   previousPoint.y = 5;
   currentPoint.x = -3;
   currentPoint.y = 5;
   int edgeType = SIDE_BACKSLASH;
   assert (edgeType != -1);
   int i = 1;
   char direction;
   while (i < strlen(p)) {
      previousPoint = currentPoint;
      direction = p[i];
      if (edgeType == SIDE_VERTICAL) {
         if (direction == 'L') {
            currentPoint.x = currentPoint.x + (1 * facing);
         } else if (direction == 'R') {
            currentPoint.x = currentPoint.x + (-1 * facing);
         } else if (direction == 'B') {
            if (facing == SOUTH) {
               currentPoint.y ++;
            } else {
               currentPoint.y --;
            }
            facing = swapFacing(facing);
         }
      } else if (edgeType == SIDE_FORWARDSLASH) {
         if (direction == 'L') {

         } else if (direction == 'R') {

         } else if (direction == 'B') {
            if (facing == SOUTH) {
               currentPoint.x ++;
            } else {
               currentPoint.x --;
            }
            facing = swapFacing(facing);
         }
      } else if (edgeType == SIDE_BACKSLASH) {
         if (direction == 'L') {
            currentPoint.y ++;
         } else if (direction == 'R') {
            currentPoint.x + (-1*facing);
         } else if (direction == 'B') {
            currentPoint.x = currentPoint.x + (-1 * facing);
            facing = swapFacing(facing);
         }
      }
      getEdgeType(m, currentPoint, previousPoint);
      i++;
   }
   return p;
}

int swapFacing (int facing) {
   if (facing == SOUTH) {
      facing = NORTH;
   } else {
      facing = SOUTH;
   }
   return facing;
}

int getEdgeType(Map* m, Point p) {
   int i = 0;
   while (i < NUM_VERTICES) {
      if (m->edges[i].point0 == p )
         return m->edges[i].edgeType;
      i++;
   }
   return -1;
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
      return ((g->currentTurn + 3) % 3)+1;
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
//     discipline numbers, and that the u has sufficient
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
//   """Make sure to update Game Data *AFTER* an action is *COMPLETED*
//      perhaps make a function for updating.""";
//   """Update mostARCsPlayer""";
//   """Update mostPublicationsPlayer""";
   if (a->actionCode == PASS){
      //throwDice....
   }else if (a->actionCode == BUILD_CAMPUS){
      //define vertex
      //vertex->location = a.destination
      //if getWhoseTurn == UNI_A
         //vertex->isOwned = CAMPUS_A etc..
      //g->unis[player].ownedCampusCount++;
      //g->unis[player].ownedCampuses append a.destination
      //Increase KPICount for the uni (is this needed?)
   }else if (a->actionCode == BUILD_GO8){
      //same as above but with GO8 instead of CAMPUS
   }else if (a->actionCode == OBTAIN_ARC){
      //EDGEs instead of VERTEXes
      //Also include mostARCsPlayer
   }else if (a->actionCode == START_SPINOFF){
      //According to Game.h, this is not a legal action?
   }else if (a->actionCode == OBTAIN_PUBLICATION){
      //g->unis[player].publicationCount++;
      //Include mostPublicationsPlayer
   }else if (a->actionCode == OBTAIN_IP_PATENT){
      //g->unis[player].patentCount++;
   }else if (a->actionCode == RETRAIN_STUDENTS){
      //Using exchange rate exchange students
      //depends on getWhoseTurn
   }
   //updateGame(Game g, action a); is this needed?
}

// return the contents of the given vertex (ie campus code or
// VACANT_VERTEX)
int getCampus(Game g, path pathToVertex) {
//MEDIUM
//   """Make this after pathing is done""";
   return 0;
}

// the contents of the given edge (ie ARC code or vacent ARC)
int getARC(Game g, path pathToEdge) {
//   """Make this after pathing is done""";
   return 0;
}

/*int checkPath (Game g, path p) {

}*/

// which u currently has the prestige award for the most ARCs?
// this is NO_ONE until the first arc is purchased after the game
// has started.
int getMostARCs (Game g) {
   return g->mostArcsPlayer;
   //COMPLETED
}

// which u currently has the prestige award for the most pubs?
// this is NO_ONE until the first publication is made.
int getMostPublications (Game g) {
   return g->mostPublicationsPlayer;
   //COMPLETED
}

// return the number of ARC grants the specified player currently has
int getARCs (Game g, int player) {
   return g->unis[player].ownedArcCount;
   //COMPLETED
}

// return the number of GO8 campuses the specified player currently has
int getGO8s (Game g, int player) {
//MEDIUM
   int i = 0;
   int GO8CampusCount = 0;
   int playerCampusCount = g->unis[player].ownedCampusCount;
   while(i < playerCampusCount) {
      if (player == g->unis[player].ownedCampuses[i].isOwned - UPGRADE) {
         GO8CampusCount++;
      }
      i++;
   }
   return GO8CampusCount;
}

// return the number of normal Campuses the specified player currently has
int getCampuses (Game g, int player) {
//MEDIUM
   int i = 0;
   int normalCampusCount = 0;
   int playerCampusCount = g->unis[player].ownedCampusCount;
   while(i < playerCampusCount) {
      if (player == g->unis[player].ownedCampuses[i].isOwned) {
         normalCampusCount++;
      }
      i++;
   }
   return normalCampusCount;
}

// return the number of IP Patents the specified player currently has
int getIPs (Game g, int player) {
   return g->unis[player].patentCount;
   //COMPLETED
}

// return the number of Publications the specified player currently has
int getPublications (Game g, int player) {
   return g->unis[player].publicationCount;
   //COMPLETED
}

// return the number of KPI points the specified player currently has
int getKPIpoints (Game g, int player){
//EASY-MEDIUM
   int KPICount = 0;
   KPICount += KPI_PER_ARC * getARC (g, player);
   KPICount += KPI_PER_CAMPUS * getCampuses (g, player);
   KPICount += KPI_PER_GO8 * getGO8s (g, player);
   KPICount += KPI_PER_PATENT * getIPs (g, player);
   if (player == g-> mostPublicationsPlayer) {
      KPICount += KPI_FOR_MOST_PUBS;
   }
   if (player == g->mostArcsPlayer) {
      KPICount += KPI_FOR_MOST_ARCS;
   }
   return KPICount;
//COMPLETED
}

// return the number of students of the specified discipline type
// the specified player currently has
int getStudents (Game g, int player, int discipline) {
   int students = 0;
   if (discipline == STUDENT_THD) {
      students = g->unis[player].studentCount[STUDENT_THD];
   } else if (discipline == STUDENT_BPS) {
      students = g->unis[player].studentCount[STUDENT_BPS];
   } else if (discipline == STUDENT_BQN) {
      students = g->unis[player].studentCount[STUDENT_BQN];
   } else if (discipline == STUDENT_MJ) {
      students = g->unis[player].studentCount[STUDENT_MJ];
   } else if (discipline == STUDENT_MTV) {
      students = g->unis[player].studentCount[STUDENT_MTV];
   } else if (discipline == STUDENT_MMONEY) {
      students = g->unis[player].studentCount[STUDENT_MMONEY];
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
   int rate = DEFAULT_EXCHANGE_RATE;
   int totalcampuses = g->unis[player].ownedCampusCount;
   int centre;
   int i = 0;
   while (i < totalcampuses) {
      centre = g->unis[player].ownedCampuses[i].retrainingCentre;
      if (disciplineFrom == centre) {
         rate = TRAINING_CENTRE_RATE;
      }
      i++;
   }
   return rate;
   //COMPLETED
}

// free all the memory malloced for the game
void disposeGame (Game g) {
   int regionNum = 0;
   while (regionNum < NUM_REGIONS) {
      (void) g->map.regions[regionNum];
      regionNum++;
   }
   int edgeNum = 0;
   while (edgeNum < NUM_EDGES) {
      (void) g->map.edges[edgeNum];
      edgeNum++;
   }
   int vertexNum = 0;
   while (vertexNum < NUM_VERTICES) {
      (void) g->map.vertices[vertexNum];
      vertexNum++;
   }

   int uniID = 0;
   while (uniID < NUM_UNIS) {
      free(&g->unis[uniID].ownedCampuses);
      free(&g->unis[uniID].ownedArcs);
      uniID++;
   }
   free (g);
}
