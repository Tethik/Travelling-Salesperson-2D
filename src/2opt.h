#ifndef _2OPT
#define _2OPT
#include "localsearch.h"
#include "types.h"
#include "map.h"
#include <ctime>
#include <vector>

#define USE_ADJACENCY
#define ADJACENCY_LIST_SIZE 128

class TwoOpt : public LocalSearch
{	
	protected: 
		double* adjacencyThresholds;
		std::vector<std::vector<int>*> adjacencyLists;
		
		
		void swap(tour* t, int from, int to, double cost);
		// what is the cost if we were to swap?
		double getNewCost(tour* t, int from, int to);
		bool findBestNewTour(tour* t, std::clock_t start);
		bool findNewTour(tour* t, std::clock_t start);
		void setAdjacencyThresholds();
	public:
		bool findBest;
		TwoOpt(Map* map);
		virtual tour* getBetterTour(tour*, std::clock_t);

};


#endif
