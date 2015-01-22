/*
 * testKBQuality.h
 *
 *  Created on: 2015¦~1¤ë18¤é
 *      Author: zfan
 */

#ifndef TESTKBQUALITY_H_
#define TESTKBQUALITY_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include "../utility/DiGraph.h"
#include "../utility/GlobalDefinition.h"
using namespace std;

/**
 *
 * query format:
 *
 * x is when id = 0
 * id type follows graphs.
 * 1 -> x
 * 2 -> y
 * 3 -> y*
 * 4 -> _y
 * 5 -> d
 * divided by #
 *
 * graph format:
 * id type neighbor
 */

/**
 * The format of dataset follows Chao's.
 * Load graph : DONE
 * d-neighbor
 * TODO : simulation?
 * construct product graph
 */
class TestKBQuality {
 public:
  TestKBQuality();
  ~TestKBQuality();

  // graph and query
  vector<DIGRAPHBASIC*> graphDB;
  vector<DIKEYS*> queryDB;
  int g_cnt, q_cnt;

  // load graph and query
  ifstream m_InputStream;

  void loadFromInputFile(const char* input_g_name, int _g_cnt,
                         const char* input_q_name, int _q_cnt);

  unordered_map<VertexID, DIGRAPHBASIC*> GdDB;
  unordered_map<VertexID, set<VertexID>*> GdVDB;
  /**
   * load G^d for all e
   */
  void loadDNeighbor();
  bool checkPredicateObjectFeasibility(DIGRAPHBASIC *dg, EdgeLabel p_1,
                                       VertexID o_1, DIKEYS *dq, EdgeLabel p_Q,
                                       VertexID o_Q);

  /**
   * compute simulation for all e
   * of (G^d, Q)
   */
  void comSim();
//  unordered_map<pair, set<DIKEYS*> > L;
  /**
   * construct L
   */
  void pairing();
  bool checkPairing(DIGRAPHBASIC *G_1, DIGRAPHBASIC *G_2, VertexID e_1,
                    VertexID e_2, DIKEYS *Q);

  /**
   * construct product graph
   */
  DIGRAPHBASIC* Gp;
  void productGraph() {
    Gp = new DIGRAPHBASIC();

  }

  /**
   * product g1 and g2
   * 1. start from (e_1, e_2)
   * 2. BFS
   * 3. update product
   *
   */
  void prodctGraph(DIGRAPHBASIC* g_1, VertexID e_1, DIGRAPHBASIC* g_2,
                   VertexID e_2) {

  }

};

#endif /* TESTKBQUALITY_H_ */
