#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#define TRUE 1
#define FLASE 0

void testCheckPoint ();
int checkPoint (int x, int y);

int main (int argc, char * argv[]) {
    float xCor;
    float yCor;
    
    testCheckPoint();
    
    printf ("insert x and y coordinates (x,y):");
    scanf ("%f,%f", &xCor, &yCor);
    
    int truePoint = checkPoint(xCor, yCor);
    
    if (truePoint == TRUE) {
        printf("It is in the graph!\n");
    } else {
        printf("It is outside of the graph!\n");
    }
}

void testCheckPoint () {
    assert (checkPoint (0, 1) == TRUE);
    assert (checkPoint (-4, 1) == TRUE);
    assert (checkPoint (-5, -1) == TRUE);
    assert (checkPoint (3, 5) == TRUE);
    assert (checkPoint (4, -3) == TRUE);
    assert (checkPoint (1, 7) == FALSE);
    assert (checkPoint (-2, -7) == FALSE);
    assert (checkPoint (6, -8) == FALSE);
    assert (checkPoint (-8, 6) == FALSE);
}

int checkPoint (int x, int y){
    int point = FALSE;

    //y-coordinate needs to be odd
    if (y%2 == 1) {
        point = TRUE;
    } else {
        point = FALSE;
    }
    
    //max region is 5x5
    if (abs(x)>5 || abs(y)>5) {
        point = FALSE;
    }
    
    //in the region 3x5, its a point
    if (abs(x) <=3 && abs(y) <=5) {
        point = TRUE;
    }
    
    //other regions
    if (abs(x) > 3 && abs(x) <= 4) {
        if (abs(y) <= 3) {
            point = TRUE;
        } else {
            point = FALSE;
        }
    }
    
    if (abs(x) > 4 && abs(x) <= 5) {
        if (abs(y) <= 1) {
            point = TRUE;
        } else {
            point = FALSE;
        }
    }
    return point;
}
