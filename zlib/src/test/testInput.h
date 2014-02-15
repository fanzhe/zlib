/*
 * testInput.h
 *
 *  Created on: 2014¦~2¤ë15¤é
 *      Author: zfan
 */

#ifndef TESTINPUT_H_
#define TESTINPUT_H_

#include "../utility/graph.h"
#include "../utility/InputReader.h"

class TestInput {
 public:
  vector<GRAPH*> graphDB;
  int g_cnt;

  TestInput(int _g_cnt) {
    g_cnt = _g_cnt;
    graphDB.resize(g_cnt);
  }

  ~TestInput() {
    for (int i = 0; i < g_cnt; i++) {
      delete graphDB[i];
    }
  }

  void loadFromInputFile(const char* input_g_file_name) {
    InputReader g_reader(input_g_file_name);

    for (int i = 0; i < g_cnt; i++) {
      GRAPH *g = new GRAPH();

//      g_reader.GetNextGraph_MultiVertexLabel(*g);
      g_reader.GetNextGraph_MultiVertexLabel_Original(*g);
//      g_reader.GetNextGraph_Original(*g);
      g->printGraphNew(cout);
      graphDB[i] = g;
    }
  }

};

#endif /* TESTINPUT_H_ */
