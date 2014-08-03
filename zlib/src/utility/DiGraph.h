/*
 * DiGraph.h
 *
 *  Created on: 2014¦~8¤ë1¤é
 *      Author: zfan
 */

#ifndef DIGRAPH_H_
#define DIGRAPH_H_

#include "GlobalDefinition.h"
#include "graph.h"
#include <vector>
#include <tr1/unordered_map>

using namespace std;

class DIGRAPH {
 public:
  /*
   * basic data structures
   */
  GraphID graphId;
  int Vcnt, Ecnt;
  int max_vcnt;
  VertexLabel *_vlabels;  // vertex label
  vector<vector<AdjElement> > _outEdges;  // out edge list
  vector<unordered_map<VertexID, bool> > _outVertex;  // _outVertex[v][u]: check if (v, u)
  vector<unordered_map<VertexID, bool> > _inVertex;  // in vertex

  /*
   * basic operations
   */
  DIGRAPH();
  DIGRAPH(int V);
  ~DIGRAPH();

  void resetVcnt(int V);
  int getVcnt();
  int getEcnt();
  void setVLabel(VertexID v, VertexLabel label);
  VertexLabel getVLabel(VertexID v);
  void insertEdge(Edge e);
  int getOutDegree(VertexID v);
  int getInDegree(VertexID v);
  void printGraphNew(ostream& out);

  /*
   * data structures for EL algorithm
   */
  VertexID e;
  vector<unordered_map<Label, int> > _outLabelCnt;
  vector<unordered_map<Label, int> > _inLabelCnt;
  void initEL();
  void constLabelCnt();

  /*
   * data structures for subIso
   */

  /*
   * data structures for simulation
   */
};

#endif /* DIGRAPH_H_ */
