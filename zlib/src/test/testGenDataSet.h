/*
 * testGenDataSet.h
 *
 *  Created on: Mar 13, 2014
 *      Author: zfan
 */

#ifndef TESTGENDATASET_H_
#define TESTGENDATASET_H_

#include <tr1/unordered_map>
#include <queue>

#include "../utility/graph.h"
#include "../utility/InputReader.h"
#include "../math/random.h"
using namespace std::tr1;

class TestGenDataSet {
 public:
  vector<GRAPH*> graphDB;
  int g_cnt, distinctLabel;
  Random* randGen;
  int threshold;

  TestGenDataSet(int _g_cnt) {
    g_cnt = _g_cnt;
    graphDB.resize(g_cnt);
    randGen = new Random();
    threshold = 8000;
  }

  ~TestGenDataSet() {
    for (int i = 0; i < g_cnt; i++) {
      delete graphDB[i];
    }
    delete randGen;
  }

  void showStat(const char* input_g_file_name) {
    InputReader g_reader(input_g_file_name);

    for (int i = 0; i < g_cnt; i++) {
      GRAPH *g = new GRAPH();
      g_reader.GetNextGraph_MultiVertexLabel_Original(*g);
      graphDB[i] = g;

      cout << "Vcnt: " << g->V() << endl;
      cout << "Ecnt: " << g->E() << endl;
      cout << "Density: " << double((2 * g->E()) / (g->V())) / (g->V() - 1)
           << endl;

      unordered_map<VertexLabel, int> mymap;
      double avg_deg = 0;
      for (int i = 0; i < g->V(); i++) {
        avg_deg += g->getDegree(i);
        VertexLabel l = g->getLabel(i);
        if (mymap.find(l) == mymap.end()) {
          mymap[l] = 1;
          continue;
        }
        mymap[l]++;
      }

      cout << "avg_deg: " << avg_deg / g->V() << endl;
      cout << "distinct_label: " << mymap.size() << endl;

      for (unordered_map<VertexLabel, int>::iterator it = mymap.begin();
          it != mymap.end(); it++) {
        cout << it->first << " " << it->second << endl;
      }
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

//      assignLabelByDegree(g);
      assignLabelByRandom(g);
      g->printGraph(output);
    }
  }

  void assignLabelByDegree(GRAPH* g) {
    for (int i = 0; i < g->V(); i++) {
      g->setLabel(i, g->getDegree(i));
    }
  }

  void assignLabelByRandom(GRAPH* g) {
    int rand = 0;
    unordered_map<VertexID, int> mymap;
    for (int i = 0; i < g->V();) {
      rand = randGen->genRanInt(distinctLabel);

      g->setLabel(i, rand);
      if (mymap.find(rand) != mymap.end()) {
        cout << i << " " << rand << endl;
        mymap[rand]++;
        i++;
      } else {
        mymap[rand] = 1;
        i++;
      }
    }
  }

  void genQuerySetDFS(const char* input_g_file_name, char* output_g_file_name,
                      int qcnt, int vcnt) {
    InputReader g_reader(input_g_file_name);
    ofstream output(output_g_file_name);

    for (int i = 0; i < g_cnt; i++) {
      GRAPH *g = new GRAPH();
      g_reader.GetNextGraph_MultiVertexLabel_Original(*g);
      graphDB[i] = g;

      GRAPH* q = new GRAPH();
      for (int j = 0; j < qcnt; j++) {

        q->graphId = j;
        generateQueryDFS(g, q, vcnt);
//        if (!isAvailable(q, vcnt)) {
//          j--;
//          continue;
//        }
        q->printGraph(output);

        q->makeEmpty();
      }
      delete q;
    }
  }

  bool isAvailable(GRAPH* q, int vcnt) {
    if (q->V() != vcnt) {
      return false;
    }
    for (int i = 0; i < q->V(); i++) {
      if (q->getLabel(i) >= threshold)
        return false;
    }
    return true;
  }

  void genQuerySetBFS(const char* input_g_file_name, char* output_g_file_name,
                      int qcnt, int vcnt) {
    InputReader g_reader(input_g_file_name);
    ofstream output(output_g_file_name);

    for (int i = 0; i < g_cnt; i++) {
      GRAPH *g = new GRAPH();
      g_reader.GetNextGraph_MultiVertexLabel_Original(*g);
      graphDB[i] = g;

      GRAPH* q = new GRAPH();
      for (int j = 0; j < qcnt; j++) {

        q->graphId = j;
        generateQueryBFS(g, q, vcnt);
        if (!isAvailable(q, vcnt)) {
          j--;
          continue;
        }

        q->printGraph(output);

        q->makeEmpty();
      }
      delete q;
    }
  }

  void generateQueryDFS(GRAPH* g, GRAPH* q, int vcnt) {
    int start_v = randGen->genRanInt(g->V());

    set<VertexID> visit_v;
    DFSwithRandom(g, start_v, vcnt, visit_v);
    g->getInducedSubGraph(visit_v, q);
  }

  void generateQueryBFS(GRAPH* g, GRAPH* q, int vcnt) {
    int start_v = randGen->genRanInt(g->V());

    set<VertexID> visit_v;
    BFSwithRandom(g, start_v, vcnt, visit_v);
    g->getInducedSubGraph(visit_v, q);
  }

  void DFSwithRandom(GRAPH* g, VertexID start_v, int vcnt,
                     set<VertexID>& visit_v) {
    visit_v.insert(start_v);
    _DFSwithRandom(g, start_v, vcnt, visit_v);
  }

  void _DFSwithRandom(GRAPH* g, VertexID v, int vcnt, set<VertexID>& visit_v) {
    if (visit_v.size() == vcnt) {
      return;
    }

    for (int i = 0; i < g->getDegree(v); i++) {
      int ind = randGen->genRanInt(g->getDegree(v));
      VertexID u = g->_adjList[v][ind].v;

      if (visit_v.find(u) != visit_v.end()) {
        continue;
      }

      visit_v.insert(u);
      _DFSwithRandom(g, u, vcnt, visit_v);
      if (visit_v.size() == vcnt)
        return;
    }
  }

  void BFSwithRandom(GRAPH* g, VertexID start_v, int vcnt,
                     set<VertexID>& visit_v) {
    queue<VertexID> nodes;

    visit_v.insert(start_v);
    nodes.push(start_v);

    while (!nodes.empty()) {
      VertexID v = nodes.front();
      nodes.pop();

      for (int j = 0; j < g->getDegree(v); j++) {
        int ind = randGen->genRanInt(g->getDegree(v));
        VertexID u = g->_adjList[v][ind].v;

        // u is visited
        if (visit_v.find(u) != visit_v.end()) {
          continue;
        }

        visit_v.insert(u);

        // found vcnt nodes
        if (visit_v.size() == vcnt) {
          return;
        }

        nodes.push(u);
      }
    }
  }
};

#endif /* TESTGENDATASET_H_ */
