/*
 * testGenDataSet.h
 *
 *  Created on: Mar 13, 2014
 *      Author: zfan
 */

#ifndef TESTGENDATASET_H_
#define TESTGENDATASET_H_

#include <tr1/unordered_map>

#include "../utility/graph.h"
#include "../utility/InputReader.h"
#include "../math/random.h"

using namespace std::tr1;

class TestGenDataSet {
 public:
  vector<GRAPH*> graphDB;
  int g_cnt, distinctLabel;
  Random* randGen;

  TestGenDataSet(int _g_cnt) {
    g_cnt = _g_cnt;
    graphDB.resize(g_cnt);
    randGen = new Random();
  }

  ~TestGenDataSet() {
    for (int i = 0; i < g_cnt; i++) {
      delete graphDB[i];
    }
  }

  void genDataSet(const char* input_g_file_name, char* output_g_file_name,
                  int _distinctLabel) {
    InputReader g_reader(input_g_file_name);
    ofstream output(output_g_file_name);
    distinctLabel = _distinctLabel;

    for (int i = 0; i < g_cnt; i++) {
      GRAPH *g = new GRAPH();
      g_reader.GetNextGraph_MultiVertexLabel_Original(*g);
      graphDB[i] = g;

      // TODO generate data set
      generateData(g);
      g->printGraph(output);
    }
  }

  void generateData(GRAPH* g) {
    int rand = 0;
    int maxLabelCnt = g->V() / distinctLabel + 1;
    unordered_map<VertexID, int> mymap;
    for (int i = 0; i < g->V();) {
      rand = randGen->genRanInt(distinctLabel);

      g->setLabel(i, rand);
      if (mymap.find(rand) != mymap.end()) {
        if (mymap[rand] > maxLabelCnt) {
//          rand++;
          continue;
        } else {
          cout << i << " " << rand << endl;
          mymap[rand]++;
          i++;
        }
      } else {
        mymap[rand] = 1;
        i++;
      }
    }
  }

  void genQuerySet(const char* input_g_file_name, char* output_g_file_name,
                   int qcnt, int vcnt) {
    // TODO generate query set
    InputReader g_reader(input_g_file_name);
    ofstream output(output_g_file_name);

    for (int i = 0; i < g_cnt; i++) {
      GRAPH *g = new GRAPH();
      g_reader.GetNextGraph_MultiVertexLabel_Original(*g);
      graphDB[i] = g;

      GRAPH* q = new GRAPH();
      // TODO generate query set
      for (int i = 0; i < qcnt; i++) {

        q->graphId = i;
        generateQuery(g, q, vcnt);
        q->printGraph(output);

        q->makeEmpty();
      }
      delete q;
    }
  }

  void generateQuery(GRAPH* g, GRAPH* q, int vcnt) {
    int start_v = randGen->genRanInt(g->V());
    set<VertexID> visit_v;
    g->BFSwithSpecVcnt(start_v, vcnt, visit_v);
    g->getInducedSubGraph(visit_v, q);
  }
};

#endif /* TESTGENDATASET_H_ */
