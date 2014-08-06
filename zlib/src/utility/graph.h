#ifndef GRAPH_H
#define GRAPH_H

//#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "GlobalDefinition.h"
#include "DisjointSets.h"
#include "utilityFunction.h"

#include "../subIso/cache.h"

#include "../vflib/argedit.h"
#include "../vflib/argraph.h"
#include "../vflib/match.h"
#include "../vflib/ull_sub_state.h"
#include "../vflib/vf_sub_state.h"
#include "../vflib/vf2_sub_state.h"
#include "../vflib/vf2_mono_state.h"
//#include "ECVector.h"
#include "myStat.h"

#include "../crypto/bigMatrix.h"
#include "../crypto/cgbe.h"
#include "../crypto/message.h"
#include "SimpleGraph.h"

using namespace std;

/**
 * Adj definition.
 */
class AdjElement {
 public:
  VertexID v;
  EdgeID eid;
  EdgeLabel elabel;

  AdjElement() {
    v = eid = elabel = -1;
  }

  AdjElement(VertexID v, EdgeID eid, EdgeLabel _elabel)
      : v(v),
        eid(eid),
        elabel(_elabel) {
  }
  ~AdjElement() {
  }
};

class GRAPH {
 public:
  bool digraph;

  GraphID graphId;
  int Vcnt, Ecnt;

#ifdef ADJMATRIX
  Label* _adj;
#endif

  VertexLabel *_vlabels;
//  ECVector<ECVector<AdjElement> > _adjList;
  vector<unordered_map<VertexID, bool> > _adjVertex;
  vector<vector<AdjElement> > _adjList;
  int maximum_vertex;

  int min_tree_height;
  VertexLabel start_label;

  VertexLabelMap vlabels_map;
  VertexLabelMapCnt vlabels_map_cnt;

  // -------- for equivalent class -------
  vector<set<VertexID> > eqv_cls;
  DisjointSets* eqv_cls_aux;
  bool eqv_cls_flg;
  // -------------------------------------

  // -------- Encryption for query --------
  CGBE * cgbe;
  BigMatrix* Mq;
  Message* msg;
  void encryptInit(const int _encode_size, const int _agg_size);
  void encrypt();
  void decrypt();
  void finalDecrypt();
  void encryptFree();

  int DEFAULTENCODING;
  int DEFAULTAGGREGATE;
  int DEFAULTAGGREGATES;
  // --------------------------------------

  // for client
  void clientPreProcess(VertexLabelMapCnt& _glabel_cnt, const int _encode_size,
                        const int _agg_size);
  STAT* myStat;

  // -------- for subiso --------
  int* M;  // for mapping between query and matching
  int* col;
  int* col1;
  int* row;
  VertexID* row_col_next_one;
  int* cnt_ones_of_row;
  // ----------------------------

  GRAPH();
  GRAPH(int V);
  ~GRAPH();

  int V();
  int VnI();
  int E();

#ifdef ADJMATRIX
  int ADJ(VertexID row, VertexID col) const;
#endif

  void setV(int V);
  void setLabel(VertexID v, VertexLabel label);
  void setELabel(VertexID u, VertexID v, VertexLabel label);
  void setGraphId(GraphID id);

  VertexLabel getLabel(VertexID v);
  EdgeLabel getELabel(VertexID u, VertexID v) const;

  void insert(EdgeID _e_id, VertexID _u, VertexID _v, EdgeLabel _l);
  void insert(VertexID u, AdjElement& _adje);
  void insert(Edge e);
  bool edge(VertexID v, VertexID w);
  int getDegree(VertexID v);
  void printGraph(std::ostream& out);
  void printGraphNew(std::ostream& out);
  void printGraphPartial(std::ostream& out);
  int makeEmpty();
  void removeEdge(VertexID v, VertexID w);
  void removeVexAllEdges(VertexID u);

  // for generating candidate region
  void setVertexLabelMap();
  void setVertexLabelMapCnt();
  VertexID getMinTreeHeight(VertexLabelMapCnt& _glabel_cnt);
  int getMinTreeHeight2(VertexID start_v);
  void BFS(VertexID start_v, int lens, set<int>& visit_v);
  void BFSwithSpecVcnt(VertexID start_v, int vcnt, set<VertexID>& visit_v);
  void BFSwithConst(VertexID start_v, int hops, set<VertexID>& visit_v,
                    int& edge_cnt, VertexLabelMapCnt& _vertex_label_map_cnt,
                    Cache& cache);
  void BFSwithConst(VertexID start_v, int hops, set<VertexID>& visit_v,
                    VertexLabelMapCnt& _vertex_label_map_cnt, Cache& cache);
  void collectSimpleGraph(SIMPLEGRAPH& crv);
  void getInducedSubGraph(set<VertexID>& vertex, GRAPH* _g, VertexID& r_vertex,
                          STAT* _myStat = NULL);
  void getInducedSubGraph(vector<VertexID>& vertex, GRAPH* _g);
  void getInducedSubGraph(set<VertexID>& vertex, GRAPH* _ind_g);

  // for matching
  void initSubIso();
  void clearSubIso();
  bool isSubgraphOf(GRAPH* g);  // g and g' are of same size
//  void isSubgraphOf1(int dep, GRAPH* g, int& res);
  void isSubgraphOf1(int dep, GRAPH* g);
  void isSubgraphOf2(GRAPH* g);
  void isSubgraphOf2e(GRAPH* g);
  void initM(int* M, GRAPH* g);
  void cnt_ones_of_row_function(GRAPH* g);
  void resetSubIso(GRAPH* g);

  // for VFLib
//  bool my_vf2_match_visitor(int n, node_id c1[], node_id c2[], void *usr_data);
  bool isSubgrpahOfByVF2(GRAPH* g);
  class NodeCompare : public AttrComparator {
   public:
    virtual bool compatible(void *attr1, void *attr2) {
      int a = *((int*) (&attr1));
      int b = *((int*) (&attr2));
//      cout << "a: " << a << " b: " << b << endl;
      return a == b;
    }
  };

  // for generating equivalent class
  void reduceByEqvCls(VertexID& r_vertex,
                      VertexLabelMapCnt& _vertex_label_map_cnt);
  void reduceByEqvCls(VertexID& r_vertex, SIMPLEGRAPH& simple_graph,
                      VertexLabelMapCnt& _vertex_label_map_cnt);
  void genEqvCls();
  void genEqvCls(SIMPLEGRAPH& simple_graph);
  bool shareSameNeighbor(VertexID u, VertexID v);
  bool shareSameNeighbor(VertexID u, VertexID v, SIMPLEGRAPH& simple_graph);
  void updateEqvCls(VertexID u, VertexID v);
  void resetEqvCls();
  void clearEqvCls();
  void initEqvCls(int _size);

  // for generating selective size-k subgraphs

};

#endif
