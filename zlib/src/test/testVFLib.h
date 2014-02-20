#ifndef TESTVFLIB_H
#define TESTVFLIB_H

//#include <stdio.h>

#include "../utility/graph.h"
#include "../utility/GlobalDefinition.h"
#include "../utility/InputReader.h"
#include "../vflib/argedit.h"
#include "../vflib/argraph.h"
#include "../vflib/match.h"
#include "../vflib/ull_sub_state.h"
#include "../vflib/vf2_sub_state.h"

class TestVFLib {
 public:
  vector<GRAPH*> graphDB, queryDB;
  int g_cnt, q_cnt;
  int cnt_res;

  TestVFLib(int _q_cnt, int _g_cnt) {
    g_cnt = _g_cnt;
    q_cnt = _q_cnt;
    graphDB.resize(g_cnt);
    queryDB.resize(q_cnt);
    cnt_res = 0;
  }

  ~TestVFLib() {
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

      queryDB[i] = g;
    }

    for (int i = 0; i < g_cnt; i++) {
      GRAPH *g = new GRAPH();
      g_reader.GetNextGraph_MultiVertexLabel(*g);

      graphDB[i] = g;
    }
  }

  void testIndSub() {
    for (int i = 0; i < q_cnt; i++) {
      GRAPH* q = queryDB[i];
      q->printGraphNew(cout);
      for (int j = 0; j < g_cnt; j++) {
        GRAPH* g = graphDB[j];
        g->printGraphNew(cout);

        GRAPH* indsg = new GRAPH();
        set<VertexID> _inds;
        _inds.insert(1);
        _inds.insert(4);
        _inds.insert(3);
        _inds.insert(5);
        int n = 0;

        g->getInducedSubGraph(_inds, indsg, n);
        cout << "+++++++++" << endl;
        indsg->printGraphNew(cout);
      }
    }
  }

  void testSubIso() {
    for (int i = 0; i < q_cnt; i++) {
      GRAPH* q = queryDB[i];
//      q->printGraphNew(cout);
      for (int j = 0; j < g_cnt; j++) {
        GRAPH* g = graphDB[j];
//        g->printGraphNew(cout);

        bool res = q->isSubgrpahOfByVF2(g);
//        cout << i << " " << j << " " << res << endl;
        if (res)
          cnt_res++;
      }
    }
    cout << "total:" << cnt_res << endl;
  }

};

#endif
