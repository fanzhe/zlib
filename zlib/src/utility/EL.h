/*
 * EL.h
 *
 *  Created on: 2014¦~8¤ë2¤é
 *      Author: zfan
 */

#ifndef EL_H_
#define EL_H_

#include "DiGraph.h"
#include <stack>

class Pair {
 public:
  VertexID u;
  VertexID v;
  Pair(VertexID _u, VertexID _v): u(_u), v(_v) {

  }
};

class EL {
 public:
  DIGRAPH* dg;
  DIGRAPH* dq;
  VertexID e1, e2;

  EL(DIGRAPH* _dq, VertexID u, DIGRAPH* _dg, VertexID v) {
    dg = _dg;
    dq = _dq;

    // initialize size
    dq->initEL(1);
    dg->initEL(1);

    dq->initEdgeVisited();
  }

  void run(VertexID _e1, VertexID _e2) {
    // check if (dg, dq) |= (e1, e2)
    e1 = _e1, e2 = _e2;
    // search candidate
    DIGRAPH* ge1q = search();
    DIGRAPH* ge2q = search();

    // product
    DIGRAPH* gq = product(ge1q, ge2q);

    // check
    check(gq);
  }

  bool checkLabelAndDeg(VertexID u, VertexID v) {
    if (dq->getVLabel(u) != dg->getVLabel(v)) {
      return false;
    }
    for (LabelCnt::iterator it = dq->_outLabelCnt[u].begin();
        it != dq->_outLabelCnt[u].end(); it++) {
      Label ul = it->first;
      if (dg->_outLabelCnt[v].find(ul) == dg->_outLabelCnt[v].end())
        return false;
      if (dq->_outLabelCnt[u][ul] > dg->_outLabelCnt[v][ul]) {
        return false;
      }
    }

    for (LabelCnt::iterator it = dq->_inLabelCnt[u].begin();
        it != dq->_inLabelCnt[u].end(); it++) {
      Label ul = it->first;
      if (dg->_inLabelCnt[v].find(ul) == dg->_inLabelCnt[v].end())
        return false;
      if (dq->_inLabelCnt[u][ul] > dg->_inLabelCnt[v][ul]) {
        return false;
      }
    }

    return true;
  }

  /*
   * TODO: test
   * G_{e}^Q
   */
  DIGRAPH* search() {
    DIGRAPH* geq = new DIGRAPH();
    // C(u, [v]): candidate map

    // L(q, e) != L(g, e)
    VertexID u = dq->e;
    VertexID v = dg->e;
    VertexID up, vp;
    if (!checkLabelAndDeg(u, v)) {
      return NULL;
    }

    stack<Pair> S;
    S.push(Pair(dq->e, dg->e));

    while (!S.empty()) {
      Pair p = S.top();
      S.pop();
      u = p.u;
      v = p.v;
      if (dq->Cq[u].find(v) == dq->Cq[u].end()) {
        // insert p
        dq->Cq[u].insert(v);
      }

      // for each unvisited edge (u, up) in dq
      for (AdjList::iterator it = dq->getOutEdge()[u].begin();
          it != dq->getOutEdge()[u].end(); it++) {
        up = it->first;
        // (u, up) is visited
        if (it->second.isVisited) {
          continue;
        }
        it->second.isVisited = true;

        // pick vp in dg
        for (AdjList::iterator it1 = dg->getOutEdge()[v].begin();
            it1 != dg->getOutEdge()[v].end(); it1++) {
          vp = it1->first;
          if (dq->getELabel(u, up) != dg->getELabel(v, vp))
            continue;
          if (!checkLabelAndDeg(up, vp))
            continue;
          S.push(Pair(up, vp));
        }
      }

      // for each unvisited edge (up, u) in dq
      for (AdjListBool::iterator it = dq->getInVertex()[u].begin();
          it != dq->getInVertex()[u].end(); it++) {
        up = it->first;
        if (dq->getOutEdge()[up][u].isVisited) {
          continue;
        }
        dq->getOutEdge()[up][u].isVisited = true;

        // pick vp in dg
        for (AdjListBool::iterator it1 = dg->getInVertex()[v].begin();
            it1 != dg->getInVertex()[v].end(); it1++) {
          vp = it1->first;
          if (dq->getELabel(up, u) != dg->getELabel(vp, v))
            continue;
          if (!checkLabelAndDeg(up, vp))
            continue;
          S.push(Pair(up, vp));
        }
      }
    }

    cout << "---- match test" << endl;
    for (CandQtoG::iterator it = dq->Cq.begin(); it != dq->Cq.end(); it++) {
      cout << it->first << " ";
      for (set<VertexID>::iterator it1 = it->second.begin();
          it1 != it->second.end(); it1++) {
        cout << *it1 << " ";
      }
      cout << endl;
    }

    // S <- (e, e)
    return geq;
  }

  /*
   * TODO
   * G^Q = G_{e1}^Q \times G_{e2}^Q
   */
  DIGRAPH* product(DIGRAPH* ge1q, DIGRAPH* ge2q) {
    DIGRAPH* gq = new DIGRAPH();

    return gq;
  }

  /*
   * TODO
   * check e1 = e2 by q via subIso
   */
  bool check(DIGRAPH* gq) {

  }
};

#endif /* EL_H_ */
