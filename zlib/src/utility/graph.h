#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>

#include "GlobalDefinition.h"
//#include "ECVector.h"

using namespace std;

class Edge {
 public:
  VertexID v, w;
  EdgeID edge_id;

  EdgeLabel label;

  Edge(EdgeID edge_id = NO_EDGE, VertexID v = NO_EDGE, VertexID w = NO_EDGE,
       EdgeLabel l = NO_LABEL)
      : v(v),
        w(w),
        edge_id(edge_id),
        label(l) {
  }

  Edge(std::string s1, std::string s2) {
    v = atoi(s1.c_str());
    w = atoi(s2.c_str());
  }

  ~Edge() {
  }

  bool operator ==(Edge e) {
    if (e.v == v && e.w == w)
      return true;
    else if (e.w == v && e.v == w)
      return true;

    return false;
  }

  void operator =(const Edge& e) {
    this->v = e.v;
    this->w = e.w;
    this->label = e.label;
    this->edge_id = e.edge_id;
  }
};

class AdjElement {
 public:
  VertexID v;
  EdgeID eid;
  EdgeLabel elabel;

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
  vector<vector<AdjElement> > _adjList;
  int maximum_vertex;

  VertexLabelMap vlabels_map;
  VertexLabelMapCnt vlabels_map_cnt;

  // -------- for equivalent class -------
  vector< set<VertexID> > eqv_cls;
  // -------------------------------------

  // -------- for subiso --------
  int* M; // for query graph
  int* col;
  int* row;
  VertexID* row_col_next_one;
  int* cnt_ones_of_row;
  // ----------------------------

  GRAPH();
  GRAPH(int V);
  ~GRAPH();

  int V();
  int E();

  void setV(int V);

#ifdef ADJMATRIX
  int ADJ(VertexID row, VertexID col) const;
#endif

  void setLabel(VertexID v, VertexLabel label);

  void setELabel(VertexID u, VertexID v, VertexLabel label);
  void setGraphId(GraphID id);

  VertexLabel getLabel(VertexID v);
  EdgeLabel getELabel(VertexID u, VertexID v) const;

  void insert(Edge e);
  bool edge(VertexID v, VertexID w);
  int getDegree(VertexID v);
  void printGraphNew(std::ostream& out);
  int makeEmpty();

  void setVertexLabelMap();
  void setVertexLabelMapCnt();

  void BFS(VertexID start_v, int lens, set<int>& visit_v);
  void BFSwithConst(VertexID start_v, int hops, set<VertexID>& visit_v,
                    VertexLabelMapCnt& _vertex_label_map_cnt);

  void getInducedSubGraph(set<int>& vertex, GRAPH* _g);
  void getInducedSubGraph(vector<int>& vertex, GRAPH* _g);

  void initSubIso();
  void clearSubIso();
  bool isSubgraphOf(GRAPH* g);  // g and g' are of same size
  void isSubgraphOf1(int dep, GRAPH* g, int& res);
  void isSubgraphOf2(GRAPH* g, int& res);
  void initM(int* M, GRAPH* g);
  void cnt_ones_of_row_function(GRAPH* g);
  void resetSubIso(GRAPH* g);

  void genEqvCls();
  bool shareSameNeighbor(VertexID u, VertexID v);
  void updateEqvCls(VertexID u, VertexID v);
  void resetEqvCls();
  void clearEqvCls();
  void initEqvCls(int _size);
};

#endif
