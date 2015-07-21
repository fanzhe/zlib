#ifndef GRAPHSIMCAL_H
#define GRAPHSIMCAL_H

#include <time.h>
#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include "GlobalDefinition.h"
#include "DiGraph.h"

using namespace std;

typedef pair< VertexID, unordered_set<VertexID> > PairIntHset;
typedef unordered_map< VertexID, unordered_set<VertexID> > MapIntHset;
typedef pair<VertexID, VertexID> PairIntInt;
typedef unordered_map<VertexID, VertexID> MapIntInt;

class GraphSimCal {
private:
	bool graphSimMain(DIGRAPHBASIC *, DIKEYS *, MapIntHset &, MapIntHset &);
	bool graphInitialization(DIGRAPHBASIC *, DIKEYS *, MapIntHset &, MapIntHset &);
	bool graphSimRefinement(DIGRAPHBASIC *, DIKEYS *, MapIntHset &, MapIntHset &);
	VertexID myIntersection(DIGRAPHBASIC::AdjList& vec1, unordered_set<VertexID> &);
	VertexID findNoEmptyRemove(MapIntHset &, DIKEYS *);
	void counterInitilization(unordered_map<VertexID, unordered_map<VertexID, VertexID> > &, MapIntHset &, DIGRAPHBASIC *, DIKEYS *);
	bool checkNodeFeasibility(DIGRAPHBASIC*, VertexID , DIKEYS*, VertexID);

public:
	time_t main1;
	time_t inti1, init1_2, refine2, check3, remove4, funcmain, funcrefine, funccheck, copyball;
	GraphSimCal();
	bool SimCal(DIGRAPHBASIC *, DIKEYS *, MapIntHset &);
};

#endif
