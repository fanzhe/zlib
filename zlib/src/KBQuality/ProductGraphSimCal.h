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
#include "../utility/GlobalDefinition.h"
#include "../utility/DiGraph.h"

using namespace std;

typedef pair<int, unordered_set<int> > PairIntHset;
typedef unordered_map<int, unordered_set<int> > MapIntHset;
typedef pair<int, int> PairIntInt;
typedef unordered_map<int, int> MapIntInt;

class ProductGraphSimCal {
 private:
  bool graphSimMain(DIPRODUCTGRAPH *, DIKEYS *, MapIntHset &, MapIntHset &);
  bool graphInitialization(DIPRODUCTGRAPH *, DIKEYS *, MapIntHset &,
                           MapIntHset &);
  bool graphSimRefinement(DIPRODUCTGRAPH *, DIKEYS *, MapIntHset &, MapIntHset &);
  int myIntersection(typename DIPRODUCTGRAPH::AdjList& vec1,
                     unordered_set<int> &);
  int findNoEmptyRemove(MapIntHset &, DIKEYS *);
  void counterInitilization(unordered_map<int, unordered_map<int, int> > &,
                            MapIntHset &, DIPRODUCTGRAPH *, DIKEYS *);
  bool checkNodeFeasibility(DIPRODUCTGRAPH *, int, DIKEYS *, int);

 public:
  time_t main1;
  time_t inti1, init1_2, refine2, check3, remove4, funcmain, funcrefine,
      funccheck, copyball;
  ProductGraphSimCal();
  bool SimCal(DIPRODUCTGRAPH *, DIKEYS *, MapIntHset &);
};

#endif /* PRODUCTGRAPHSIMCAL_H_ */
