/*
 * testKB.h
 *
 *  Created on: 2014¦~8¤ë2¤é
 *      Author: zfan
 */

#ifndef TESTKB_H_
#define TESTKB_H_

#include "../utility/DiGraph.h"
#include "../utility/InputReader.h"
#include "../utility/EL.h"

class TestKB {
 public:
  vector<DIGRAPHBASIC*> graphDB;
  vector<DIGRAPHBASIC*> queryDB;
  int g_cnt;
  int q_cnt;

  TestKB(int _g_cnt, int _q_cnt) {
    g_cnt = _g_cnt;
    graphDB.resize(g_cnt);

    q_cnt = _q_cnt;
    queryDB.resize(q_cnt);
  }

  ~TestKB() {
    for (int i = 0; i < g_cnt; i++) {
      delete graphDB[i];
    }

    for (int i = 0; i < q_cnt; i++) {
      delete queryDB[i];
    }
  }

  void loadFromInputFile(const char* input_g_name, const char* input_gvl_name,
                         const char* input_el_name, const char* input_q_name,
                         const char* input_qvl_name) {
    InputReader q_reader(input_q_name, input_qvl_name, input_el_name);

    for (int i = 0; i < q_cnt; i++) {
      DIGRAPHBASIC *dq = new DIGRAPHBASIC();

      q_reader.GetKBDiGraph(*dq);
      dq->printGraph(cout);
      queryDB[i] = dq;
    }

    InputReader g_reader(input_g_name, input_gvl_name, input_el_name);

    for (int i = 0; i < g_cnt; i++) {
      DIGRAPHBASIC *dg = new DIGRAPHBASIC();

      g_reader.GetKBDiGraph(*dg);
      dg->printGraph(cout);
      graphDB[i] = dg;
    }
  }

  void testEL() {
    VertexID u = 1;
    VertexID v1 = 1;
    VertexID v2 = 1;
    for (int i = 0; i < q_cnt; i++) {
      for (int j = 0; j < g_cnt; j++) {
        EL* el = new EL(queryDB[i], u, graphDB[j], v1, v2);
        // TODO
        el->run();
        delete el;
      }
    }
  }
};

#endif /* TESTKB_H_ */
