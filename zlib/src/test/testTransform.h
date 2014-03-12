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

};

#endif /* TESTTRANSFORM_H_ */
