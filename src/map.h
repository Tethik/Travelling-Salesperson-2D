
#ifndef _MAP
#include <vector>
#include "types.h"

class Map
{
		double** distance_mat;	
		// komplement till distansmatris.
		int** index_mat;	
		int dim;
		double calculateDistance(coordinate*, coordinate*);
		void setDistanceMatrix(std::vector<coordinate*>* cities);
		void setAdjacencyThresholds();
		bool distCompare(int,int);
		
	public:
		Map (std::vector<coordinate*>*);
		~Map();
		std::vector<std::vector<int>*> adjacencyLists;
		int getDimension();
		double getDistance(int, int);
		//~ coordinate* getCityCoordinate(int);
		double getTourDistance(std::vector<int>&);
		double getTourDistance(tour* t);
		double* getDistanceRow(int);


};

#define _MAP
#endif
