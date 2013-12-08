
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <math.h>
#include <fstream>
#include <ctime>

#include "map.h"
#include "types.h"
#include "minspan.h"
#include "localsearch.h"
#include "tabusearch.h"
#include "naivegreedy.h"
#include "2opt.h"
#include "simulatedannealing.h"
#include "randomtour.h"

using namespace std;

#define KATTIS
//#define DEBUG_TRACE

Map* map;

void print_usage(string);
vector<coordinate*>* getCitiesFromSTDin();

int main()
{
	std::clock_t deadline = std::clock() + 1.9*CLOCKS_PER_SEC;
	vector<coordinate*>* cities = getCitiesFromSTDin();
	if(cities->size() == 0)
	{
		return 0;
	}
	
	if(cities->size() == 1)
	{
		cout << 0 << endl;
		return 0;
	}

	map = new Map(cities);
	//~ LocalSearch* local_search = new TabuSearch(map);
	TwoOpt* twoopt = new TwoOpt(map);
	LocalSearch* local_search = new SimulatedAnnealing(map);
	NaiveGreedy* greedy = new NaiveGreedy;
	//~ RandomTour* randomTour = new RandomTour;
	
	//~ local_search->findBest = true;
	tour* best_tour = new tour;
	best_tour->cost = -1;
	twoopt->findBest = false;
	//~ twoopt->adjacency = 42;
	
	int i = 0;
	
	//~ std::clock() < deadline && 
	for(; i < map->getDimension() && std::clock() < deadline; i++)
	{
		// rand() % map->getDimension()
		
		// Startgissning		
		tour* curr_tour = greedy->naiveTspPath(map, rand() % map->getDimension());
		
		//~ double greedy_cost = curr_tour->cost;		
		
		//~ cout << best_tour->cost << " " << curr_tour->cost << endl;
			
		//~ curr_tour = twoopt->getBetterTour(curr_tour, deadline);
		
		if(curr_tour->cost < best_tour->cost || best_tour->cost < 0) {			
			tour* tmp = best_tour;
			best_tour = curr_tour;
			curr_tour = tmp;			
		}				
		delete curr_tour;
	}
	
	//~ tour* curr_tour = greedy->naiveTspPath(map, rand() % map->getDimension());
	
	//~ twoopt->findBest = true;	
	best_tour = twoopt->getBetterTour(best_tour, deadline);
	
	//~ // Förbättring
	best_tour = local_search->getBetterTour(best_tour, deadline);
	//~ 
	//~ cout << "Improvements: " << i << endl;
	
	// Output.
	printTour(best_tour);
	
	// Free resources
	delete local_search;
	delete map;
	
}

vector<coordinate*>* getCitiesFromSTDin()
{
	string line;
	getline(cin, line);
	int count = atoi(line.c_str());

	vector<coordinate*>* cities = new vector<coordinate*>();

	for(int i = 0; i < count; i++)
	{
		getline(cin, line);
		cities->push_back(newCoordinate(line));
	}

	return cities;
}
