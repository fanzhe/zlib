/*
 * DiGraph.cpp
 *
 *  Created on: 2014¦~8¤ë1¤é
 *      Author: zfan
 */

#include "DiGraph.h"

#ifndef DEFAULT_VERTEX_NUMBER
#define DEFAULT_VERTEX_NUMBER   (256)
#endif

DIGRAPH::DIGRAPH()
    : Vcnt(0),
      Ecnt(0),
      graphId(INVALID_GRAPH_ID) {
  _vlabels = new VertexLabel[DEFAULT_VERTEX_NUMBER];
  _outEdges.resize(DEFAULT_VERTEX_NUMBER);
  _outVertex.resize(DEFAULT_VERTEX_NUMBER);
  _inVertex.resize(DEFAULT_VERTEX_NUMBER);

  max_vcnt = DEFAULT_VERTEX_NUMBER;

  for (int i = 0; i < DEFAULT_VERTEX_NUMBER; i++) {
    _vlabels[i] = NO_LABEL;
  }
}

DIGRAPH::DIGRAPH(int V)
    : Vcnt(V),
      Ecnt(0),
      graphId(INVALID_GRAPH_ID) {
  _vlabels = new VertexLabel[V];
  _outEdges.resize(V);
  _outVertex.resize(V);
  _inVertex.resize(V);

  max_vcnt = V;

  for (int i = 0; i < V; i++) {
    _vlabels[i] = NO_LABEL;
  }
}

DIGRAPH::~DIGRAPH() {
  delete[] _vlabels;
  _outEdges.clear();
  _outVertex.clear();
  _inVertex.clear();
}

int DIGRAPH::getVcnt() {
  return Vcnt;
}

int DIGRAPH::getEcnt() {
  return Ecnt;
}

void DIGRAPH::resetVcnt(int V) {
  if (max_vcnt > V) {
    Vcnt = V;
  } else {
    max_vcnt = V;
    int prev_Vcnt = getVcnt();

    Vcnt = V;

    delete[] _vlabels;
    _vlabels = new VertexLabel[V];
    for (int i = 0; i < V; i++) {
      _vlabels[i] = NO_LABEL;
    }

    for (int i = 0; i < prev_Vcnt; i++) {
      _outEdges[i].clear();
      _outVertex[i].clear();
      _inVertex[i].clear();
    }
    _outEdges.clear();
    _outVertex.clear();
    _inVertex.clear();

    _outEdges.resize(V);
    _outVertex.resize(V);
    _inVertex.resize(V);
  }
}

void DIGRAPH::setVLabel(VertexID v, VertexLabel label) {
  ASSERT(v < max_vcnt);
  _vlabels[v] = label;
}

void DIGRAPH::insertEdge(Edge e) {
  VertexID v = e.v, w = e.w;
  Ecnt++;
  _outEdges[v].push_back(AdjElement(w, e.edge_id, e.label));
  _outVertex[v][w] = true;  // out vertex
  _inVertex[w][v] = true;  // in vertex
}

VertexLabel DIGRAPH::getVLabel(VertexID v) {
  return _vlabels[v];
}

int DIGRAPH::getInDegree(VertexID v) {
//  return _vInDeg[v];
  return _inVertex[v].size();
}

int DIGRAPH::getOutDegree(VertexID v) {
  ASSERT(v < Vcnt);
  return (int) _outEdges[v].size();
}

void DIGRAPH::printGraphNew(ostream& out) {
  cout << "vid " << "out " << "in " << endl;
  for (int i = 0; i < getVcnt(); i++) {
    VertexID v = i;
    out << v << " " << getOutDegree(v) << " " << getInDegree(v) << endl;
  }

  cout << "src " << "vl " << "el " << "dest " << "vl " << endl;
  for (int i = 0; i < getVcnt(); i++) {
    VertexID s = i;
    VertexLabel sl = getVLabel(s);

    for (int j = 0; j < getOutDegree(s); j++) {
      VertexID d = _outEdges[s][j].v;
      VertexLabel dl = getVLabel(d);

      EdgeLabel el = _outEdges[s][j].elabel;
      out << s << " " << sl << " " << el << " " << d << " " << dl << endl;
    }
  }
}

void DIGRAPH::constLabelCnt() {
  _outLabelCnt.clear();
  _inLabelCnt.clear();
  _outLabelCnt.resize(getVcnt());
  _inLabelCnt.resize(getVcnt());

  for (int i = 0; i < getVcnt(); i++) {
    VertexID s = i;
    VertexLabel sl = getVLabel(s);

    for (int j = 0; j < getOutDegree(s); j++) {
      VertexID d = _outEdges[s][j].v;
      VertexLabel dl = getVLabel(d);
//      EdgeLabel el = _outEdges[s][j].elabel;

      // out label
      if (_outLabelCnt[s].find(dl) == _outLabelCnt[s].end()) {
        _outLabelCnt[s][dl] = 1;
      } else {
        _outLabelCnt[s][dl]++;
      }

      // in label
      if (_inLabelCnt[d].find(sl) == _inLabelCnt[d].end()) {
        _inLabelCnt[d][sl] = 1;
      } else {
        _inLabelCnt[d][sl]++;
      }
    }
  }
}

void DIGRAPH::initEL() {
  constLabelCnt();
}
