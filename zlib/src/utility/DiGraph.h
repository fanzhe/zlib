/*
 * DiGraph.h
 *
 *  Created on: 2014¦~8¤ë1¤é
 *      Author: zfan
 */

#ifndef DIGRAPH_H_
#define DIGRAPH_H_

#include "GlobalDefinition.h"
#include <vector>
#include <tr1/unordered_map>

using namespace std;

template<class VLabelType, class ELabelType>
class DIGRAPH {
 public:
  /**
   * Adj definition.
   */
  template<class ELableType>
  class AdjElement {
   public:
    VertexID v;
    EdgeID eid;
    ELableType elabel;
    /**
     * used and initialized in EL
     */
    bool isVisited;

    AdjElement() {
      v = eid = elabel = -1;
    }

    AdjElement(VertexID v, EdgeID eid, const ELableType& _elabel)
        : v(v),
          eid(eid),
          elabel(_elabel) {
    }

    ~AdjElement() {

    }
  };
  /*
   * data structures for basic operations
   */

  typedef unordered_map<VertexID, VLabelType> VLabels;
  typedef AdjElement<ELabelType> ADJELE;
  typedef unordered_map<VertexID, ADJELE> AdjList;
  typedef unordered_map<VertexID, AdjList> OutEdge;
  typedef unordered_map<VertexID, bool> AdjListBool;
  typedef unordered_map<VertexID, AdjListBool> InVertex;

  GraphID graphId;
  int Vcnt, Ecnt;
  VLabels _vlabels;  // vertex label
  OutEdge _outEdges;  // out edge list
  InVertex _inVertex;  // in vertex

  /*
   * basic operations
   */
  DIGRAPH();
  ~DIGRAPH();
  void reset();

  VLabels& getVLabel();
  OutEdge& getOutEdge();
  InVertex& getInVertex();
  int getVcnt();
  int getEcnt();

  bool isEdge(VertexID s, VertexID d);
  bool isVertex(VertexID v);
  void insertVertex(VertexID v, const VLabelType& label);
  void insertEdge(VertexID s, VertexID d, const ELabelType& el);
  void setVLabel(VertexID v, VLabelType& label);
  void setELabel(VertexID s, VertexID d, ELabelType& el);
  const VLabelType& getVLabel(VertexID v);
  const ELabelType& getELabel(VertexID s, VertexID d);
  void removeEdge(VertexID s, VertexID d, bool verify);
  void removeAllEdges(VertexID s);
  int getOutDegree(VertexID v);
  int getInDegree(VertexID v);
  void printGraph(ostream& out);

  /*
   * data structures for subIso
   */

  /*
   * data structures for simulation
   */

  /*
   * data structures for EL algorithm
   */
  VertexID e;
  MapLabelCnt _outLabelCnt;
  MapLabelCnt _inLabelCnt;

  void initEL(VertexID x);
  void initEdgeVisited();
  void constLabelCnt();

};

/**
 * implementations
 */

#ifndef DEFAULT_VERTEX_NUMBER
#define DEFAULT_VERTEX_NUMBER   (256)
#endif

template<class VLabelType, class ELabelType>
DIGRAPH<VLabelType, ELabelType>::DIGRAPH()
    : Vcnt(0),
      Ecnt(0),
      graphId(INVALID_GRAPH_ID) {
}

template<class VLabelType, class ELabelType>
DIGRAPH<VLabelType, ELabelType>::~DIGRAPH() {
  _vlabels.clear();
  _outEdges.clear();
  _inVertex.clear();
}

template<class VLabelType, class ELabelType>
int DIGRAPH<VLabelType, ELabelType>::getVcnt() {
  return Vcnt;
}

template<class VLabelType, class ELabelType>
int DIGRAPH<VLabelType, ELabelType>::getEcnt() {
  return Ecnt;
}

template<class VLabelType, class ELabelType>
bool DIGRAPH<VLabelType, ELabelType>::isEdge(VertexID s, VertexID d) {
  ASSERT(isVertex(s));
  ASSERT(isVertex(d));
  return (_outEdges[s].find(d) != _outEdges[s].end());
}

template<class VLabelType, class ELabelType>
bool DIGRAPH<VLabelType, ELabelType>::isVertex(VertexID v) {
  return (_vlabels.find(v) != _vlabels.end());
}

template<class VLabelType, class ELabelType>
typename DIGRAPH<VLabelType, ELabelType>::VLabels& DIGRAPH<VLabelType,
    ELabelType>::getVLabel() {
  return _vlabels;
}

template<class VLabelType, class ELabelType>
typename DIGRAPH<VLabelType, ELabelType>::OutEdge& DIGRAPH<VLabelType,
    ELabelType>::getOutEdge() {
  return _outEdges;
}

template<class VLabelType, class ELabelType>
typename DIGRAPH<VLabelType, ELabelType>::InVertex& DIGRAPH<VLabelType,
    ELabelType>::getInVertex() {
  return _inVertex;
}

template<class VLabelType, class ELabelType>
void DIGRAPH<VLabelType, ELabelType>::reset() {
  _vlabels.clear();
  _outEdges.clear();
  _inVertex.clear();
}

template<class VLabelType, class ELabelType>
void DIGRAPH<VLabelType, ELabelType>::insertVertex(VertexID v,
                                                   const VLabelType& label) {
  Vcnt++;
  _vlabels[v] = label;
}

template<class VLabelType, class ELabelType>
void DIGRAPH<VLabelType, ELabelType>::insertEdge(VertexID s, VertexID d,
                                                 const ELabelType& el) {
  Ecnt++;
  _outEdges[s][d] = AdjElement<ELabelType>(s, Ecnt, el);
  _inVertex[d][s] = true;
}

template<class VLabelType, class ELabelType>
void DIGRAPH<VLabelType, ELabelType>::setVLabel(VertexID v, VLabelType& label) {
  ASSERT(isVertex(v));
  _vlabels[v] = label;
}

template<class VLabelType, class ELabelType>
void DIGRAPH<VLabelType, ELabelType>::setELabel(VertexID s, VertexID d,
                                                ELabelType& el) {
  ASSERT(isEdge(s, d));
  _outEdges[s][d].elabel = el;
}

template<class VLabelType, class ELabelType>
void DIGRAPH<VLabelType, ELabelType>::removeEdge(VertexID s, VertexID d,
                                                 bool verify = true) {
  if (verify)
    ASSERT(isEdge(s, d));
  Ecnt--;
//   remove out edge
  _outEdges[s].erase(d);

// remove in vertex
  _inVertex[d].erase(s);
}

template<class VLabelType, class ELabelType>
void DIGRAPH<VLabelType, ELabelType>::removeAllEdges(VertexID s) {
  ASSERT(isVertex(s));

  for (typename AdjList::iterator it = getOutEdge()[s].begin();
      it != _outEdges[s].end(); it++) {
    VertexID d = it->first;
    removeEdge(s, d, false);
  }
}

template<class VLabelType, class ELabelType>
const VLabelType& DIGRAPH<VLabelType, ELabelType>::getVLabel(VertexID v) {
  ASSERT(isVertex(v));
  return _vlabels[v];
}

template<class VLabelType, class ELabelType>
const ELabelType& DIGRAPH<VLabelType, ELabelType>::getELabel(VertexID s,
                                                             VertexID d) {
  ASSERT(isEdge(s, d));
  return _outEdges[s][d].elabel;
}

template<class VLabelType, class ELabelType>
int DIGRAPH<VLabelType, ELabelType>::getInDegree(VertexID v) {
  ASSERT(isVertex(v));
  return (int) _inVertex[v].size();
}

template<class VLabelType, class ELabelType>
int DIGRAPH<VLabelType, ELabelType>::getOutDegree(VertexID v) {
  ASSERT(isVertex(v));
  return (int) _outEdges[v].size();
}

template<class VLabelType, class ELabelType>
void DIGRAPH<VLabelType, ELabelType>::printGraph(ostream& out) {
  out << "Vcnt " << "Ecnt " << endl;
  out << getVcnt() << " " << getEcnt() << " " << endl;

  out << "vid " << "out " << "in " << endl;
  for (typename VLabels::iterator it = getVLabel().begin();
      it != getVLabel().end(); it++) {
    VertexID v = it->first;
    out << v << " " << getOutDegree(v) << " " << getInDegree(v) << endl;
  }

  out << "eid " << "src " << "vl " << "el " << "dest " << "vl " << endl;
  for (typename VLabels::iterator it = getVLabel().begin();
      it != getVLabel().end(); it++) {
    VertexID s = it->first;
    VLabelType sl = getVLabel(s);

    for (typename AdjList::iterator it1 = getOutEdge()[s].begin();
        it1 != getOutEdge()[s].end(); it1++) {
      VertexID d = it1->first;
      VLabelType dl = getVLabel(d);

      EdgeID eid = it1->second.eid;
      ELabelType el = it1->second.elabel;

      out << eid << " " << s << " ";
      out << sl;
      out << " ";
      out << el;
      out << " " << d << " ";
      out << dl;
      out << endl;
    }
  }
}

template<class VLabelType, class ELabelType>
void DIGRAPH<VLabelType, ELabelType>::constLabelCnt() {
  _outLabelCnt.clear();
  _inLabelCnt.clear();

  for (typename VLabels::iterator it = getVLabel().begin();
      it != getVLabel().end(); it++) {
    VertexID s = it->first;
    VLabelType sl = getVLabel(s);

    for (typename AdjList::iterator it1 = getOutEdge()[s].begin();
        it1 != _outEdges[s].end(); it1++) {
      VertexID d = it1->first;
      VLabelType dl = getVLabel(d);

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

template<class VLabelType, class ELabelType>
void DIGRAPH<VLabelType, ELabelType>::initEdgeVisited() {
  for (typename VLabels::iterator it = getVLabel().begin();
      it != getVLabel().end(); it++) {
    VertexID s = it->first;
    for (typename AdjList::iterator it1 = getOutEdge()[s].begin();
        it1 != getOutEdge()[s].end(); it1++) {
      it1->second.isVisited = false;
    }
  }
}

template<class VLabelType, class ELabelType>
void DIGRAPH<VLabelType, ELabelType>::initEL(VertexID x) {
  e = x;
  constLabelCnt();
}

#endif /* DIGRAPH_H_ */
