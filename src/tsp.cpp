
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <math.h>
#include <fstream>

#include "map.h"
#include "types.h"
#include "christofides.h"
#include "localsearch.h"
#include "tabusearch.h"
#include "naivegreedy.h"
#include "2opt.h"

using namespace std;

//#define PRINT
#define KATTIS
//#define DEBUG_TRACE

Map* map;

void print_usage(string);
vector<coordinate*>* getCitiesFromSTDin();
vector<int> getPathFromFile(string);
void printMapMatrix();
void printMapCities();

int main()
{
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
	LocalSearch* local_search = new TabuSearch(map);
	//~ LocalSearch* local_search = new TwoOpt(map);
	NaiveGreedy* greedy = new NaiveGreedy;
	// christofides(map);
	
	tour* curr_tour= greedy->naiveTspPath(map);	

	#ifdef PRINT
		cout << "\nNaive path: \n";
		printTour(curr_tour);
	#endif
	
	curr_tour = local_search->getBetterTour(curr_tour);
	printTour(curr_tour);

	#ifdef DEBUG_TRACE
		LocalSearch* local_search_2 = new TwoOpt(map);
		tour* ref_tour= greedy.naiveTspPath(map);
		cout << "TABU-cost: " << curr_tour->cost << endl;
		cout << "Greedy-cost: " << ref_tour->cost << endl;		
		local_search_2->getBetterTour(ref_tour);
		cout << "Two-2-cost: " << ref_tour->cost << endl;		
	#endif

	// Free resources
	delete local_search;
	delete map;
	delete curr_tour;
}

vector<coordinate*>* getCitiesFromSTDin()
{
	string line;
	getline(cin, line);
	int count = atoi(line.c_str());

	vector<coordinate*>* cities = new vector<coordinate*>();

	for(int i = 0;i < count; i++)
	{
		getline(cin, line);
		cities->push_back(newCoordinate(line));
	}

	return cities;
}
