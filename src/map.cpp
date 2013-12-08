#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>    // std::sort

#include "types.h"
#include "map.h"
#include "types.h"

using namespace std;

Map::Map (vector<coordinate*>* cities)
{
	dim = cities->size();
	setDistanceMatrix(cities);
	cities->clear();		
	delete cities; // Frigör coordinates etc, vi behöver space för neighbourmaps? :/
	setAdjacencyThresholds();
}

Map::~Map()
{
	for(int i = 0; i < dim; i++)
	{
		delete distance_mat[i];
	}
	delete distance_mat;	
}

double Map::getDistance(int i, int j)
{
	return distance_mat[i][j];
}

double* Map::getDistanceRow(int i) {
	return distance_mat[i];
}

int Map::getDimension()
{
	return dim;
}

double Map::calculateDistance(coordinate* c1, coordinate* c2)
{
	double x = c1->x - c2->x;
	double y = c1->y - c2->y;
	return sqrt(pow(x, 2.0) + pow(y, 2.0));
}

void Map::setDistanceMatrix(std::vector<coordinate*>* cities)
{
	distance_mat = new double*[dim];
	for(int i = 0; i < dim; i++)
	{
		distance_mat[i] = new double[dim];
	}

	for(int i = 0; i < dim; i++)
	{		
		for(int j = i+1; j < dim; j++)
		{
			double d = calculateDistance(cities->at(i), cities->at(j));
			distance_mat[i][j] = d;
			distance_mat[j][i] = d;			
		}
	}
}

//~ bool Map::distCompare (int i,int j) { 
	//~ return (distance_mat[current][i] < distance_mat[current][j]); 
//~ }

void Map::setAdjacencyThresholds() {
	// Adjancency lists...
	
	int dim = getDimension();
	//~ int pos = (dim > ADJACENCY_LIST_SIZE) ?  ADJACENCY_LIST_SIZE - 2 : dim - 2;	
	//~ this->adjacencyThresholds = new double[dim];
	
	for(int i = 0; i < dim; ++i) {	
		vector<int>* neighbours = new vector<int>;
		for(int j = 0; j < dim; ++j) {
			if(i == j) continue;			
			neighbours->push_back(j);
		}		
		
		sort(neighbours->begin(), neighbours->end(), 
			[i, this] (int x, int y) -> bool { 
				return this->getDistance(i,x) < this->getDistance(i,y);
			}
		);
		
		
		//~ if(i == 0)
		//~ {
			//~ for(int j = 0; j < neighbours->size(); ++j)
			//~ {				
				//~ cout << neighbours->at(j) << " " << distance_mat[i][neighbours->at(j)] << endl;
			//~ }
		//~ }
		
		this->adjacencyLists.push_back( neighbours );	
		
	}
}



double Map::getTourDistance(vector<int>& nodes)
{
	//~ cout << "Getting tour distance" << endl;
	double c = 0;
	int N = nodes.size();
	for(int i = 1; i < N; i++)
	{
		c += getDistance(nodes[i-1], nodes[i]);
	}
	c += getDistance(nodes[N-1], nodes[0]);
	return c;
}

double Map::getTourDistance(tour* t) {
	return getTourDistance(t->path);
}
