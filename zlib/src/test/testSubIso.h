#ifndef TESTSUBISO_H
#define TESTSUBISO_H

#include "../utility/GlobalDataStructures.h"
#include "../utility/utilityFunction.h"
#include "../utility/graph.h"
#include "../utility/myStat.h"
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
      g_reader.GetNextGraph_MultiVertexLabel_Original(*g);

      graphDB[i] = g;
    }
  }

  void debugSubIso() {
    for (int i = 0; i < q_cnt; i++) {
      GRAPH* q = queryDB[i];
      cout << "q" << i << endl;

      q->clientPreProcess();

      for (int j = 0; j < g_cnt; j++) {
        GRAPH* g = graphDB[j];
        SubIso* subIso = new SubIso(q, g);

        bool res1 = subIso->isSubIso();
        if (res1) {
          cnt_res1++;
        }
        delete subIso;

        // VF2
        bool res2 = q->isSubgrpahOfByVF2(g);
        if (res2) {
          cnt_res2++;
        }

        if (res1 != res2) {
          cout << i << " " << j << " " << res1 << " " << res2 << endl;
        }
      }

      q->encryptFree();
    }

    cout << "total: " << cnt_res1 << " vs. " << cnt_res2 << endl;
  }

  void testSubIso() {
    STAT* myStat = new STAT();

    for (int i = 0; i < q_cnt; i++) {
      GRAPH* q = queryDB[i];
//      if (i != 4) {
//        continue;
//      }
      cout << "q:" << i << " |V|: " << q->Vcnt << " |E|: " << q->Ecnt;

      clock_t _s = clock();
      q->clientPreProcess();
      clock_t _e = clock();
      q->myStat->encrypt_time = gettime(_s, _e);

      for (int j = 0; j < g_cnt; j++) {
        GRAPH* g = graphDB[j];
        cout << "   g:" << j << " |V|: " << g->Vcnt << " |E|: " << g->Ecnt
             << endl;
        SubIso* subIso = new SubIso(q, g);

        clock_t _s = clock();
        bool res1 = subIso->isSubIso();
        clock_t _e = clock();
        myStat->avg_sp_time += gettime(_s, _e);

        myStat->avg_client_time += q->myStat->encrypt_time
            + q->myStat->decrypt_time;
        myStat->avg_client_msg_size += q->myStat->encypted_msg_cnt;

        cout << "decrypt_time: " << q->myStat->decrypt_time << endl;
        cout << "isSubgraphOf2e_time: " << q->myStat->isSubgraphOf2e_time << endl;
        cout << "mul_add_time: " << q->myStat->mul_add_time << endl;

        q->myStat->reset();


        if (res1) {
          cnt_res1++;
        }
        delete subIso;
      }

      q->encryptFree();
    }

    // after finish
    cout << "total: " << (q_cnt * q_cnt) << " / " << cnt_res1 << endl;
    cout << "avg_sp_time: " << myStat->avg_sp_time / (g_cnt * q_cnt) << endl;
    cout << "avg_client_time: " << myStat->avg_client_time / (g_cnt * q_cnt)
         << endl;
    cout << "avg_total_msg_size: "
         << myStat->avg_client_msg_size / (g_cnt * q_cnt) << endl;

    delete myStat;
  }

};

#endif
