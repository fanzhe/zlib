#ifndef TESTCASEMINDFSCODE_H
#define TESTCASEMINDFSCODE_H

#include <iostream>

#include "../utility/InputReader.h"
#include "../utility/GlobalDefinition.h"
#include "../utility/graph.h"
#include "../minDFS/GraphToMinDFSCode.h"
#include "../minDFS/Trans_GSPANGraph_GRAPH.h"
#include "../gSpan/gspan.h"

using namespace std;

class TestMinDFSCode {
 private:
  vector<GRAPH*> graphDB;
  int g_cnt;

 public:
  TestMinDFSCode(int _g_cnt) {
    g_cnt = _g_cnt;
    graphDB.resize(g_cnt);
  }

  ~TestMinDFSCode() {
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

  void testMinDFSCode() {
    // test min dfs code
    for (int i = 0; i < g_cnt; i++) {
      GRAPH* g = graphDB[i];
      GraphToMinDFSCode mindfs;

      GSPAN::DFSCode dfs_code;
      mindfs.Init(g);

      mindfs.ConvertGRAPH(dfs_code);

      cout << "minDFSCode: ";
      dfs_code.write(cout);
      cout << endl;

    }
  }
};

#endif
