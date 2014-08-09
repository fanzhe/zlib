/*
 * testKB.h
 *
 *  Created on: 2014¦~8¤ë2¤é
 *      Author: zfan
 */

#ifndef TESTKB_H_
#define TESTKB_H_

#include "../utility/DiGraph.h"
#include "../utility/InputReader.h"
#include "../utility/EL.h"

class DDGVertex {
 public:
  Pair p;  // (v1, v2)
  bool determined;
  bool Xv1v2Qi;
  MapListPair LXPair;  // \vee \wedge X_(v1, v2)
  MapPairHash AllXPairHash;
  MapPairVertex AllXPairV;

  friend inline ostream& operator<<(ostream& out, const DDGVertex& obj) {
    out << obj.p;
    out << "[";
//    for (MapPairHash::iterator it = obj.AllXPairHash.begin();
//        it != obj.AllXPairHash.end(); it++) {
//      out << it->second << ", ";
//    }
    out << "]";
    return out;
  }
};

class TestKB {
 public:
  vector<DIGRAPHBASIC*> graphDB;
  vector<DIGRAPHBASIC*> queryDB;
  int g_cnt;
  int q_cnt;

  TestKB(int _g_cnt, int _q_cnt) {
    g_cnt = _g_cnt;
    graphDB.resize(g_cnt);

    q_cnt = _q_cnt;
    queryDB.resize(q_cnt);
  }

  ~TestKB() {
    for (int i = 0; i < g_cnt; i++) {
      delete graphDB[i];
    }

    for (int i = 0; i < q_cnt; i++) {
      delete queryDB[i];
    }
  }

  void loadFromInputFile(const char* input_g_name, const char* input_gvl_name,
                         const char* input_el_name, const char* input_q_name,
                         const char* input_qvl_name) {
    InputReader q_reader(input_q_name, input_qvl_name, input_el_name);

    for (int i = 0; i < q_cnt; i++) {
      DIGRAPHBASIC *dq = new DIGRAPHBASIC();

      q_reader.GetKBDiGraph(*dq);
      dq->printGraph(cout);
      queryDB[i] = dq;
    }

    InputReader g_reader(input_g_name, input_gvl_name, input_el_name);

    for (int i = 0; i < g_cnt; i++) {
      DIGRAPHBASIC *dg = new DIGRAPHBASIC();

      g_reader.GetKBDiGraph(*dg);
      dg->printGraph(cout);
      graphDB[i] = dg;
    }
  }

  // test EL function
  void testEL(VertexID v1, VertexID v2) {
    VertexID u = 1;
    for (int i = 0; i < g_cnt; i++) {
      for (int j = 0; j < q_cnt; j++) {
        EL el(queryDB[j], u, graphDB[i], v1, v2);
        el.run();
      }
    }
  }

  /**
   * TODO:
   * put them into a single class
   */

  // global
  MapPairVertex GlobalAllXPairHash;
  DIGRAPHDDG* ddg;
  VertexID vid;
  VertexID pvid;

  void run(VertexID e1, VertexID e2) {
    // initialize
    vid = 1;
    bool Xe1e2 = false;
    bool terminate = false;
    DIGRAPHBASIC *dg = graphDB[0];
    VertexID u = 1;
    VertexID v1 = e1;
    VertexID v2 = e2;

    ddg = new DIGRAPHDDG();
    DDGVertex tmp_ddgv;
    // initialize a node in ddg
    tmp_ddgv.p.u = v1;
    tmp_ddgv.p.v = v2;
    ddg->insertVertex(vid, tmp_ddgv);
    pvid = vid;
    vid++;

    // while Xe1e2 is *false* and not *terminate*
    while (!Xe1e2 && !terminate) {
      // for each query i
      for (int i = 0; i < q_cnt; i++) {
        DIGRAPHBASIC *dq = queryDB[i];

        // TODO
        // do BFS before that

        // run
        EL el(dq, u, dg, v1, v2);
        el.run();

        // update Xe1e2
        updateDependency(i, el, v1, v2);

        ddg->printGraph(cout);
        return;
      }

      // determine next
      Pair p = retreiveNext();
      v1 = p.u;
      v2 = p.v;
    }
  }

  void propagate(VertexID v, ULong ul) {
    // if v (in ddg) is determined
    if (ddg->getVLabel(v).determined) {
      // propagate by inEdges.
//      ddg->getVLabel(v).AllXPairHash[ul];

//      // for each vp in ddg, (vp, v)
//      for (typename DIGRAPHDDG::AdjListBool::iterator it = ddg->getInVertex()[v]
//          .begin(); it != ddg->getInVertex()[v].end(); it++) {
//        VertexID vp = it->first;
//
//        DDGVertex& ddgvp = ddg->getVLabel()[vp];
//        // for each mapping
//        for (MapListPair::iterator it1 = ddgvp.LXPair.begin();
//            it1 != ddgvp.LXPair.end(); it1++) {
//          // for each pair
//          for (int k = 0; k < it1->second.size(); k++) {
//
//          }
//        }
//
//      }
//
//    } else {
//      // v is not determined
//      return;
    }
  }

  // update dependency graph
  void updateDependency(int i, EL& el, VertexID v1, VertexID v2) {
    DDGVertex tmp_ddgv;
    tmp_ddgv.p.u = v1;
    tmp_ddgv.p.v = v2;

    // result
    if (el.determined) {
      if (el.terminate) {
        // true
        tmp_ddgv.Xv1v2Qi = true;
        tmp_ddgv.determined = true;
      } else {
        // no mapping, false
        tmp_ddgv.Xv1v2Qi = false;
        tmp_ddgv.determined = true;
      }
      // insert vertex
      ddg->insertVertex(vid, tmp_ddgv);
      // insert edge
      ddg->insertEdge(pvid, vid, i);
      vid++;
      return;
    } else {
      // have mapping, boolean variables
      tmp_ddgv.determined = false;
      tmp_ddgv.AllXPairHash = el.AllXPairHash;
      tmp_ddgv.LXPair = el.LXPair;
    }

    // check conflict
    // for each pair in vid
    for (MapPairHash::iterator it = tmp_ddgv.AllXPairHash.begin();
        it != tmp_ddgv.AllXPairHash.end(); it++) {
      ULong ul = it->first;

      // check and update global and local
      if (GlobalAllXPairHash.find(ul) != GlobalAllXPairHash.end()) {
        tmp_ddgv.AllXPairV[ul] = GlobalAllXPairHash[ul];
      } else {
        GlobalAllXPairHash[ul] = vid;
      }
    }

    // insert vertex
    ddg->insertVertex(vid, tmp_ddgv);
    // insert edge
    ddg->insertEdge(pvid, vid, i);
    vid++;
    return;
  }

  // get next vertex pair
  Pair retreiveNext() {

    // determine pvid
    pvid;
  }

};

#endif /* TESTKB_H_ */
