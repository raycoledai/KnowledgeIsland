/*#define KPI_PER_ARC 2         #define KPI_PER_PATENT 10
   #define KPI_PER_CAMPUS 10     #define KPI_PER_GO8 20
   #define KPI_FOR_MOST_PUBS 10  #define KPI_FOR_MOST_ARCS 10 
*/


int getKPIpoints (Game g, int player){
	
	int KPICount = 0;
	
	KPICount += KPI_PER_ARC*getARC (g, player);
	
	KPICount += KPI_PER_CAMPUS*getCampuses (g, player);
	
	if (player == g-> mostPublicationsPlayer) {
		KPICount += KPI_FOR_MOST_PUBS;
	}     
	
	KPICount += KPI_PER_PATENT*getIPs (g, player);
	
	KPICount += KPI_PER_GO8*getGO8s (g, player);
	
	if (player == g->mostArcsPlayer) {
		KPICount += KPI_FOR_MOST_ARCS;
	}
	
	return KPICount;
	
}


	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	