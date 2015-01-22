#ifndef GRAPHSIMCAL_H
#define GRAPHSIMCAL_H

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
	int myIntersection(typename DIGRAPHBASIC::AdjList& vec1, unordered_set<int> &);
	int findNoEmptyRemove(MapIntHset &, DIKEYS *);
	void counterInitilization(unordered_map<int, unordered_map<int, int> > &, MapIntHset &, DIGRAPHBASIC *, DIKEYS *);
	bool checkNodeFeasibility(DIGRAPHBASIC*, int, DIKEYS*, int);

public:
	time_t main1;
	time_t inti1, init1_2, refine2, check3, remove4, funcmain, funcrefine, funccheck, copyball;
	GraphSimCal();
	bool SimCal(DIGRAPHBASIC *, DIKEYS *, MapIntHset &);
};

#endif
