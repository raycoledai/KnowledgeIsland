#define GO8_VALUE 3


// return the number of GO8 campuses the specified player currently has
int getGO8s (Game g, int player) {
//MEDIUM
   """You will need to go through the\
   university's campuses and check to see if they are GO8 or normal""";

   int i = 0;
   int GO8CampusCount = 0;
   int playerCampusCount = g->unis[player].ownedCampusCount;
   while(i < playerCampusCount) {
      if (player == g->unis[player].ownedCampuses[i].isOwned - GO8_VALUE) {
         GO8CampusCount++;
      }
      i++;
   }

   return GO8CampusCount;

}

// return the number of normal Campuses the specified player currently has
int getCampuses (Game g, int player) {
//MEDIUM
   """You will need to go through the\
   university's campuses and check to see if they are GO8 or normal""";

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
