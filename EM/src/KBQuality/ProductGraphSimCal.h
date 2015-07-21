/*
 * ProductGraphSimCal.h
 *
 *  Created on: 2015¦~2¤ë6¤é
 *      Author: zfan
 */

#ifndef PRODUCTGRAPHSIMCAL_H_
#define PRODUCTGRAPHSIMCAL_H_

#include <time.h>
#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include "GlobalDefinition.h"
#include "DiGraph.h"

using namespace std;

typedef pair<VertexID, unordered_set<VertexID> > PairIntHset;
typedef unordered_map<VertexID, unordered_set<VertexID> > MapIntHset;
typedef pair<VertexID, VertexID> PairIntInt;
typedef unordered_map<VertexID, VertexID> MapIntInt;

class ProductGraphSimCal {
 private:
  bool graphSimMain(DIPRODUCTGRAPH *, DIKEYS *, MapIntHset &, MapIntHset &);
  bool graphInitialization(DIPRODUCTGRAPH *, DIKEYS *, MapIntHset &,
                           MapIntHset &);
  bool graphSimRefinement(DIPRODUCTGRAPH *, DIKEYS *, MapIntHset &, MapIntHset &);
  VertexID myIntersection(DIPRODUCTGRAPH::AdjList& vec1,
                     unordered_set<VertexID> &);
  VertexID findNoEmptyRemove(MapIntHset &, DIKEYS *);
  void counterInitilization(unordered_map<VertexID, unordered_map<VertexID, VertexID> > &,
                            MapIntHset &, DIPRODUCTGRAPH *, DIKEYS *);
  bool checkNodeFeasibility(DIPRODUCTGRAPH *, VertexID, DIKEYS *, VertexID);

 public:
  time_t main1;
  time_t inti1, init1_2, refine2, check3, remove4, funcmain, funcrefine,
      funccheck, copyball;
  ProductGraphSimCal();
  bool SimCal(DIPRODUCTGRAPH *, DIKEYS *, MapIntHset &);
};

#endif /* PRODUCTGRAPHSIMCAL_H_ */
