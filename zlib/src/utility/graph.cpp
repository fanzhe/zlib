#include "graph.h"

#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <queue>
#include <tr1/unordered_map>

#include "utilityFunction.h"

using namespace std;
using namespace std::tr1;

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

  min_tree_height = NO_VERTEX;
  start_label = NO_VERTEX;
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

int GRAPH::VnI() {
  int _cnt = 0;
  for (int i = 0; i < V(); i++) {
    if (getDegree(i) > 0) {
      _cnt++;
    }
  }
  return _cnt;
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

void GRAPH::insert(VertexID _u, AdjElement& _adje) {
  _adjList[_u].push_back(_adje);
}

void GRAPH::insert(EdgeID _e_id, VertexID _u, VertexID _v, EdgeLabel _l) {
  _adjList[_u].push_back(AdjElement(_v, _e_id, _l));
  _adjList[_v].push_back(AdjElement(_u, _e_id, _l));
  Ecnt++;
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

void GRAPH::printGraphPartial(std::ostream& out) {
  cout << "vertex label map cnt: " << endl;
  for (VertexLabelMapCnt::iterator it = vlabels_map_cnt.begin();
      it != vlabels_map_cnt.end(); it++) {
    VertexLabel _v_lable = it->first;
    cout << _v_lable << ": " << it->second << endl;
  }
}

void GRAPH::printGraph(std::ostream& out) {
  out << "t # " << graphId << endl;

  for (VertexID i = 0; i < Vcnt; i++) {
    out << "v " << i << " " << getLabel(i) << endl;
  }

  for (VertexID i = 0; i < Vcnt; i++) {
    VertexID u = i;
    for (int j = 0; j < getDegree(u); j++) {
      VertexID v = _adjList[u][j].v;
      if (u < v) {
        out << "e " << u << " " << v << " " << _adjList[u][j].elabel << endl;
      }
    }
  }
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

// make empty of the most basic data structure
// for subIso or eqvCls, please use their corresponding API
int GRAPH::makeEmpty() {
#ifdef ADJMATRIX
  int nElem = Vcnt * Vcnt;

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

  vlabels_map.clear();
  vlabels_map_cnt.clear();

  return 0;
}

int GRAPH::getDegree(VertexID v) {
  ASSERT(v < Vcnt);

  return (int) _adjList[v].size();
}

void GRAPH::setVertexLabelMap() {
  vlabels_map.clear();

  for (int i = 0; i < Vcnt; i++) {
    if (getDegree(i) == 0)
      continue;
    VertexLabel _v_label = _vlabels[i];
    vlabels_map[_v_label].insert(i);
  }
}

void GRAPH::setVertexLabelMapCnt() {
  for (int i = 0; i < Vcnt; i++) {
    if (getDegree(i) == 0)
      continue;
    VertexLabel _v_label = _vlabels[i];
    vlabels_map_cnt[_v_label] = 0;
  }

  for (int i = 0; i < Vcnt; i++) {
    if (getDegree(i) == 0)
      continue;
    VertexLabel _v_label = _vlabels[i];
    vlabels_map_cnt[_v_label]++;
  }
}

int GRAPH::getMinTreeHeight2(VertexID start_v) {
  int _min_tree_height = 0;

  unordered_map<VertexID, int> map_hop;
  queue<VertexID> nodes;

  nodes.push(start_v);
  map_hop[start_v] = 1;

  // Begin BFS
  while (!nodes.empty()) {
    // for each node v
    VertexID v = nodes.front();
    nodes.pop();

    // for each incident node u of v
    for (int j = 0; j < getDegree(v); j++) {
      VertexID u = _adjList[v][j].v;

      // u is visited
      if (map_hop.find(u) != map_hop.end()) {
        continue;
      }

      // u is what we want,
      // add u to next_nodes for iteration
      nodes.push(u);
      map_hop[u] = map_hop[v] + 1;

      if (_min_tree_height < map_hop[u]) {
        _min_tree_height = map_hop[u];
      }
    }
    // nodes = next_nodes;
  }
  return _min_tree_height;
}

VertexID GRAPH::getMinTreeHeight() {
  min_tree_height = V() + 1;
  VertexID start_v;
  for (int i = 0; i < V(); i++) {
    int _min = getMinTreeHeight2(i);
    if (min_tree_height > _min) {
      start_v = i;
      min_tree_height = _min;
    }
  }
  return start_v;
}

void GRAPH::BFSwithConst(VertexID start_v, int hops, set<VertexID>& visit_v,
                         int& edge_cnt,
                         VertexLabelMapCnt& _vertex_label_map_cnt,
                         Cache& cache) {
//  cout << "start BFS new " << endl;
//  cout << "hop size " << hops << endl;

  VertexLabel _start_v_l = _vlabels[start_v];
  bool isSingle = false;
  if (_vertex_label_map_cnt[_start_v_l] == 1) {
    isSingle = true;
  }
  _vertex_label_map_cnt[_start_v_l]--;

  unordered_map<VertexID, int> map_hop;
  queue<int> nodes;

  visit_v.insert(start_v);
  nodes.push(start_v);
  map_hop[start_v] = 0;

  // for each node v
  while (!nodes.empty()) {
    int v = nodes.front();
    nodes.pop();

    if (map_hop[v] == hops)
      continue;

    // for each incident node u of v
    for (int j = 0; j < getDegree(v); j++) {
      int u = _adjList[v][j].v;

      // u is visited
      if (map_hop.find(u) != map_hop.end()) {
        continue;
      }

      // u's label is not contained
      VertexLabel _u_l = _vlabels[u];
      if (_vertex_label_map_cnt.find(_u_l) == _vertex_label_map_cnt.end()) {
        continue;
      }

      // single
      if (isSingle == true && _u_l == _start_v_l) {
        continue;
      }

      // u is the root vertex of previous matching
      if (cache.ifRootVertex.find(u) != cache.ifRootVertex.end()) {
        continue;
      }

      // update the label map cnt
      if (_vertex_label_map_cnt[_u_l] > 0) {
        _vertex_label_map_cnt[_u_l]--;
      }

      // u is what we want,
      // add u to next_nodes for iteration
      visit_v.insert(u);
      edge_cnt++;
      nodes.push(u);
      map_hop[u] = map_hop[v] + 1;
    }
  }
//  cout << "end BFS new" << endl;
}

void GRAPH::collectSimpleGraph(SIMPLEGRAPH& crv) {
  int new_id = 0;
  for (set<VertexID>::iterator it = crv.vertex_set.begin();
      it != crv.vertex_set.end(); it++) {
    VertexID u = *it;

    crv.label_to_vertex[getLabel(u)].insert(u);
    crv.old_to_new[u] = new_id;
    crv.new_to_old[new_id] = u;
    new_id++;

    // for each u's neighbor v
    for (int i = 0; i < getDegree(u); i++) {
      VertexID v = _adjList[u][i].v;

      // v is in set vertex
      if (u > v && crv.vertex_set.find(v) != crv.vertex_set.end()) {
        crv.edge_cnt++;
        crv.addDeg(u);
        crv.addDeg(v);
      }
    }
  }
}

void GRAPH::BFSwithConst(VertexID start_v, int hops, set<VertexID>& visit_v,
                         VertexLabelMapCnt& _vertex_label_map_cnt,
                         Cache& cache) {
//  cout << "start BFS new " << endl;
//  cout << "hop size " << hops << endl;

  VertexLabel _start_v_l = _vlabels[start_v];
  bool isSingle = false;
  if (_vertex_label_map_cnt[_start_v_l] == 1) {
    isSingle = true;
  }
  _vertex_label_map_cnt[_start_v_l]--;

  unordered_map<VertexID, int> map_hop;
  queue<int> nodes;

  visit_v.insert(start_v);
  nodes.push(start_v);
  map_hop[start_v] = 0;

  // for each node v
  while (!nodes.empty()) {
    int v = nodes.front();
    nodes.pop();

    if (map_hop[v] == hops)
      continue;

    // for each incident node u of v
    for (int j = 0; j < getDegree(v); j++) {
      int u = _adjList[v][j].v;

      // u is visited
      if (map_hop.find(u) != map_hop.end()) {
        continue;
      }

      // u's label is not contained
      VertexLabel _u_l = _vlabels[u];
      if (_vertex_label_map_cnt.find(_u_l) == _vertex_label_map_cnt.end()) {
        continue;
      }

      // single
      if (isSingle == true && _u_l == _start_v_l) {
        continue;
      }

      // u is the root vertex of previous matching
      if (cache.ifRootVertex.find(u) != cache.ifRootVertex.end()) {
        continue;
      }

      // update the label map cnt
      if (_vertex_label_map_cnt[_u_l] > 0) {
        _vertex_label_map_cnt[_u_l]--;
      }

      // u is what we want,
      // add u to next_nodes for iteration
      visit_v.insert(u);
      nodes.push(u);
      map_hop[u] = map_hop[v] + 1;
    }
  }
//  cout << "end BFS new" << endl;
}

void GRAPH::BFSwithSpecVcnt(VertexID start_v, int vcnt,
                            set<VertexID>& visit_v) {
  queue<VertexID> nodes;

  visit_v.insert(start_v);
  nodes.push(start_v);

  while (!nodes.empty()) {
    VertexID v = nodes.front();
    nodes.pop();

    for (int j = 0; j < getDegree(v); j++) {
      VertexID u = _adjList[v][j].v;

      // u is visited
      if (visit_v.find(u) != visit_v.end()) {
        continue;
      }

      visit_v.insert(u);

      // found vcnt nodes
      if (visit_v.size() == vcnt) {
        return;
      }

      nodes.push(u);
    }
  }
}

void GRAPH::BFS(VertexID start_v, int hops, set<VertexID>& visit_v) {
  cout << "start BFS" << endl;
  cout << "hop size " << hops << endl;
  visit_v.insert(start_v);

  unordered_map<VertexID, int> map_hop;
  queue<VertexID> nodes;
  nodes.push(start_v);
  map_hop[start_v] = 0;

  // for each node v
  while (!nodes.empty()) {
    VertexID v = nodes.front();
    nodes.pop();

    if (map_hop[v] == hops)
      continue;

    // for each incident node u of v
    for (int j = 0; j < getDegree(v); j++) {
      VertexID u = _adjList[v][j].v;

      // u is visited
      if (map_hop.find(u) != map_hop.end()) {
        continue;
      }

      // u is what we want,
      // add u to next_nodes for iteration
      visit_v.insert(u);
      nodes.push(u);
      map_hop[u] = map_hop[v] + 1;
    }
  }
  cout << "end BFS" << endl;
}

void GRAPH::getInducedSubGraph(set<VertexID>& vertex, GRAPH* _ind_g) {
  int _ind_g_s = vertex.size();
  _ind_g->setV(_ind_g_s);

  // set vertex
  VertexID _new_v_id = 0;
  unordered_map<VertexID, VertexID> g_to_ind_v;
  for (set<VertexID>::iterator it = vertex.begin(); it != vertex.end(); it++) {
    VertexID u = *it;
    VertexLabel u_l = getLabel(u);
    g_to_ind_v[u] = _new_v_id;
    _ind_g->setLabel(_new_v_id++, u_l);
  }

  // set edge
  int e_id = 0;
  for (set<VertexID>::iterator it = vertex.begin(); it != vertex.end(); it++) {
    VertexID u = *it;
    for (int i = 0; i < getDegree(u); i++) {
      VertexID v = _adjList[u][i].v;
      VertexLabel vl = _adjList[u][i].elabel;
      if (u <= v || vertex.find(v) == vertex.end())
        continue;

      _ind_g->insert(e_id++, g_to_ind_v[u], g_to_ind_v[v], vl);
    }
  }

  // set the vertex label map of the induced subgraph
  _ind_g->setVertexLabelMap();
  _ind_g->setVertexLabelMapCnt();
}

void GRAPH::getInducedSubGraph(vector<VertexID>& vertex, GRAPH* _ind_g) {
  int _ind_g_s = vertex.size();
  _ind_g->setV(_ind_g_s);

  // set vertex
  VertexID _new_v_id = 0;
  unordered_map<VertexID, VertexID> g_to_ind_v;
  for (vector<VertexID>::iterator it = vertex.begin(); it != vertex.end();
      it++) {
    VertexID u = *it;
    VertexLabel u_l = getLabel(u);
    g_to_ind_v[u] = _new_v_id;
    _ind_g->setLabel(_new_v_id++, u_l);
  }

  // set edge
  int e_id = 0;
  for (vector<VertexID>::iterator it = vertex.begin(); it != vertex.end();
      it++) {
    VertexID u = *it;
    for (int i = 0; i < getDegree(u); i++) {
      VertexID v = _adjList[u][i].v;
      VertexLabel vl = _adjList[u][i].elabel;
      if (u <= v || !isInVector(v, vertex))
        continue;

      _ind_g->insert(e_id++, g_to_ind_v[u], g_to_ind_v[v], vl);
    }
  }

  // set the vertex label map of the induced subgraph
  _ind_g->setVertexLabelMap();
  _ind_g->setVertexLabelMapCnt();
}

void GRAPH::getInducedSubGraph(set<VertexID>& vertex, GRAPH* _ind_g,
                               VertexID& r_vertex, STAT* _myStat) {
  int _ind_g_s = vertex.size();
  _ind_g->setV(_ind_g_s);
  VertexID _r_vertex = r_vertex;

  double _s = clock();
  // set vertex
  VertexID _new_v_id = 0;
  unordered_map<VertexID, VertexID> g_to_ind_v;
  for (set<VertexID>::iterator it = vertex.begin(); it != vertex.end(); it++) {
    VertexID u = *it;
    VertexLabel u_l = getLabel(u);
    g_to_ind_v[u] = _new_v_id;

    // re-set r_vertex
    if (u == _r_vertex)
      r_vertex = _new_v_id;

    _ind_g->setLabel(_new_v_id++, u_l);
  }
  double _e = clock();
  _myStat->cr_cont_time_1 += gettime(_s, _e);

  _s = clock();
  // set edge
  int e_id = 0;
  AdjElement _adje(0, 0, 0);
  for (set<VertexID>::iterator it = vertex.begin(); it != vertex.end(); it++) {
    VertexID u = *it;

    // for each u's neighbor v
    for (int i = 0; i < getDegree(u); i++) {
      VertexID v = _adjList[u][i].v;
      VertexLabel vl = _adjList[u][i].elabel;
      // u > v, and u is in set vertex
      if (u <= v || vertex.find(v) == vertex.end()) {
        continue;
      }
//      _ind_g->insert(e_id++, g_to_ind_v[u], g_to_ind_v[v], vl);
      _adje.eid = e_id;
      _adje.elabel = vl;
      _adje.v = g_to_ind_v[v];
      _ind_g->insert(g_to_ind_v[u], _adje);
      _adje.v = g_to_ind_v[u];
      _ind_g->insert(g_to_ind_v[v], _adje);
      _ind_g->Ecnt++;
      e_id++;
    }
  }
  _e = clock();
  _myStat->cr_cont_time_2 += gettime(_s, _e);

  // set the vertex label map of the induced subgraph
  _ind_g->setVertexLabelMap();
  _ind_g->setVertexLabelMapCnt();
}

void GRAPH::initM(int* M, GRAPH* g) {
  for (int i = 0; i < V(); i++) {
    VertexID u_q = i;
    VertexLabel u_q_l = getLabel(u_q);

    for (int j = 0; j < g->V(); j++) {
      VertexID u_g = j;
      VertexLabel u_g_l = g->getLabel(u_g);

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
//  cout << "=========== M ==============" << endl;
//  printMatrix(M, V());

  setArray(row, V(), 0);
  setArray(col, V(), -1);
  setArray(col1, V(), -1);
  setArray(row_col_next_one, V() * V(), 0);

  cnt_ones_of_row_function(g);
}

void GRAPH::initSubIso() {
  // begin of subIso
  M = new int[V() * V()];
  col = new int[V()];
  col1 = new int[V()];
  row = new int[V()];
  row_col_next_one = new VertexID[V() * V()];
  cnt_ones_of_row = new int[V()];
}

void GRAPH::clearSubIso() {
  // end of subIso
  delete[] M;
  delete[] col;
  delete[] col1;
  delete[] row;
  delete[] row_col_next_one;
  delete[] cnt_ones_of_row;
}

void GRAPH::resetEqvCls() {
  ASSERT(eqv_cls.size() > 0);
  ASSERT(eqv_cls_aux->NumElements() > 0);

  for (int i = 0; i < eqv_cls.size(); i++) {
    eqv_cls[i].clear();
  }

  eqv_cls_aux->MakeEmpty();
  eqv_cls_flg = false;
}

void GRAPH::clearEqvCls() {
  ASSERT(eqv_cls.size() > 0);
  ASSERT(eqv_cls_aux->NumElements() > 0);

  clearVectorSet(eqv_cls);
  delete eqv_cls_aux;
}

void GRAPH::initEqvCls(int _size) {
  ASSERT(_size > 0);

  eqv_cls.resize(_size);
  eqv_cls_aux = new DisjointSets(_size);
  eqv_cls_flg = false;
}

bool GRAPH::shareSameNeighbor(VertexID u, VertexID v,
                              SIMPLEGRAPH& simple_graph) {
  if (simple_graph.vertex_deg[u] == 0 || simple_graph.vertex_deg[v] == 0) {
    return false;
  }

  if (simple_graph.vertex_deg[u] != simple_graph.vertex_deg[v]) {
    return false;
  }

  for (int i = 0; i < getDegree(u); i++) {
    VertexID nu = _adjList[u][i].v;
    if (!simple_graph.vIsIn(nu)) {
      continue;
    }

    bool flg = false;
    for (int j = 0; j < getDegree(v); j++) {
      VertexID nv = _adjList[v][j].v;
      if (!simple_graph.vIsIn(nv)) {
        continue;
      }

      if (nu == nv || nu == v) {
        flg = true;
        break;
      }
    }

    if (flg == false) {
      return false;
    }
  }

  return true;
}

bool GRAPH::shareSameNeighbor(VertexID u, VertexID v) {
  if (getDegree(u) == 0 || getDegree(v) == 0) {
    return false;
  }

  if (getDegree(u) != getDegree(v)) {
    return false;
  }

  for (int i = 0; i < getDegree(u); i++) {
    VertexID nu = _adjList[u][i].v;
    bool flg = false;
    for (int j = 0; j < getDegree(v); j++) {
      VertexID nv = _adjList[v][j].v;
      if (nu == nv || nu == v) {
        flg = true;
        break;
      }
    }
    if (flg == false) {
      return false;
    }
  }

  return true;
}

void GRAPH::updateEqvCls(VertexID u, VertexID v) {
  eqv_cls[u].insert(v);
  eqv_cls[v].insert(u);

  // if u and v share the same equivalent class
  if (twoSetsAreEqual(eqv_cls[u], eqv_cls[v])) {
    return;
  }

  for (set<VertexID>::iterator it = eqv_cls[u].begin(); it != eqv_cls[u].end();
      it++) {
    if (*it != v && *it != u) {
      updateEqvCls(*it, v);
    }
  }

  for (set<VertexID>::iterator it = eqv_cls[v].begin(); it != eqv_cls[v].end();
      it++) {
    if (*it != u && *it != v) {
//      cout << "*it: " << *it << ", u: " << u << endl;
      updateEqvCls(*it, u);
    }
  }
}

void GRAPH::reduceByEqvCls(VertexID& old_r_vertex, SIMPLEGRAPH& simple_graph,
                           VertexLabelMapCnt& _vertex_label_map_cnt) {
  ASSERT(eqv_cls.size() == simple_graph.vertex_set.size());
  ASSERT(eqv_cls_aux->NumElements() == simple_graph.vertex_set.size());

  VertexID new_r_vertex = simple_graph.old_to_new[old_r_vertex];

  unordered_map<VertexID, set<VertexID> > _map;
  for (set<VertexID>::iterator it = simple_graph.vertex_set.begin();
      it != simple_graph.vertex_set.end(); it++) {
    VertexID old_u = *it;
    VertexID new_u = simple_graph.old_to_new[old_u];

    _map[eqv_cls_aux->FindSet(new_u)].insert(new_u);
  }

  // for each disjoint set
  for (unordered_map<VertexID, set<VertexID> >::iterator it = _map.begin();
      it != _map.end(); it++) {
    VertexID _root = it->first;
    set<VertexID>& _ds = it->second;
    int _lable_cnt = _vertex_label_map_cnt[getLabel(_root)];
    bool _is = false;

    // disjoint set size is smaller
    if (_ds.size() < _lable_cnt)
      continue;

    // r_vertex is in this disjoint set
    if (_ds.find(new_r_vertex) != _ds.end())
      _is = true;

    // remove
    int _rm_cnt = 0;
    for (set<VertexID>::iterator it1 = _ds.begin(); it1 != _ds.end(); it1++) {
      VertexID new_u = *it1;
      VertexID old_u = simple_graph.new_to_old[new_u];

      if (_ds.size() - _rm_cnt == _lable_cnt) {
        break;
      } else {

        if (new_u == _root)
          continue;

        // remove all u's edges
        simple_graph.remove_vertex(old_u);

        // update
        eqv_cls_flg = true;

        // removed
        _rm_cnt++;
      }
    }

    // re-set r_vertex
    if (_is) {
      old_r_vertex = simple_graph.new_to_old[_root];
    }
  }
}

void GRAPH::reduceByEqvCls(VertexID& r_vertex,
                           VertexLabelMapCnt& _vertex_label_map_cnt) {
  ASSERT(eqv_cls.size() == V());
  ASSERT(eqv_cls_aux->NumElements() == V());

  unordered_map<VertexID, set<VertexID> > _map;
  for (int i = 0; i < V(); i++) {
    _map[eqv_cls_aux->FindSet(i)].insert(i);
  }

  // for each disjoint set
  for (unordered_map<VertexID, set<VertexID> >::iterator it = _map.begin();
      it != _map.end(); it++) {
    VertexID _root = it->first;
    set<VertexID>& _ds = it->second;
    int _lable_cnt = _vertex_label_map_cnt[getLabel(_root)];
    bool _is = false;

    // disjoint set size is smaller
    if (_ds.size() < _lable_cnt)
      continue;

    // r_vertex is in this disjoint set
    if (_ds.find(r_vertex) != _ds.end())
      _is = true;

    // remove edges
    int _rm_cnt = 0;
    for (set<VertexID>::iterator it1 = _ds.begin(); it1 != _ds.end(); it1++) {
      if (_ds.size() - _rm_cnt == _lable_cnt) {
        break;
      } else {
        VertexID u = *it1;
        if (u == _root)
          continue;

        // remove all u's edges
        removeVexAllEdges(u);

        // update
        eqv_cls_flg = true;

        // removed
        _rm_cnt++;
      }
    }

    // re-set r_vertex
    if (_is) {
      r_vertex = _root;
    }
  }
}

void GRAPH::genEqvCls(SIMPLEGRAPH& simple_g) {
  set<VertexID>& vertex_set = simple_g.vertex_set;

  ASSERT(eqv_cls.size() == vertex_set.size());
  ASSERT(eqv_cls_aux->NumElements() == vertex_set.size());

  // initialize eqv_cls
  for (set<VertexID>::iterator it = vertex_set.begin(); it != vertex_set.end();
      it++) {
    VertexID new_u = simple_g.old_to_new[*it];
    eqv_cls[new_u].insert(new_u);
  }

  // bigO(Deg*V)
  for (set<VertexID>::iterator it = vertex_set.begin(); it != vertex_set.end();
      it++) {
    VertexID old_u = *it;
    VertexID new_u = simple_g.old_to_new[old_u];

    for (int j = 0; j < getDegree(old_u); j++) {
      VertexID old_v = _adjList[old_u][j].v;
      VertexID new_v = simple_g.old_to_new[old_v];

      if (!simple_g.vIsIn(old_v)) {
        continue;
      }

      // u and v share the same label
      // note the order
      if (old_u > old_v || getLabel(old_u) != getLabel(old_v)) {
        continue;
      }

      // u and v share the same neighbor vertexes
      if (shareSameNeighbor(old_u, old_v, simple_g)) {
//        updateEqvCls(u, v);
        eqv_cls_aux->Union(eqv_cls_aux->FindSet(new_u),
                           eqv_cls_aux->FindSet(new_v));
      } else {
        continue;
      }
    }
  }

  // collect equivalent class
  unordered_map<VertexID, set<VertexID> > _map;
  for (set<VertexID>::iterator it = vertex_set.begin(); it != vertex_set.end();
      it++) {
    VertexID old_u = *it;
    VertexID new_u = simple_g.old_to_new[old_u];
    _map[eqv_cls_aux->FindSet(new_u)].insert(new_u);
  }

  for (set<VertexID>::iterator it = vertex_set.begin(); it != vertex_set.end();
      it++) {
    VertexID old_u = *it;
    VertexID new_u = simple_g.old_to_new[old_u];
    for (set<VertexID>::iterator it1 =
        _map[eqv_cls_aux->FindSet(new_u)].begin();
        it1 != _map[eqv_cls_aux->FindSet(new_u)].end(); it1++) {
      VertexID new_v = *it1;
      eqv_cls[new_u].insert(new_v);
    }
  }

}

void GRAPH::genEqvCls() {
  ASSERT(eqv_cls.size() == V());
  ASSERT(eqv_cls_aux->NumElements() == V());

  // initialize eqv_cls
  for (int i = 0; i < V(); i++) {
    eqv_cls[i].insert(i);
  }

  // for each label _l
  for (VertexLabelMapCnt::iterator it = vlabels_map_cnt.begin();
      it != vlabels_map_cnt.end(); it++) {
    VertexLabel _l = it->first;

    set<VertexID>& _set = vlabels_map[_l];

    // for each vertex u
    for (set<VertexID>::iterator it1 = _set.begin(); it1 != _set.end(); it1++) {
      VertexID u = *it1;

      // for each vertex v
      set<VertexID>::iterator it2 = it1;
      for (it2++; it2 != _set.end(); it2++) {
        VertexID v = *it2;
        if (getLabel(u) != getLabel(v))
          continue;

        // u and v share the same neighbor vertexes
        if (shareSameNeighbor(u, v)) {
          //        updateEqvCls(u, v);
          eqv_cls_aux->Union(eqv_cls_aux->FindSet(u), eqv_cls_aux->FindSet(v));
        } else {
          continue;
        }

      }
    }
  }

  // collect equivalent class
  unordered_map<VertexID, set<VertexID> > _map;
  for (int i = 0; i < V(); i++) {
    _map[eqv_cls_aux->FindSet(i)].insert(i);
  }

  for (int i = 0; i < V(); i++) {
    for (set<VertexID>::iterator it = _map[eqv_cls_aux->FindSet(i)].begin();
        it != _map[eqv_cls_aux->FindSet(i)].end(); it++) {
      eqv_cls[i].insert(*it);
    }
  }

}

void GRAPH::isSubgraphOf2e(GRAPH* g) {
//  cout << "=========== query vertex ==============" << endl;
//  printArray(col1, V());

  msg->resetRi();

  double _s = clock();
  // real thing with encrypted
  for (int i = 0; i < V(); i++) {
    for (int j = i + 1; j < V(); j++) {
      VertexID _i = col[i];
      VertexID _j = col[j];

      if (V() < DEFAULTENCODING) {
        // * multiplications
        if (!g->edge(_i, _j)) {
          cgbe->mul(msg->Ri, msg->Ri, Mq->matrix[i][j]);
        } else {
          cgbe->mul(msg->Ri, msg->Ri, cgbe->I);
        }
      } else {
        // additions
        if (!g->edge(_i, _j)) {
          cgbe->add(msg->Ri, msg->Ri, Mq->matrix[i][j]);
        } else {
          cgbe->add(msg->Ri, msg->Ri, cgbe->I);
        }
      }
    }
  }

  // aggregate
  if (V() < DEFAULTENCODING) {
    // * additions
    cgbe->add(msg->Rk, msg->Rk, msg->Ri);
  } else {
    // multiplications
    cgbe->mul(msg->Rk, msg->Rk, msg->Ri);
  }
  double _e = clock();
  myStat->mul_add_time += gettime(_s, _e);

  msg->cnt++;

  // decryption
  _s = clock();
  decrypt();
  _e = clock();
  myStat->decrypt_time += gettime(_s, _e);
}

void GRAPH::isSubgraphOf2(GRAPH* g) {
//  cout << "=========== query vertex ==============" << endl;
//  printArray(col1, V());

  // real thing with plain text
  for (int i = 0; i < V(); i++) {
    for (int j = 0; j < V(); j++) {
      if (i != j && edge(i, j)) {
        VertexID _i = col[i];
        VertexID _j = col[j];
        if (!g->edge(_i, _j)) {
          return;
        }
      }
    }
  }
  msg->answer = 1;
}

void GRAPH::isSubgraphOf1(int dep, GRAPH* g) {
  // early stop
  if (msg->answer == 1) {
    return;
  }

  // okay
  if (dep == V()) {
    myStat->red_psb_map_cnt++;
    double _s = clock();
    isSubgraphOf2e(g);
    double _e = clock();
    myStat->isSubgraphOf2e_time += gettime(_s, _e);

    return;
  }

  // enumeration
  for (int i = 0; i < cnt_ones_of_row[dep]; i++) {
    // for each vertex of g.
    int j = row_col_next_one[MATRIX_INDEX(dep, i, V())];

    // j is not covered
    if (row[j] == 0) {
      // ------------- use equivalent class ---------------
      if (g->eqv_cls[j].size() > 1) {
        bool flg = true;
        for (set<VertexID>::iterator it = g->eqv_cls[j].begin();
            it != g->eqv_cls[j].end(); it++) {
          // exists un-covered vertex *it s.t.
          // (1) *it is equivalent to j;
          // (2) order, i.e., *it < j.
          if (*it < j) {
            if (row[*it] == 0) {
              flg = false;
              break;
            }
          } else {
            break;
          }
        }

        if (flg == false) {
          continue;
        }
      }
      // -------------------------------------------------

      // move on
      row[j] = 1;
      col[dep] = j;
      col1[j] = dep;
      isSubgraphOf1(dep + 1, g);
      row[j] = 0;
      col[dep] = col1[j] = -1;
    }
  }
}

/**
 * q and g are of the same size
 * a very naive solution for
 * preserving the privacy of q.
 *
 * ps: q's structure is not known.
 */
bool GRAPH::isSubgraphOf(GRAPH* g) {
  msg->answer = 0;

  // org psb map cnt
  myStat->org_psb_map_cnt += sumUpVertexLabelCnt(g->vlabels_map_cnt,
                                                 g->vlabels_map_cnt);

  isSubgraphOf1(0, g);

  // last decryption
  if (msg->cnt > 0) {
    double _s = clock();
    finalDecrypt();
    double _e = clock();
    myStat->decrypt_time += gettime(_s, _e);
  }

  return (msg->answer == 1);
}

bool GRAPH::isSubgrpahOfByVF2(GRAPH* g) {
  NodeCompare node_compare;

  int n;

  ARGEdit iso_test_ed;
  ARGEdit sub_iso_test_ed;

  for (VertexID i = 0; i < V(); i++) {
    sub_iso_test_ed.InsertNode((void*) (getLabel(i)));
  }
  for (VertexID i = 0; i < V(); i++) {
    for (VertexID j = i + 1; j < V(); j++) {
      if (edge(i, j)) {
        sub_iso_test_ed.InsertEdge(i, j, (void*) (getELabel(i, j)));
        sub_iso_test_ed.InsertEdge(j, i, (void*) (getELabel(j, i)));
      }
    }
  }

  Graph sub_iso_test_graph(&sub_iso_test_ed);
  sub_iso_test_graph.SetNodeComparator(&node_compare);
  sub_iso_test_graph.SetEdgeComparator(&node_compare);

  for (VertexID i = 0; i < g->V(); i++) {
    iso_test_ed.InsertNode((void*) (g->getLabel(i)));
  }
  for (VertexID i = 0; i < g->V(); i++) {
    for (VertexID j = i + 1; j < g->V(); j++) {
      if (g->edge(i, j)) {
        iso_test_ed.InsertEdge(i, j, (void*) (g->getELabel(i, j)));
        iso_test_ed.InsertEdge(j, i, (void*) (g->getELabel(j, i)));
      }
    }
  }

  Graph iso_test_graph(&iso_test_ed);
  iso_test_graph.SetNodeComparator(&node_compare);
  iso_test_graph.SetEdgeComparator(&node_compare);

  VF2MonoState sub_state(&sub_iso_test_graph, &iso_test_graph);
  node_id ni1[g->maximum_vertex];
  node_id ni2[g->maximum_vertex];

  if (match(&sub_state, &n, ni1, ni2)) {
    return true;
  }

  return false;
}

void GRAPH::removeVexAllEdges(VertexID u) {
  Ecnt -= _adjList[u].size();

  for (int i = 0; i < _adjList[u].size(); i++) {
    VertexID w = _adjList[u][i].v;

    for (vector<AdjElement>::iterator it = _adjList[w].begin();
        it != _adjList[w].end(); it++) {
      if ((*it).v == u) {
        _adjList[w].erase(it);
        break;
      }
    }
  }

  _adjList[u].clear();
}

void GRAPH::removeEdge(VertexID v, VertexID w) {
  for (vector<AdjElement>::iterator it = _adjList[v].begin();
      it != _adjList[v].end(); it++) {
    if ((*it).v == w) {
      _adjList[v].erase(it);
      break;
    }
  }

  for (vector<AdjElement>::iterator it = _adjList[w].begin();
      it != _adjList[w].end(); it++) {
    if ((*it).v == v) {
      _adjList[w].erase(it);
      break;
    }
  }
}

void GRAPH::clientPreProcess() {
  /**
   * preprocessing at the client side.
   * 1. encryption
   * 2. determine l_s and h
   */
  // initialize encryption
  encryptInit();
  // encryption
  encrypt();

  // set h and l_s
  start_label = getLabel(getMinTreeHeight());

  // init stat
  myStat = new STAT();
}

void GRAPH::encryptInit() {
  // init Mq
  Mq = new BigMatrix(V(), V());
  cgbe = new CGBE();
  msg = new Message(V());

  // set values of Mq
  for (int i = 0; i < V(); i++) {
    VertexID u = i;
    for (int j = 0; j < V(); j++) {
      VertexID v = j;

      if (V() < DEFAULTENCODING) {
        if (edge(u, v)) {
          // is edge, 1
          Mq->setValue(u, v, cgbe->encoding);
        } else {
          // is not edge
          Mq->setValue(u, v, 1);
        }
      } else {
        if (edge(u, v)) {
          Mq->setValue(u, v, 1);
        } else {
          Mq->setValue(u, v, cgbe->encoding);
        }
      }
    }
  }

  // set value I
  if (V() < DEFAULTENCODING) {
    cgbe->setvalue(cgbe->I, 1);
  } else {
    cgbe->setvalue(cgbe->I, cgbe->encoding);
  }
}

void GRAPH::encrypt() {
  ASSERT(Mq != NULL);
  ASSERT(cgbe != NULL);

  // encrypt Mq
  for (int i = 0; i < V(); i++) {
    for (int j = 0; j < V(); j++) {
      cgbe->encrypt(Mq->matrix[i][j], Mq->matrix[i][j]);
    }
  }

  // encrypt I
  if (V() < DEFAULTENCODING) {
    cgbe->encrypt(cgbe->I, cgbe->I);
  } else {
    cgbe->encrypt(cgbe->I, cgbe->I);
  }
}

void GRAPH::finalDecrypt() {
  if (V() < DEFAULTENCODING) {
    cgbe->decrypt(msg->Rk, msg->R, (V() * (V() - 1)) / 2);
    msg->resetRk();
    msg->cnt = 0;

    if (!cgbe->isZero(msg->R)) {
      msg->answer = 1;
    }
  } else {
    cgbe->decrypt(msg->Rk, msg->R, msg->cnt);
    msg->cnt = 0;
    msg->resetRk();

    if (cgbe->isZero(msg->R)) {
      msg->answer = 1;
    }
  }

  myStat->encypted_msg_cnt++;
}

void GRAPH::decrypt() {
  if (V() < DEFAULTENCODING) {
    if (msg->cnt >= DEFAULTAGGREGATES) {
      cgbe->decrypt(msg->Rk, msg->R, (V() * (V() - 1)) / 2);
      msg->resetRk();
      msg->cnt = 0;

      myStat->encypted_msg_cnt++;

      if (!cgbe->isZero(msg->R)) {
        msg->answer = 1;
      }
    }
  } else {
    if (msg->cnt >= DEFAULTAGGREGATE) {
      cgbe->decrypt(msg->Rk, msg->R, msg->cnt);
      msg->cnt = 0;
      msg->resetRk();

      myStat->encypted_msg_cnt++;

      if (cgbe->isZero(msg->R)) {
        msg->answer = 1;
      }
    }
  }
}

void GRAPH::encryptFree() {
  delete Mq;
  delete cgbe;
  delete msg;
  delete myStat;
}
