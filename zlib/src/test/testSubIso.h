#ifndef TESTSUBISO_H
#define TESTSUBISO_H

#include "../utility/GlobalDataStructures.h"
#include "../utility/graph.h"
#include "../subIso/subIso.h"

class TestSubIso {
 public:
  vector<GRAPH*> graphDB, queryDB;
  int g_cnt, q_cnt;
  int cnt_res1, cnt_res2;

  TestSubIso(int _q_cnt, int _g_cnt) {
    g_cnt = _g_cnt;
    q_cnt = _q_cnt;
    graphDB.resize(g_cnt);
    queryDB.resize(q_cnt);
    cnt_res1 = cnt_res2 = 0;
  }

  ~TestSubIso() {
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

  void debugSubIso() {
      for (int i = 0; i < q_cnt; i++) {
        GRAPH* q = queryDB[i];
        cout << "q" << i << endl;
        for (int j = 0; j < g_cnt; j++) {

//          if (i != 4 || j != 13)
//            continue;

//          cout << "-----------query graph----------" << endl;
//          q->printGraphNew(cout);


          GRAPH* g = graphDB[j];
          SubIso* subIso = new SubIso(q, g);

//          cout << "-----------graph data----------" << endl;
//          g->printGraphNew(cout);

          bool res1 = subIso->isSubIso();
          if (res1) {
            cnt_res1++;
          }
          bool res2 = q->isSubgrpahOfByVF2(g);
          if (res2) {
            cnt_res2++;
          }
  //        cout << i << " " << j << " " << res << endl;

          if (res1 != res2) {
            cout << i << " " << j << " " << res1 << " " << res2 << endl;
          }
          delete subIso;
        }
      }

      cout << "total: " << cnt_res1 << " vs. " << cnt_res2 << endl;
    }

  void testSubIso() {
    for (int i = 0; i < q_cnt; i++) {
      GRAPH* q = queryDB[i];
      for (int j = 0; j < g_cnt; j++) {

        GRAPH* g = graphDB[j];
        SubIso* subIso = new SubIso(q, g);

        bool res1 = subIso->isSubIso();
        if (res1) {
          cnt_res1++;
        }
        delete subIso;
      }
    }

    cout << "total: " << cnt_res1 << " vs. " << cnt_res2 << endl;
  }

};

#endif
