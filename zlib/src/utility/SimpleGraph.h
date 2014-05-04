/*
 * SimpleGraph.h
 *
 *  Created on: 2014¦~5¤ë4¤é
 *      Author: zfan
 */

#ifndef SIMPLEGRAPH_H_
#define SIMPLEGRAPH_H_

#include <set>
#include "GlobalDefinition.h"
#include <tr1/unordered_map>
#include <stdlib.h>

using namespace std;

class SIMPLEGRAPH {
 public:
  set<VertexID> vertex_set;
  VertexLabelMapCnt vertex_deg;
  VertexLabelMap label_to_vertex;
  unordered_map<VertexID, VertexID> old_to_new;
  unordered_map<VertexID, VertexID> new_to_old;
  int edge_cnt;

  SIMPLEGRAPH() {
    edge_cnt = 0;
  }

  void addDeg(VertexID u) {
    if (vertex_deg.find(u) == vertex_deg.end()) {
      vertex_deg[u] = 1;
      return;
    }

    vertex_deg[u]++;
  }

  bool vIsIn(VertexID u) {
    if (vertex_deg.find(u) == vertex_deg.end()) {
      return false;
    }
    return true;
  }

  void remove_vertex(VertexID u) {
    set<VertexID>::iterator remove_it = vertex_set.find(u);
    if (remove_it == vertex_set.end()) {
//      cout << "error" << endl;
//      exit(1);
      return;
    }
    vertex_set.erase(remove_it);
  }

  void reset() {
    edge_cnt = 0;
    vertex_deg.clear();
    label_to_vertex.clear();
    old_to_new.clear();
    new_to_old.clear();
  }
};

#endif /* SIMPLEGRAPH_H_ */
