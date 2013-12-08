#include "2opt.h"
#include <vector>
#include "map.h"
#include <iostream>
#include <ctime>
#include <algorithm>


using namespace std;

TwoOpt::TwoOpt(Map* map) {
	this->map = map;	
	
	#ifdef USE_ADJACENCY	
	setAdjacencyThresholds();
	#endif
}

void TwoOpt::setAdjacencyThresholds() {
	// Adjancency lists...
	vector<double> distances; 
	int dim = map->getDimension();
	int pos = (dim > ADJACENCY_LIST_SIZE) ?  ADJACENCY_LIST_SIZE - 2 : dim - 2;	
	//~ this->adjacencyThresholds = new double[dim];
	
	for(int i = 0; i < dim; ++i) {	
		vector<int>* neighbours = new vector<int>;
		for(int j = 0; j < dim; ++j) {
			if(i == j) continue;
			distances.push_back(map->getDistance(i,j));
		}
		
		sort(distances.begin(), distances.end());		
		
		double dist = distances.at(pos);
		//~ this->adjacencyThresholds[i] = ;		
		
		for(int j = 0; j < dim; ++j) {
			if(i == j) continue;
			if(map->getDistance(i,j) <= dist) //this->adjacencyThresholds[i])				
				neighbours->push_back(j);
		}
		//~ cout << neighbours->size() << endl;
		
		
		this->adjacencyLists.push_back( neighbours );
		distances.clear();
	}
}

// From wikipedia psuedocode:
// http://en.wikipedia.org/wiki/2-opt
void TwoOpt::swap(tour* t, int from, int to, double cost)
{	
	int toswap = (to - from) / 2 + (to - from) % 2; 	
	// 1. take route[0] to route[i-1] and add them in order to new_route
	// 2. take route[i] to route[k] and add them in reverse order to new_route
	// 3. take route[k+1] to end and add them in order to new_route
	for(int i = 0; i < toswap; ++i) {		
		int left = from+i;
		int right = to-i;						
		int tmp = t->path[left];		
		t->path[left] = t->path[right];
		t->reverse[t->path[right]] = left;
		t->path[right] = tmp;
		t->reverse[tmp] = right;
	}
	
	t->cost += cost;		
}


double TwoOpt::getNewCost(tour* t, int i1, int i2) {
	int e1 = t->path[i1];
	int s2 = t->path[i2];
	
	int lastIndex = t->path.size()-1;	
	int s1 = i1 > 0 ? t->path[i1-1] : t->path[lastIndex];	
	int e2 = i2 < lastIndex ? t->path[i2+1] : t->path[0];
 
	double cost = 0;	
	cost -= map->getDistance(e1, s1);		
	cost -= map->getDistance(s2, e2);		
	cost += map->getDistance(e1, e2);	
	cost += map->getDistance(s2, s1);
	
	return cost;
}

bool TwoOpt::findNewTour(tour* t, std::clock_t deadline) {		
	unsigned int size = t->path.size();
	bool changed = false;
	for(unsigned int i = 1; i < size && std::clock() < deadline; ++i)
	{	
		//~ #ifdef USE_ADJACENCY
		//~ int city = t->path[i];
		//~ double maxAllowedDist = adjacencyThresholds[city];	
		//~ #endif
		
		vector<int>* adjacent = this->adjacencyLists[t->path[i]];
		for(unsigned int c = 0; c < adjacent->size(); ++c)
		{		
			int city = adjacent->at(c);
			int j = t->reverse[city];
			
			if(j == 0)
				continue;
			
		//~ 
		//~ for(unsigned int j = i+1; j < size; ++j)
		//~ {		
			//~ #ifdef USE_ADJACENCY	
			//~ if(maxAllowedDist < map->getDistance(city,t->path[j]))
				//~ continue;
			//~ #endif
				
			double cost = (i < j) ? getNewCost(t, i, j) : getNewCost(t, j, i);
			// Fix for rounding errors. #fulhack
			if(cost > -0.00001)
				cost = 0;
			
			if(cost < 0) {					
				if(i < j)
					swap(t, i, j, cost);
				else
					swap(t, j, i, cost);
					
				changed = true;					
			}		
		}	
	}	

	return changed;
}

bool TwoOpt::findBestNewTour(tour* t, std::clock_t deadline) {		
	double bestcost = 0;
	int besti = -1;
	int bestj = -1;
	unsigned int size = t->path.size();
	for(unsigned int i = 1; i < size && std::clock() < deadline; ++i)
	{	
		//~ #ifdef USE_ADJACENCY
		//~ int city = t->path[i];
		//~ double maxAllowedDist = adjacencyThresholds[city];
		//~ #endif
		//~ 
		//~ for(unsigned int j = i+1; j < size; ++j)
		//~ {			
			//~ #ifdef USE_ADJACENCY	
			//~ if(maxAllowedDist < map->getDistance(city,t->path[j]))
				//~ continue;
			//~ #endif
		vector<int>* adjacent = this->adjacencyLists[t->path[i]];
		for(unsigned int c = 0; c < adjacent->size(); ++c)
		{		
			int city = adjacent->at(c);
			int j = t->reverse[city];
			
			if(j == 0)
				continue;
			
			double cost = (i < j) ? getNewCost(t, i, j) : getNewCost(t, j, i);
			// Fix for rounding errors. #fulhack
			if(cost > -0.00001)
				cost = 0;
			
			if(cost < bestcost) {									
				bestcost = cost;
				besti = i;
				bestj = j;	
			}		
		}	
	}	

	if(besti < 0)
		return false;
	
	if(besti < bestj)
		swap(t, besti, bestj, bestcost);
	else
		swap(t, bestj, besti, bestcost);
	
	return true;
}

tour* TwoOpt::getBetterTour(tour* t, std::clock_t deadline)
{	
	int i = 0;	
	bool timedout = false;
	bool (TwoOpt::*twoOptfunc) (tour*, clock_t) = (TwoOpt::findBest) ? &TwoOpt::findBestNewTour : &TwoOpt::findNewTour;
	
	for(int i = 0; t->reverse.size() < t->path.size(); ++i) {
		t->reverse.push_back(0);
	}
	
	for(int i = 0; i < t->path.size(); ++i) {
		t->reverse[t->path[i]] = i;
	}
	
	
	while(!timedout){
		i++;
		bool done = !(this->*twoOptfunc)(t, deadline);		
		if(done)
			break;
		timedout = std::clock() > deadline;
		//~ cout << t->cost << endl;
		
	}	
	//~ cout << "Attempts: " << i << " " << ((timedout) ? "true" : "false") << endl;
	
	return t;	
}


