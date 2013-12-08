#include "simulatedannealing.h"
#include <stdlib.h> // rand
#include <vector>

using namespace std;

SimulatedAnnealing::SimulatedAnnealing(Map* map) : TwoOpt(map) {
	this->map = map;
}

tour* SimulatedAnnealing::cloneTour(tour* original) {
	tour* newTour = new tour();
	newTour->path = original->path;
	newTour->cost = original->cost;
	newTour->reverse = original->reverse;
	return newTour;
}

tour* SimulatedAnnealing::getBetterTour(tour* t, std::clock_t deadline) {
	
	tour* best_so_far = cloneTour(t);
	
	while(std::clock() < deadline) {
			
		tour* curr_tour = cloneTour(best_so_far);
		//~ cout << &curr_tour->path << " " << &best_so_far->path << endl;
		// Do some bad modification
		randomBadModify(curr_tour);
		//~ t = findLeastBadTour(t, deadline);
		
		// Keep improving until done!
		while(findBestNewTour(curr_tour, deadline)) {};
		
		if(std::clock() > deadline)
			break;
		
		
		if(best_so_far->cost > curr_tour->cost) {			
			delete best_so_far;			
			best_so_far = curr_tour;
			
		} else {			
			delete curr_tour;
		}
			
	}
	
	//~ cout << best_so_far->cost << endl;
	//~ cout << "Broke out!" << endl;
	return best_so_far;
}

tour* SimulatedAnnealing::randomBadModify(tour* t) {
	//~ tour* t = cloneTour(original);
	int dim = map->getDimension();
	
	int i1 = rand() % dim;	
	int i2 = rand() % dim;
	
	//~ if(i2 == 0)
		//~ i2++;
	//~ if(i1 == 0)
		//~ i1++;
	if(i1 == i2)
		i1 = (i2 + 1) % dim ;
	
	//~ double cost = 0;
	//~ if(i1 < i2) { 
		//~ double cost = getNewCost(t, i1, i2);		
	
		//~ swap(t, i1, i2, cost);
	//~ } else {
		//~ double cost = getNewCost(t, i2, i1);		
		//~ swap(t, i2, i1, cost);
	//~ }
	t->cost += (i1 < i2) ? getNewBadCost(t, i1, i2) : getNewBadCost(t, i2, i1);
	int tmp = t->path[i1];
	t->path[i1] = t->path[i2];
	t->path[i2] = tmp;
	
	return t;	
}

tour* SimulatedAnnealing::findLeastBadTour(tour* t, std::clock_t deadline) {
	//~ tour* t = cloneTour(original);
	double bestcost = -1;
	int besti = -1;
	int bestj = -1;
	unsigned int size = t->path.size();
	for(unsigned int i = 1; i < size && std::clock() < deadline; ++i)
	{			
		//~ int city = t->path[i];
		//~ double maxAllowedDist = map->getDistance(city,city);
		
		for(unsigned int j = i+1; j < size; ++j)
		{				
			//~ if(maxAllowedDist < map->getDistance(city,t->path[j]))
				//~ continue;
						
			double cost = getNewCost(t, i, j);
			
			if(bestcost < 0 || cost < bestcost) {									
				bestcost = cost;
				besti = i;
				bestj = j;	
				//~ cout << i << " " << j << " " << bestcost << endl;
			}		
		}	
	}	
	
	if(besti > -1)
		swap(t, besti, bestj, bestcost);	
	return t;
}

double SimulatedAnnealing::getNewBadCost(tour* t, int i1, int i2) {
        int city1 = t->path[i1];
        int city2 = t->path[i2];
        
        int lastIndex = t->path.size()-1;       
        int left1 = i1 > 0 ? t->path[i1-1] : t->path[lastIndex];
        int right1 = i1 < lastIndex ? t->path[i1+1] : t->path[0];
        int left2 = i2 > 0 ? t->path[i2-1] : t->path[lastIndex];
        int right2 = i2 < lastIndex ? t->path[i2+1] : t->path[0];
        

        double cost = 0;        
        
        cost -= map->getDistance(city1, left1);                
        cost -= map->getDistance(city2, right2);        
        cost += map->getDistance(city2, left1);        
        cost += map->getDistance(city1, right2);
        
        if(right1 == city2) {
			return cost;
        }
        
        cost -= map->getDistance(city1, right1);
        cost -= map->getDistance(city2, left2);        
        cost += map->getDistance(city1, left2);
        cost += map->getDistance(city2, right1);
        
        return cost;
}
