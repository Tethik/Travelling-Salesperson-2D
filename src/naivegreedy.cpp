#include "naivegreedy.h"
#include "map.h"
#include <vector>

using namespace std;

// Kattis naive algorithm: https://kth.kattis.scrool.se/problems/oldkattis:tsp
tour* NaiveGreedy::naiveTspPath(Map* map)
{
	return naiveTspPath(map, 0);
}

tour* NaiveGreedy::naiveTspPath(Map* map, int startnode) {
	int N = map->getDimension();
	bool used[N];
	vector<int> path;
	// Init array
	for(int i = 0; i < N; i++)
	{
		used[i] = false;
		path.push_back(0);
	}
	used[startnode] = true;
	path[0] = startnode;	
	//~ double totalcost = 0.0;
	double cost = 0;
	for(int i = 1; i < N; i++)
	{
		int best = -1;
		double cost = 2147483647;
		
		for(int n = 0; n < map->adjacencyLists[path[i-1]]->size(); n++)
		{
			int j = map->adjacencyLists[path[i-1]]->at(n);
			
			if(used[j])
				continue;
			
			
			//~ double d = ;
			//~ 
			//~ if(best == -1 || d < cost)
			//~ {
			//~ cout << path[i-1] << " -> " <<  j << endl;
			best = j;
			cost += map->getDistance(path[i-1], j);
			//~ cost = d;
			break;
			//~ }
		}
		#ifdef DEBUG_TRACE
			cout << "Best " << best << endl;
			cout << "Cost " << cost << endl;
		#endif
		path[i] = best;
		used[best] = true;
	}
	
	cout << endl;
	cout << endl;
	
	tour* tmp = new tour(path);
	tmp->cost = cost;
	
	return tmp;
}
