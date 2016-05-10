#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

void testCheckPoint ();
int checkPoint (int x, int y);

int main (int argc, char * argv[]) {
    float xCor;
    float yCor;
    
    testCheckPoint();
    
    printf ("insert x and y coordinates (x,y):");
    scanf ("%f,%f", &xCor, &yCor);
    
    int truePoint = checkPoint(xCor, yCor);
    
    if (truePoint == 1) {
        printf("It is in the graph!\n");
    } else {
        printf("It is outside of the graph!\n");
    }
}

void testCheckPoint () {
    assert (checkPoint (0, 1) == 1);
    assert (checkPoint (-4, 1) == 1);
    assert (checkPoint (-5, -1) == 1);
    assert (checkPoint (3, 5) == 1);
    assert (checkPoint (4, -3) == 1);
    assert (checkPoint (1, 7) == 0);
    assert (checkPoint (-2, -7) == 0);
    assert (checkPoint (6, -8) == 0);
    assert (checkPoint (-8, 6) == 0);
}

int checkPoint (int x, int y){
    int point = 0;

    //y-coordinate needs to be odd
    if (y%2 == 1) {
        point = 1;
    } else {
        point = 0;
    }
    
    //max region is 5x5
    if (abs(x)>5 || abs(y)>5) {
        point = 0;
    }
    
    //in the region 3x5, its a point
    if (abs(x) <=3 && abs(y) <=5) {
        point = 1;
    }
    
    //other regions
    if (abs(x) > 3 && abs(x) <= 4) {
        if (abs(y) <= 3) {
            point = 1;
        } else {
            point = 0;
        }
    }
    
    if (abs(x) > 4 && abs(x) <= 5) {
        if (abs(y) <= 1) {
            point = 1;
        } else {
            point = 0;
        }
    }
    return point;
}
