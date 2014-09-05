/*
 * testUllman.h
 *
 *  Created on: Sep 2, 2014
 *      Author: zfan
 */

#ifndef TESTULLMAN_H_
#define TESTULLMAN_H_

#include "../utility/graph.h"
#include "../subIso/ullman.h"
#include "../math/matrix.h"

class TestUllman {
 public:
  vector<GRAPH*> graphDB, queryDB;
  int g_cnt, q_cnt;
  int cnt_res;

  TestUllman(int _q_cnt, int _g_cnt) {
    g_cnt = _g_cnt;
    q_cnt = _q_cnt;
    graphDB.resize(g_cnt);
    queryDB.resize(q_cnt);
    cnt_res = 0;
  }

  ~TestUllman() {
    for (int i = 0; i < g_cnt; i++) {
      delete graphDB[i];
      delete queryDB[i];
    }
  }

  void loadFromInputFile(const char* input_q_file_name,
                         const char* input_g_file_name) {
    InputReader q_reader(input_q_file_name);
    InputReader g_reader(input_g_file_name);

    for (int i = 0; i < q_cnt; i++) {
      GRAPH *g = new GRAPH();
      q_reader.GetNextGraph_MultiVertexLabel(*g);

      g->AM = matrix_init(g->V(), g->V());
      for (int i = 0; i < g->V(); i++) {
        for (int j = 0; j < g->V(); j++) {
          if (g->edge(i, j)) {
            g->AM[i][j] = 1;
          }
        }
      }

      queryDB[i] = g;
    }

    for (int i = 0; i < g_cnt; i++) {
      GRAPH *g = new GRAPH();
      g_reader.GetNextGraph_MultiVertexLabel(*g);

      g->AM = matrix_init(g->V(), g->V());
      for (int i = 0; i < g->V(); i++) {
        for (int j = 0; j < g->V(); j++) {
          if (g->edge(i, j)) {
            g->AM[i][j] = 1;
          }
        }
      }

      graphDB[i] = g;
    }
  }

  void testUllman() {
    map<double, double> result;
    for (int i = 0; i < q_cnt; i++) {
      GRAPH* q = queryDB[i];
      cout << i << ": ";

      clock_t start = clock();
      for (int j = 0; j < g_cnt; j++) {
        GRAPH* g = graphDB[j];
//        g->printGraph(cout);
        Ullman* ull = new Ullman(q, g);
        cout << "- " <<  j << " " << ull->p1 << " " << ull->p2 << " " << ull->p3 << endl;
        bool res = ull->result;
        if (res)
          cnt_res++;
      }
      clock_t end = clock();

      double runTime = gettime(start, end) * CLOCKS_PER_SEC;
      result[runTime]++;
    }
    printMapTT(result);
    cout << "total:" << cnt_res << endl;
  }
};

#endif /* TESTULLMAN_H_ */
