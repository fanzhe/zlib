#ifndef ISSUBISO_H
#define ISSUBISO_H

#include <set>
#include <vector>

#include "../utility/GlobalDefinition.h"
#include "../utility/myStat.h"

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
  STAT* myStat;

//  set<string> ifHasString;
//  set<HashCode> ifHasCm;
  Cache cache;

  typedef struct VERTEXWDEG {
    VertexID u;
    int deg;

    bool operator <(const VERTEXWDEG& vd) const {
      return deg < vd.deg;
    }
  } VertexWDeg;

 public:
  SubIso(GRAPH* _q, GRAPH* _g);
  ~SubIso();
  void calVertexLabelMap();
  void clearVertexLabelMap();
  bool isVisited(vector<int>& path, int dep, int v);
  void genAllCanReg(vector<VertexWDeg>& rootVertexSet);
  bool genCanReg(VertexID& r_vertex, GRAPH* cr);
  bool predictCR(GRAPH* cr, long long limit);
  void canRegEqvCls(GRAPH* cr, VertexID& r_vertex);
  void canRegReduce(GRAPH* cr, VertexID& r_vertex);
  void genAllCanMatch(VertexID r_vertex, GRAPH* cr, GRAPH* cm);
  void genCanMatch(int dep, GRAPH* cr, vector<VertexID>& canVertex, GRAPH* cm);
  void doMatch(GRAPH* cm);
  void sortRootVertex(set<VertexID>& rootVertex,
                      vector<VertexWDeg>& rootVertexNew);
  void doSubIso();
  bool isSubIso();
  bool isMapped(VertexLabelMapCnt& _vertex_map_cnt);
  bool isCanMatChecked(GRAPH* g);
  void cacheAllSubOf(GSPAN::DFSCode& dfs_code);

};

#endif
