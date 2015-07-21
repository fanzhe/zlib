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
#include "DiGraph.h"
#include "GlobalDefinition.h"
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
 *
 */
class TestKBQuality {
 public:
  TestKBQuality();
  ~TestKBQuality();

  // graph and query
  vector<DIGRAPHBASIC*> graphDB;
  vector<DIKEYS*> queryDB;
  int g_cnt, q_cnt;
  DIPRODUCTGRAPH* global_Gp;
  unordered_set<VertexID> E;
  unordered_set<VertexID> org_E;
  vector<Pair> L;
  unordered_set<VertexID> hash_L;
  unordered_map<VertexID, unordered_set<VertexID> > GpTCIndx;

  VertexID thetaGd;
  VertexID thetaOrgGd;
  VertexID thetaL;

  // load graph and query
  ifstream m_InputStream;

  string org_output_Gd_folder;
  ofstream org_m_GdOutputStream;
  string org_output_L_folder;
  ofstream org_m_LOutputStream;

  string output_Gd_folder;
  ofstream m_GdOutputStream;
  string output_L_folder;
  ofstream m_LOutputStream;
  string output_Gp_folder;
  ofstream m_GpOutputStream;

  void loadFromInputFile(const char* input_g_name, int _g_cnt,
                         const char* input_q_name, int _q_cnt,
                         const char* org_output_Gd_folder,
                         const char* org_output_L_folder,
                         const char* output_Gd_folder,
                         const char* output_L_folder,
                         const char* output_Gp_folder);

  vector<unordered_map<VertexID, DIGRAPHBASIC*> > GdDB;
  vector<unordered_map<VertexID, unordered_set<VertexID>*> > GdVDB;
  /**
   * load G^d for all e
   */
  void loadDNeighbor();
  bool checkPredicateObjectFeasibility(DIGRAPHBASIC *dg, EdgeLabel p_1,
                                       VertexID o_1, DIKEYS *dq, EdgeLabel p_Q,
                                       VertexID o_Q);
  void printOrgDGraphChao(ofstream& out, DIGRAPHBASIC* G);
  void printDGraphChao(ofstream& out, DIGRAPHBASIC*);
  void printOrgLChao(DIGRAPHBASIC* G);
  void printLChao(ofstream& out, DIGRAPHBASIC*);
  void printGpChao(ofstream& out);
  void collectTC(DIGRAPHBASIC* G);
  void getInducedSubgraph(DIGRAPHBASIC *G, unordered_set<VertexID> &vs,
                          DIGRAPHBASIC *indG);
//  void comProductGraph(DIGRAPHBASIC *G1, DIGRAPHBASIC *G2, DIPRODUCTGRAPH *GP);
  void comProductGraph(DIGRAPHBASIC *G, VertexID e1,
                       unordered_set<VertexID> &G1v, VertexID e2,
                       unordered_set<VertexID> &G2v, DIPRODUCTGRAPH *GP);

  /**
   * compute simulation between
   * Q and GP
   */
  void comSim(DIKEYS* Q, DIPRODUCTGRAPH *GP, unordered_set<VertexID>&);

  /**
   * output L for MR
   */
  void collectLandGp(DIPRODUCTGRAPH *GP, unordered_set<VertexID>&,
                     DIGRAPHBASIC*);
  void outputGd(int i, DIGRAPHBASIC* G, unordered_set<VertexID>& vIndG,
                VertexID e);
  void unionProductGraph(DIPRODUCTGRAPH *Gp, unordered_set<VertexID>& vGp);
  VertexID hashPair(VertexID a, VertexID b);
  /**
   * compute simulation between
   * Q and indG
   */
  void comSim(DIKEYS* Q, DIGRAPHBASIC* G, unordered_set<VertexID>&);
//  unordered_map<pair, set<DIKEYS*> > L;
  /**
   * construct L
   */
  void pairing();

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
