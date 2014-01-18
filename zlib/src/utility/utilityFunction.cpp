#include "utilityFunction.h"

#include <set>

#include "graph.h"
#include "GlobalDefinition.h"

void setVertexNeighborToSet(set<VertexID>& _set, VertexID v, GRAPH* g) {
  for (int i = 0; i < g->getDegree(v); i++) {
    VertexID v = g->_adjList[v][i].v;
    _set.insert(v);
  }
}

string convertInt(int number) {
  stringstream ss;  //create a stringstream
  ss << number;  //add number to the stream
  return ss.str();  //return a string with the contents of the stream
}
