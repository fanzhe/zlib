#ifndef ISSUBISO_H
#define ISSUBISO_H

#include <iostream>
#include <map>
#include <set>
#include <string>

#include "../utility/graph.h"
#include "../utility/GlobalDefinition.h"

class SubIso {
 protected:
  VertexLabel start_label;
  GRAPH* q;
  GRAPH* g;
  bool res;

  set<string> ifHasString;
  set<HashCode> ifHasCm;

 public:
  SubIso(GRAPH* _q, GRAPH* _g);
  void calVertexLabelMap();
  bool isVisited(vector<int>& path, int dep, int v);
  void genAllCanReg(set<VertexID>& rootVertexSet);
  bool genCanReg(VertexID r_vertex, GRAPH* cr);
  void genAllCanMatch(VertexID r_vertex, GRAPH* cr, GRAPH* cm);
  void genCanMatch(int dep, GRAPH* cr, vector<VertexID>& canVertex, GRAPH* cm);
  void doMatch(vector<VertexID>& canMatVertex, GRAPH* cm);
  void doSubIso();
  bool isSubIso();
  bool isMapped(VertexLabelMapCnt& _vertex_map_cnt);
  bool isCmChecked(GRAPH* g);
};

#endif
