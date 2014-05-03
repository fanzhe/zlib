#ifndef TESTSUBISO_H
#define TESTSUBISO_H

#include "../utility/GlobalDataStructures.h"
#include "../utility/utilityFunction.h"
#include "../utility/graph.h"
#include "../utility/myStat.h"
#include "../subIso/subIso.h"
#include "../utility/OutputWriter.h"

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
//        g->printGraphNew(cout);

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

  void testSubIso(const char* detailed_result, const char* avg_result) {
    OutputWriter* writer = new OutputWriter(detailed_result, avg_result);
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
        subIso->myStat->each_tt_time = gettime(_s, _e);

        myStat->avg_total_time += subIso->myStat->each_tt_time + q->myStat->encrypt_time;
        myStat->avg_sp_time += subIso->myStat->each_tt_time - q->myStat->decrypt_time;
        myStat->avg_client_time += q->myStat->encrypt_time
            + q->myStat->decrypt_time;
        myStat->avg_cr_time += subIso->myStat->cr_time;
        myStat->avg_cr_bfs_time += subIso->myStat->cr_bfs_time;
        myStat->avg_cr_cont_time += subIso->myStat->cr_cont_time;
        myStat->avg_nec_time += subIso->myStat->nec_time;
        myStat->avg_nc_time += subIso->myStat->nc_time;
        myStat->avg_cr_predict_time += subIso->myStat->cr_predict_time;
        myStat->avg_cm_time += subIso->myStat->cm_time;
        myStat->avg_canon_cm_time += subIso->myStat->canon_cm_time;
        myStat->avg_decomp_cm_time += subIso->myStat->decomp_cm_time;
        myStat->avg_match_time += subIso->myStat->match_time;
        myStat->avg_isSubgraphOf2e_time += subIso->myStat->isSubgraphOf2e_time;
        myStat->avg_mul_add_time += subIso->myStat->mul_add_time;

        myStat->avg_org_psb_map_cnt += q->myStat->org_psb_map_cnt;
        myStat->avg_red_psb_map_cnt += q->myStat->red_psb_map_cnt;
        myStat->avg_client_msg_size += q->myStat->encypted_msg_cnt;

        myStat->avg_cm_cnt += subIso->myStat->cm_cnt;
        myStat->avg_cm_cnt_prune += subIso->myStat->cm_cnt_prune;
        myStat->avg_cr_cnt += subIso->myStat->cr_cnt;
        myStat->avg_cr_cnt_predict += subIso->myStat->cr_cnt_predict;

        myStat->avg_org_cr_v += subIso->myStat->org_cr_v;
        myStat->avg_org_cr_e += subIso->myStat->org_cr_e;
        myStat->avg_red_cr_v += subIso->myStat->red_cr_v;
        myStat->avg_red_cr_e += subIso->myStat->red_cr_e;
        myStat->avg_nec_effect_v += subIso->myStat->nec_effect_v;
        myStat->avg_nec_effect_e += subIso->myStat->nec_effect_e;
        myStat->avg_nc_effect_v += subIso->myStat->nc_effect_v;
        myStat->avg_nc_effect_e += subIso->myStat->nc_effect_e;

        writer->writeDetailsubIsoResult(subIso->myStat);
        writer->writeDetailMatchResult(q->myStat);

        if (res1) {
          cnt_res1++;
        }

        q->myStat->reset();
        delete subIso;
      }

      q->encryptFree();
    }

    myStat->tt = (q_cnt * g_cnt);
    myStat->answer_cnt = cnt_res1;
    writer->writeAvgResult(myStat);

    delete myStat;
  }

};

#endif
