/*
 * testTransform.h
 *
 *  Created on: Mar 12, 2014
 *      Author: zfan
 */

#ifndef TESTTRANSFORM_H_
#define TESTTRANSFORM_H_

#include "../utility/graph.h"
#include "../utility/InputReader.h"

#include <tr1/unordered_map>

using namespace std;

class TestTransform {
 public:
  vector<GRAPH*> graphDB;
  int g_cnt;

  TestTransform(int _g_cnt) {
    g_cnt = _g_cnt;
    graphDB.resize(g_cnt);
  }

  ~TestTransform() {
    for (int i = 0; i < g_cnt; i++) {
      delete graphDB[i];
    }
  }

  void transformFromSnap(const char* input_g_file_name,
                         const char* output_g_file_name) {
    InputReader g_reader(input_g_file_name, "snap");
    ofstream output(output_g_file_name);
    for (int i = 0; i < g_cnt; i++) {
      GRAPH *g = new GRAPH();
      g_reader.GetSnapGraph(*g);

      graphDB[i] = g;

      g->printGraph(output);
    }
  }

  void transformFromInputFile(const char* input_g_file_name,
                              const char* output_g_file_name) {
    InputReader g_reader(input_g_file_name);
    ofstream output(output_g_file_name);
    for (int i = 0; i < g_cnt; i++) {
      GRAPH *g = new GRAPH();
      g_reader.GetNextGraph_MultiVertexLabel(*g);

      graphDB[i] = g;

      g->printGraph(output);
    }
  }

  void transformFromSNAP(const char* input_g_file_name,
                         const char* output_g_file_name) {
    ifstream m_InputStream(input_g_file_name);
    ofstream output(output_g_file_name);
    for (int i = 0; i < g_cnt; i++) {
      GRAPH *g = new GRAPH();

      // read SNAP graph as adj
      int old_u, old_v;
      int new_u, new_v;
      int new_id = 0;
      unordered_map<int, int> old_to_new;
      unordered_map<int, set<int> > _adj;

      while (!m_InputStream.eof()) {
        m_InputStream >> old_u;
        m_InputStream >> old_v;

        if (old_to_new.find(old_u) == old_to_new.end()) {
          old_to_new[old_u] = new_id;
          new_u = new_id;
          new_id++;
        } else {
          new_u = old_to_new[old_u];
        }

        if (old_to_new.find(old_v) == old_to_new.end()) {
          old_to_new[old_v] = new_id;
          new_v = new_id;
          new_id++;
        } else {
          new_v = old_to_new[old_v];
        }

        _adj[new_u].insert(new_v);
        _adj[new_v].insert(new_u);
      }
      cout << _adj.size() << endl;
      // end of read

      // construct graph g
      int Vcnt = old_to_new.size();
      g->setV(Vcnt);
      int elabel = 0;
      int Eid = 0;
      // for all vertices u
      for (int i = 0; i < Vcnt; i++) {
        int u = i;

        // set degree as u's label
        g->setLabel(u, _adj[u].size());

        // set edges
        for (set<int>::iterator it = _adj[u].begin(); it != _adj[u].end();
            it++) {
          int v = *it;

          if (u <= v)
            continue;
          g->insert(Eid++, u, v, elabel);
        }
      }
      // end of construction

      graphDB[i] = g;

      g->printGraph(output);
    }
  }

};

#endif /* TESTTRANSFORM_H_ */
