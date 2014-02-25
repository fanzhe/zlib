#ifndef ISSUBISO_H
#define ISSUBISO_H

#include <set>
#include <vector>

#include "../utility/GlobalDefinition.h"
#include "cache.h"

class GRAPH;
namespace GSPAN {
struct DFSCode;
} /* namespace GSPAN */

class SubIso {
 protected:
  VertexLabel start_label;
  GRAPH* q;
  GRAPH* g;
  bool response;
  int tree_height;
  int cnt_cm;

  double cr_time;
  double cm_time;
  double match_time;
  double enum_cm_time;
//  set<string> ifHasString;
//  set<HashCode> ifHasCm;
  Cache cache;

 public:
  SubIso(GRAPH* _q, GRAPH* _g);
  void calVertexLabelMap();
  void clearVertexLabelMap();
  bool isVisited(vector<int>& path, int dep, int v);
  void genAllCanReg(set<VertexID>& rootVertexSet);
  bool genCanReg(VertexID& r_vertex, GRAPH* cr);
  void canRegEqvCls(GRAPH* cr, VertexID& r_vertex);
  void canRegReduce(GRAPH* cr, VertexID& r_vertex);
  void genAllCanMatch(VertexID r_vertex, GRAPH* cr, GRAPH* cm);
  void genCanMatch(int dep, GRAPH* cr, vector<VertexID>& canVertex, GRAPH* cm);
  void doMatch(GRAPH* cm);
  void doSubIso();
  bool isSubIso();
  bool isMapped(VertexLabelMapCnt& _vertex_map_cnt);
  bool isCanMatChecked(GRAPH* g);
  void cacheAllSubOf(GSPAN::DFSCode& dfs_code);
};

#endif
