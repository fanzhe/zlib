/*
 * testSubGen.h
 *
 *  Created on: Feb 14, 2014
 *      Author: zfan
 */

#ifndef TESTSUBGEN_H
#define TESTSUBGEN_H

#include <iostream>

#include "../utility/InputReader.h"
#include "../utility/GlobalDefinition.h"
#include "../utility/graph.h"
#include "../minDFS/GraphToMinDFSCode.h"
#include "../minDFS/Trans_GSPANGraph_GRAPH.h"
#include "../gSpan/gspan.h"

using namespace std;

class TestSubGen {
 private:
  vector<GRAPH*> graphDB;
  int g_cnt;

 public:
  TestSubGen(int _g_cnt) {
    g_cnt = _g_cnt;
    graphDB.resize(g_cnt);
  }

  ~TestSubGen() {
    for (int i = 0; i < g_cnt; i++) {
      delete graphDB[i];
    }
  }

  void loadFromInputFile(const char* input_file_name) {
    InputReader reader(input_file_name);

    for (int i = 0; i < g_cnt; i++) {
      GRAPH *g = new GRAPH();
      reader.GetNextGraph(*g);
      g->printGraphNew(cout);

      graphDB[i] = g;
    }

  }

  void testSubGen() {
    // test subgraph generation
    for (int i = 0; i < g_cnt; i++) {
      GRAPH* g = graphDB[i];

      SubGraphGen* sg = new SubGraphGen();
      GraphToMinDFSCode mindfs;

      GSPAN::DFSCode dfs_code;
      mindfs.Init(g);

      mindfs.ConvertGRAPH(dfs_code);

      sg->Start(dfs_code);

      cout << "All subgraphs size: " << sg->Size() << endl;

      for (int i = 0; i < sg->Size(); i++) {
        if(sg->Subgraphs[i].nodeCount() == 5) {
          sg->Subgraphs[i].write(cout);
          cout << endl;
        }
      }
    }
  }
};

#endif
