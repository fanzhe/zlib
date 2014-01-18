#include "../gSpan/gspan.h"
#include "../utility/graph.h"

#include "Trans_GSPANGraph_GRAPH.h"

bool Convert(GSPAN::Graph& from, GRAPH& to) {
  int Size = from.size();
  to.setV(Size);
  for (int i = 0; i < Size; i++)
    to.setLabel(i, from[i].label);
  for (int i = 0; i < Size; i++) {
    for (GSPAN::Vertex::edge_iterator it = from[i].edge.begin();
        it != from[i].edge.end(); it++) {
      if (it->to > i)
        to.setELabel(i, it->to, it->elabel);
    }
  }

  return true;
}

bool Convert(GRAPH& from, GSPAN::Graph& to) {
  int size = from.V();
  for (int i = 0; i < size; i++) {
    GSPAN::Vertex vertex;
    vertex.label = from.getLabel(i);

    for (int j = 0; j < size; j++) {
      if (i == j)
        continue;

      int elabel = from.getELabel(i, j);
      if (from.getELabel(i, j) != NO_EDGE)
        vertex.push(i, j, elabel);
    }

    to.push_back(vertex);
  }
  to.buildEdge();

  return true;
}

bool Convert(GSPAN::DFSCode& dfs_code, GRAPH& to) {
  GSPAN::Graph _graph;
  dfs_code.toGraph(_graph);
  Convert(_graph, to);

  return true;
}

bool Convert(GSPAN::DFS* dfs_code, int dfs_size, GRAPH& to) {
  int v_size = 0;
  for (int i = 0; i < dfs_size; i++)
    v_size = max(v_size, max(dfs_code[i].from, dfs_code[i].to) + 1);
  to.setV(v_size);

  for (int i = 0; i < dfs_size; i++) {
    if (dfs_code[i].fromlabel != -1)
      to.setLabel(dfs_code[i].from, dfs_code[i].fromlabel);
    if (dfs_code[i].tolabel != -1)
      to.setLabel(dfs_code[i].to, dfs_code[i].tolabel);

    to.insert(Edge(i, dfs_code[i].from, dfs_code[i].to, dfs_code[i].elabel));
  }

  return true;
}
