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

#include "../subIso/cache.h"

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
    map<double, double> result;
    for (int i = 0; i < q_cnt; i++) {
      GRAPH* q = queryDB[i];
//      q->printGraphNew(cout);

      for (int j = 0; j < g_cnt; j++) {
        GRAPH* g = graphDB[j];
//        g->printGraphNew(cout);
        clock_t start = clock();

        bool res = q->isSubgrpahOfByVF2(g);
//        cout << i << " " << j << " " << res << endl;
        if (res) {
          clock_t end = clock();
          double runTime = gettime(start, end) * CLOCKS_PER_SEC;
          result[runTime]++;
          cnt_res++;
          continue;
        }
        clock_t end = clock();
        double runTime = gettime(start, end) * CLOCKS_PER_SEC;
        result[runTime]++;
      }

    }
    printMapTT(result);
    cout << "total:" << cnt_res << endl;
  }

  /**
   * for PI DEMO submission
   * 2014/09/05
   */

  Cache cache;
  void testForPI() {
    map<double, double> result;
    int _cnt = 0;
    int res_cnt = 0;
    for (int i = 0; i < q_cnt; i++) {
      GRAPH* q = queryDB[i];
      cout << i << endl;
      for (int j = 0; j < g_cnt; j++) {
        GRAPH* g = graphDB[j];

        clock_t start = clock();

        // gogogo
        bool res = _testForPI(q, g);

        clock_t end = clock();
        // ms
        double runTime = gettime(start, end) * CLOCKS_PER_SEC;
        result[_cnt++] = runTime;
        if (res)
          res_cnt++;
      }

    }
    cout << "true: " << res_cnt << endl;
    printMapTT(result);
    // calculate std dev.
  }

  bool _testForPI(GRAPH* q, GRAPH* g) {
    GRAPH* cs = new GRAPH();
    // Pre-process q
    g->setVertexLabelMap();
    g->setVertexLabelMapCnt();
    q->setVertexLabelMap();
    q->setVertexLabelMapCnt();

    q->clientPreProcessWoEnc(g->vlabels_map_cnt);
    if (g->vlabels_map_cnt.find(q->start_label) == g->vlabels_map_cnt.end()) {
      return false;
    }

    set<VertexID>::iterator it = g->vlabels_map[q->start_label].begin();
    for (; it != g->vlabels_map[q->start_label].end(); it++) {
      VertexID r_vertex = *it;
      cache.ifRootVertex.insert(r_vertex);

      // BFS
      set<VertexID> visit_v;
      g->BFSwithConst(r_vertex, q->min_tree_height, visit_v, q->vlabels_map_cnt, cache);

      // Construct CS
      g->getInducedSubGraph(visit_v, cs);

      // VF2
      bool res = q->isSubgrpahOfByVF2(cs);
      if (res)
        return res;

      // reset cs
      cs->makeEmpty();
    }

    delete cs;
    return false;
  }

};

#endif
