#include "graph.h"

#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <utility>

#include "utilityFunction.h"

using namespace std;

#ifndef DEFAULT_VERTEX_NUMBER
#define DEFAULT_VERTEX_NUMBER   (256)
#endif

GRAPH::GRAPH()
    : Vcnt(0),
      Ecnt(0),
      graphId(INVALID_GRAPH_ID) {
  digraph = false;
#ifdef ADJMATRIX
  _adj = new Label[DEFAULT_VERTEX_NUMBER * DEFAULT_VERTEX_NUMBER];
#endif
  _vlabels = new VertexLabel[DEFAULT_VERTEX_NUMBER];
  _adjList.resize(DEFAULT_VERTEX_NUMBER);

  maximum_vertex = DEFAULT_VERTEX_NUMBER;

#ifdef ADJMATRIX
  for (int i = 0; i < (DEFAULT_VERTEX_NUMBER * DEFAULT_VERTEX_NUMBER); i++)
  _adj[i] = NO_EDGE;
#endif

  for (int i = 0; i < DEFAULT_VERTEX_NUMBER; i++)
    _vlabels[i] = NO_LABEL;
}

GRAPH::GRAPH(int V)
    : Vcnt(V),
      Ecnt(0),
      graphId(INVALID_GRAPH_ID) {
  digraph = false;
#ifdef ADJMATRIX
  _adj = new Label[V * V];
#endif
  _vlabels = new VertexLabel[V];
  _adjList.resize(V);

  maximum_vertex = V;
#ifdef ADJMATRIX
  for (int i = 0; i < (V * V); i++)
  _adj[i] = NO_EDGE;
#endif
  for (int i = 0; i < V; i++)
    _vlabels[i] = NO_LABEL;
}

GRAPH::~GRAPH() {
#ifdef ADJMATRIX
  delete[] _adj;
#endif
  delete[] _vlabels;
}

int GRAPH::V() {
  return Vcnt;
}

int GRAPH::E() {
  return Ecnt;
}

void GRAPH::setV(int V) {
  if (maximum_vertex > V) {
    Vcnt = V;
  } else {
    VertexLabel* vlabels2 = new VertexLabel[V];

#ifdef ADJMATRIX
    Label* adj2 = new Label[V * V];
#endif

    maximum_vertex = V;
    int prev_Vcnt = Vcnt;
    Vcnt = V;

    delete[] _vlabels;
#ifdef ADJMATRIX
    delete[] _adj;
#endif
    _vlabels = vlabels2;
#ifdef ADJMATRIX
    _adj = adj2;

    for (int i = 0; i < (V * V); i++)
    _adj[i] = NO_EDGE;
#endif
    for (int i = 0; i < V; i++)
      _vlabels[i] = NO_LABEL;

    for (int i = 0; i < prev_Vcnt; i++) {
      _adjList[i].clear();
    }
    _adjList.clear();
    _adjList.resize(V);
  }
}

void GRAPH::setLabel(VertexID v, VertexLabel label) {
#ifdef ADJMATRIX
  int index = MATRIX_INDEX(v, v, Vcnt);
  _adj[index] = _vlabels[v];
#endif
  _vlabels[v] = label;
}

void GRAPH::setELabel(VertexID u, VertexID v, VertexLabel label) {
#ifdef ADJMATRIX
  int index = MATRIX_INDEX(u, v, Vcnt);
  int index2 = MATRIX_INDEX(v, u, Vcnt);

  _adj[index] = label;
  _adj[index2] = label;
#endif

#ifndef ADJMATRIX
  if (!edge(u, v)) {
    insert(Edge(Ecnt, u, v, label));
  } else {
    for (int i = 0; i < (int) _adjList[u].size(); i++) {
      if (_adjList[u][i].v == v) {
        _adjList[u][i].elabel = label;
      }
    }
  }
#endif
}

VertexLabel GRAPH::getLabel(VertexID v) {
  ASSERT(v < Vcnt);
  return _vlabels[v];
}

#ifdef ADJMATRIX
int GRAPH::ADJ(VertexID row, VertexID col) const {
  int index = MATRIX_INDEX(row, col, Vcnt);

  return _adj[index];
}
#endif

EdgeLabel GRAPH::getELabel(VertexID u, VertexID v) const {
#ifdef ADJMATRIX
  return ADJ(u, v);
#endif

#ifndef ADJMATRIX
  ASSERT(u < Vcnt);
  ASSERT(v < Vcnt);

  for (int i = 0; i < (int) _adjList[u].size(); i++) {
    if (_adjList[u][i].v == v) {
      return _adjList[u][i].elabel;
    }
  }
  return NO_EDGE;
#endif
}

void GRAPH::insert(Edge e) {
  VertexID v = e.v, w = e.w;
#ifdef ADJMATRIX
  int index = MATRIX_INDEX(v, w, Vcnt);
  int index2 = MATRIX_INDEX(w, v, Vcnt);

  if (_adj[index] == NO_EDGE) {
    Ecnt++;
    _adj[index] = e.label;
    if (!digraph) {
      _adj[index2] = e.label;
    }
  }
#endif

#ifndef ADJMATRIX
  Ecnt++;
#endif
  _adjList[v].push_back(AdjElement(w, e.edge_id, e.label));
  _adjList[w].push_back(AdjElement(v, e.edge_id, e.label));
}

void GRAPH::setGraphId(GraphID id) {
  graphId = id;
}

void GRAPH::printGraphNew(std::ostream& out) {
  out << "-1 " << graphId << endl;

  out << Vcnt << " " << Ecnt << endl;

  for (VertexID i = 0; i < Vcnt; i++)
    out << i << " " << _vlabels[i] << std::endl;

  for (VertexID i = 0; i < Vcnt; i++) {
    cout << "Deg(" << i << ") = " << getDegree(i) << " ";
    for (VertexID j = 0; j < Vcnt; j++) {
      if (edge(i, j) && i != j) {
        out << "(" << j << ", " << getELabel(i, j) << ")";
      }
    }
    out << std::endl;
  }

  cout << "vertex label map: " << endl;
  for (VertexLabelMap::iterator it = vlabels_map.begin();
      it != vlabels_map.end(); it++) {
    VertexLabel _v_lable = it->first;
    cout << _v_lable << ": ";
    for (set<VertexID>::iterator it1 = it->second.begin();
        it1 != it->second.end(); it1++) {
      VertexID _v_id = *it1;
      cout << _v_id << " ";
    }
    cout << endl;
  }

  cout << "vertex label map cnt: " << endl;
  for (VertexLabelMapCnt::iterator it = vlabels_map_cnt.begin();
      it != vlabels_map_cnt.end(); it++) {
    VertexLabel _v_lable = it->first;
    cout << _v_lable << ": " << it->second << endl;
  }
}

bool GRAPH::edge(VertexID v, VertexID w) {
#ifdef ADJMATRIX
  return (ADJ(v, w) != NO_EDGE);
#endif
  return getELabel(v, w) != NO_EDGE;
}

int GRAPH::makeEmpty() {
  int nElem = Vcnt * Vcnt;

#ifdef ADJMATRIX
  for (int i = 0; i < nElem; i++) {
    _adj[i] = NO_EDGE;
  }
#endif
  for (int i = 0; i < Vcnt; i++) {
    _vlabels[i] = NO_LABEL;
  }

  for (int i = 0; i < Vcnt; i++) {
    _adjList[i].clear();
  }

  Vcnt = 0;
  Ecnt = 0;
  graphId = INVALID_GRAPH_ID;

  return 0;
}

int GRAPH::getDegree(VertexID v) {
  ASSERT(v < Vcnt);

  return (int) _adjList[v].size();
}

void GRAPH::setVertexLabelMap() {
  for (int i = 0; i < Vcnt; i++) {
    VertexLabel _v_label = _vlabels[i];
    vlabels_map[_v_label].insert(i);
  }
}

void GRAPH::setVertexLabelMapCnt() {
  for (int i = 0; i < Vcnt; i++) {
    VertexLabel _v_label = _vlabels[i];
    vlabels_map_cnt[_v_label] = 0;
  }

  for (int i = 0; i < Vcnt; i++) {
    VertexLabel _v_label = _vlabels[i];
    vlabels_map_cnt[_v_label]++;
  }
}

void GRAPH::BFSwithConst(VertexID start_v, int hops, set<VertexID>& visit_v,
                         VertexLabelMapCnt& _vertex_label_map_cnt) {
  cout << "start BFS new " << endl;
  cout << "hop size" << hops << endl;

  VertexLabel _u_l = _vlabels[start_v];
  _vertex_label_map_cnt[_u_l]--;
  visit_v.insert(start_v);

  set<int> nodes;
  nodes.insert(start_v);

  set<int>::iterator it_begin = nodes.begin();
  set<int>::iterator it_end = nodes.end();

  // for each hop
  for (int cnt_hops = 0; cnt_hops < hops; cnt_hops++) {

    cout << "hop - vertex: ";
    printSet(nodes);
    if (nodes.size() == 0) {
      cout << "end BFS" << endl;
      return;
    }

    set<int> next_nodes;

    // for each node v
    for (set<int>::iterator it = it_begin; it != it_end; it++) {
      int v = *it;

      // for each incident node u of v
      for (int j = 0; j < getDegree(v); j++) {
        int u = _adjList[v][j].v;

        // u is visited
        if (visit_v.find(u) != visit_v.end()) {
          continue;
        }

        // u's label is not contained
        VertexLabel _u_l = _vlabels[u];
        if (_vertex_label_map_cnt.find(_u_l) == _vertex_label_map_cnt.end()) {
          continue;
        }

        // update the label map cnt
        if (_vertex_label_map_cnt[_u_l] > 0) {
          _vertex_label_map_cnt[_u_l]--;
        }

        // u is what we want,
        // add u to next_nodes for iteration
        visit_v.insert(u);
        next_nodes.insert(u);
      }
    }
    // nodes = next_nodes;
    nodes = next_nodes;
    it_begin = nodes.begin();
    it_end = nodes.end();
  }
  cout << "end BFS new" << endl;
}

void GRAPH::BFS(VertexID start_v, int hops, set<VertexID>& visit_v) {
  cout << "start BFS" << endl;
  cout << "hop size " << hops << endl;
  visit_v.insert(start_v);

  set<VertexID> nodes;
  nodes.insert(start_v);

  set<VertexID>::iterator it_begin = nodes.begin();
  set<VertexID>::iterator it_end = nodes.end();

  // for each hop
  for (int cnt_hops = 0; cnt_hops < hops; cnt_hops++) {

//    cout << "hop - vertex: ";
//    printSet(nodes);
    if (nodes.size() == 0) {
      cout << "end BFS" << endl;
      return;
    }

    set<int> next_nodes;

    // for each node v
    for (set<VertexID>::iterator it = it_begin; it != it_end; it++) {
      VertexID v = *it;

      // for each incident node u of v
      for (int j = 0; j < getDegree(v); j++) {
        VertexID u = _adjList[v][j].v;

        // u is visited
        if (visit_v.find(u) != visit_v.end()) {
          continue;
        }

        // u is what we want,
        // add u to next_nodes for iteration
        visit_v.insert(u);
        next_nodes.insert(u);
      }
    }
    // nodes = next_nodes;
    nodes = next_nodes;
    it_begin = nodes.begin();
    it_end = nodes.end();
  }
  cout << "end BFS" << endl;
}

// TODO
//
void GRAPH::getInducedSubGraph(vector<int>& vertex, GRAPH* _ind_g) {
  int _ind_g_s = vertex.size();
  _ind_g->setV(_ind_g_s);

  // set vertex
  VertexID _new_v_id = 0;
  map<VertexID, VertexID> g_to_ind_v;
  for (vector<int>::iterator it = vertex.begin(); it != vertex.end(); it++) {
    VertexID u = *it;
    VertexLabel u_l = getLabel(u);
    g_to_ind_v[u] = _new_v_id;
    _ind_g->setLabel(_new_v_id++, u_l);
  }

  // set edge
  int e_id = 0;
  for (vector<int>::iterator it = vertex.begin(); it != vertex.end(); it++) {
    VertexID u = *it;
    for (vector<int>::iterator it1 = vertex.begin(); it1 != vertex.end();
        it1++) {
      VertexID v = *it1;
      if (!edge(u, v) || u <= v)
        continue;

      _ind_g->insert(
          Edge(e_id++, g_to_ind_v[u], g_to_ind_v[v], getELabel(u, v)));
    }
  }

  // set the vertex label map of the induced subgraph
  _ind_g->setVertexLabelMap();
  _ind_g->setVertexLabelMapCnt();
}

// TODO
//
void GRAPH::getInducedSubGraph(set<int>& vertex, GRAPH* _ind_g) {
  int _ind_g_s = vertex.size();
  _ind_g->setV(_ind_g_s);

  // set vertex
  VertexID _new_v_id = 0;
  map<VertexID, VertexID> g_to_ind_v;
  for (set<int>::iterator it = vertex.begin(); it != vertex.end(); it++) {
    VertexID u = *it;
    VertexLabel u_l = getLabel(u);
    g_to_ind_v[u] = _new_v_id;
    _ind_g->setLabel(_new_v_id++, u_l);
  }

  // set edge
  int e_id = 0;
  for (set<int>::iterator it = vertex.begin(); it != vertex.end(); it++) {
    VertexID u = *it;
    for (set<int>::iterator it1 = vertex.begin(); it1 != vertex.end(); it1++) {
      VertexID v = *it1;
      if (!edge(u, v) || u <= v)
        continue;

      _ind_g->insert(
          Edge(e_id++, g_to_ind_v[u], g_to_ind_v[v], getELabel(u, v)));
    }
  }

  // set the vertex label map of the induced subgraph
  _ind_g->setVertexLabelMap();
  _ind_g->setVertexLabelMapCnt();
}

void GRAPH::initM(int* M, GRAPH* g) {
  for (int i = 0; i < V(); i++) {
    VertexID u_q = i;
    VertexLabel u_q_l = getLabel(u_q);

    for (int j = 0; j < V(); j++) {
      VertexID u_g = j;
      VertexLabel u_g_l = getLabel(u_g);

      if (u_q_l == u_g_l) {
        M[MATRIX_INDEX(u_q, u_g, V())] = 1;
      } else {
        M[MATRIX_INDEX(u_q, u_g, V())] = 0;
      }
    }
  }
}

void GRAPH::cnt_ones_of_row_function(GRAPH* g) {
  for (int i = 0; i < V(); i++) {
    int _cnt = 0;
    for (int j = 0; j < V(); j++) {
      if (M[MATRIX_INDEX(i, j, V())] == 1) {
        row_col_next_one[MATRIX_INDEX(i, _cnt, V())] = (VertexID) j;
        _cnt++;
      }
    }
    cnt_ones_of_row[i] = _cnt;
  }
}

void GRAPH::resetSubIso(GRAPH* g) {
  ASSERT(V() == g->V());

  initM(M, g);
  cout << "=========== M ==============" << endl;
  printMatrix(M, V());

  setArray(row, V(), 0);
  setArray(row_col_next_one, V()*V(), 0);

  cnt_ones_of_row_function(g);
}

void GRAPH::initSubIso(GRAPH* g) {
  // TODO
  // initialization
  M = new int[V() * V()];
  col = new int[V()];
  row = new int[V()];
  row_col_next_one = new VertexID[V() * V()];
  cnt_ones_of_row = new int[V()];
}

void GRAPH::isSubgraphOf2(GRAPH* g, int& res) {
  cout << "=========== print array col ==============" << endl;
  printArray(col, V());

  // real thing


}

void GRAPH::isSubgraphOf1(int dep, GRAPH* g, int& res) {
  // early stop
  if (res == 0) {
    return;
  }

  // okay
  if (dep == V()) {
    isSubgraphOf2(g, res);
    return;
  }

  // enumeration
  for (int i = 0; i < cnt_ones_of_row[dep]; i++) {
    int j = row_col_next_one[MATRIX_INDEX(dep, i, V())];

    if (row[j] == 0) {
      row[j] = 1;
      col[dep] = j;
      isSubgraphOf1(dep + 1, g, res);
      row[j] = 0;
    }
    // else continue
  }

}

/**
 * q and g are of the same size
 * a very naive solution for
 * preserving the privacy of q
 */
bool GRAPH::isSubgraphOf(GRAPH* g) {
  int res = 1;
  isSubgraphOf1(0, g, res);
  return (res == 0);
}



