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
}

DIGRAPH::~DIGRAPH() {
  _vlabels.clear();
  _outEdges.clear();
  _inVertex.clear();
}

int DIGRAPH::getVcnt() {
  return Vcnt;
}

int DIGRAPH::getEcnt() {
  return Ecnt;
}

bool DIGRAPH::isEdge(VertexID s, VertexID d) {
  ASSERT(isVertex(s));
  ASSERT(isVertex(d));
  return (_outEdges[s].find(d) != _outEdges[s].end());
}

bool DIGRAPH::isVertex(VertexID v) {
  return (_vlabels.find(v) != _vlabels.end());
}

VLabels& DIGRAPH::getVLabel() {
  return _vlabels;
}

OutEdge& DIGRAPH::getOutEdge() {
  return _outEdges;
}

InVertex& DIGRAPH::getInVertex() {
  return _inVertex;
}

void DIGRAPH::reset() {
  _vlabels.clear();
  _outEdges.clear();
  _inVertex.clear();
}

void DIGRAPH::insertVertex(VertexID v, VertexLabel label) {
  Vcnt++;
  _vlabels[v] = label;
}

void DIGRAPH::insertEdge(VertexID s, VertexID d, EdgeLabel el) {
  Ecnt++;
  _outEdges[s][d] = AdjElement(s, Ecnt, el);
  _inVertex[d][s] = true;
}

void DIGRAPH::insertEdge(Edge e) {
  VertexID s = e.v, d = e.w;
  Ecnt++;
  _outEdges[s][d] = AdjElement(d, e.edge_id, e.label);
  _inVertex[d][s] = true;  // in vertex
}

void DIGRAPH::setVLabel(VertexID v, VertexLabel label) {
  ASSERT(isVertex(v));
  _vlabels[v] = label;
}

void DIGRAPH::setELabel(VertexID s, VertexID d, EdgeLabel el) {
  ASSERT(isEdge(s, d));
  _outEdges[s][d].elabel = el;
}

void DIGRAPH::removeEdge(VertexID s, VertexID d, bool verify = true) {
  if (verify)
    ASSERT(isEdge(s, d));
  Ecnt--;
//   remove out edge
  _outEdges[s].erase(d);

// remove in vertex
  _inVertex[d].erase(s);
}

void DIGRAPH::removeAllEdges(VertexID s) {
  ASSERT(isVertex(s));

  for (AdjList::iterator it = getOutEdge()[s].begin(); it != _outEdges[s].end();
      it++) {
    VertexID d = it->first;
    removeEdge(s, d, false);
  }
}

VertexLabel DIGRAPH::getVLabel(VertexID v) {
  ASSERT(isVertex(v));
  return _vlabels[v];
}

EdgeLabel DIGRAPH::getELabel(VertexID s, VertexID d) {
  ASSERT(isEdge(s, d));
  return _outEdges[s][d].elabel;
}

int DIGRAPH::getInDegree(VertexID v) {
  ASSERT(isVertex(v));
  return (int) _inVertex[v].size();
}

int DIGRAPH::getOutDegree(VertexID v) {
  ASSERT(isVertex(v));
  return (int) _outEdges[v].size();
}

void DIGRAPH::printGraph(ostream& out) {
  out << "Vcnt " << "Ecnt " << endl;
  out << getVcnt() << " " << getEcnt() << " " << endl;

  out << "vid " << "out " << "in " << endl;
  for (VLabels::iterator it = getVLabel().begin(); it != getVLabel().end();
      it++) {
    VertexID v = it->first;
    out << v << " " << getOutDegree(v) << " " << getInDegree(v) << endl;
  }

  out << "eid " << "src " << "vl " << "el " << "dest " << "vl " << endl;
  for (VLabels::iterator it = getVLabel().begin(); it != getVLabel().end();
      it++) {
    VertexID s = it->first;
    VertexLabel sl = getVLabel(s);

    for (AdjList::iterator it1 = getOutEdge()[s].begin();
        it1 != getOutEdge()[s].end(); it1++) {
      VertexID d = it1->first;
      VertexLabel dl = getVLabel(d);

      EdgeID eid = it1->second.eid;
      EdgeLabel el = it1->second.elabel;
      out << eid << " " << s << " " << sl << " " << el << " " << d << " " << dl
          << endl;
    }
  }
}

void DIGRAPH::constLabelCnt() {
  _outLabelCnt.clear();
  _inLabelCnt.clear();

  for (VLabels::iterator it = getVLabel().begin(); it != getVLabel().end();
      it++) {
    VertexID s = it->first;
    VertexLabel sl = getVLabel(s);

    for (AdjList::iterator it1 = getOutEdge()[s].begin();
        it1 != _outEdges[s].end(); it1++) {
      VertexID d = it1->first;
      VertexLabel dl = getVLabel(d);

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

void DIGRAPH::initEdgeVisited() {
  for (VLabels::iterator it = getVLabel().begin(); it != getVLabel().end();
      it++) {
    VertexID s = it->first;
    for (AdjList::iterator it1 = getOutEdge()[s].begin();
        it1 != getOutEdge()[s].end(); it1++) {
      it1->second.isVisited = false;
    }
  }
}

void DIGRAPH::initEL(VertexID x) {
  e = x;
  constLabelCnt();
}
