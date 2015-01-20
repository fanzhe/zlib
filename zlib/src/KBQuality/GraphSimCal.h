#ifndef GRAPHSIMCAL_H
#ifndef GRAPHSIMCAL_H

#include <time.h>
#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include "../utility/GlobalDefinition.h"
#include "../utility/DiGraph.h"

using namespace std;

typedef pair< int, unordered_set<int> > PairIntHset;
typedef unordered_map< int, unordered_set<int> > MapIntHset;
typedef pair<int, int> PairIntInt;
typedef unordered_map<int, int> MapIntInt;

class GraphSimCal {
private:
	bool graphSimMain(DIGRAPHBASIC *, DIKEYS *, MapIntHset &, MapIntHset &);
	bool graphInitialization(DIGRAPHBASIC *, DIKEYS *, MapIntHset &, MapIntHset &);
	bool graphSimRefinement(DIGRAPHBASIC *, DIKEYS *, MapIntHset &, MapIntHset &);
	int myIntersection(vector<int> &, unordered_set<int> &);
	int findNoEmptyRemove(MapIntHset &, DIKEYS *);
	void counterInitilization(vector<vector<int> > &, MapIntHset &, DIGRAPHBASIC *, DIKEYS *);

public:
	time_t main1;
	time_t inti1, init1_2, refine2, check3, remove4, funcmain, funcrefine, funccheck, copyball;
	GraphSimCal();
	bool SimCal(DIGRAPHBASIC *, DIKEYS *, MapIntHset &);
};

#endif
