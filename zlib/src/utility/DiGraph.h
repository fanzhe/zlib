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

class DIGRAPH {
 public:
  /*
   * data structures for basic operations
   */
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
  void insertVertex(VertexID v, VertexLabel label);
  void insertEdge(VertexID s, VertexID d, EdgeLabel el);
  void insertEdge(Edge e);
  void setVLabel(VertexID v, VertexLabel label);
  void setELabel(VertexID s, VertexID d, EdgeLabel el);
  VertexLabel getVLabel(VertexID v);
  EdgeLabel getELabel(VertexID s, VertexID d);
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
  CandQtoG Cq;

  void initEL(VertexID x);
  void initEdgeVisited();
  void constLabelCnt();

};

#endif /* DIGRAPH_H_ */
