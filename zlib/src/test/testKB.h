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

class TestKB {
 public:
  vector<DIGRAPH*> graphDB;
  int g_cnt;

  TestKB(int _g_cnt) {
    g_cnt = _g_cnt;
    graphDB.resize(g_cnt);
  }

  ~TestKB() {
    for (int i = 0; i < g_cnt; i++) {
      delete graphDB[i];
    }
  }

  void loadFromInputFile(const char* input_g_name, const char* input_vl_name,
                         const char* input_el_name) {
    InputReader g_reader(input_g_name, input_vl_name, input_el_name);

    for (int i = 0; i < g_cnt; i++) {
      DIGRAPH *dg = new DIGRAPH();

      g_reader.GetKBDiGraph(*dg);
      dg->printGraphNew(cout);
      graphDB[i] = dg;
    }
  }

};

#endif /* TESTKB_H_ */
