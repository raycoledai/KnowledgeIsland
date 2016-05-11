#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#define DEFAULT_EXCHANGE_RATE 3
#define TRAINING_CENTRE_RATE 2


// return how many students of discipline type disciplineFrom
// the specified player would need to retrain in order to get one
// student of discipline type disciplineTo.  This will depend
// on what retraining centers, if any, they have a campus at.
int getExchangeRate (Game g, int player, int disciplineFrom, int disciplineTo) {
    //MEDIUM
    """If a university has a campus on a retraining centre, then\
    it only takes 2 students of type of retraining centre, to train\
    into any other type of student."""

    int i = 0;
    int rate = DEFAULT_EXCHANGE_RATE;
    int totalcampuses = g->universities[player].ownedCampusCount;
    int centre;
    
    while (i < totalcampuses) {
        centre = g->universities[player].ownedCampuses[i].retrainingCentre;
        if (disciplineFrom == centre) {
            rate = TRAINING_CENTRE_RATE;
        }
        i++;
    }
    
    return rate;
}

