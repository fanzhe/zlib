/*
 * EL.h
 *
 *  Created on: 2014¦~8¤ë2¤é
 *      Author: zfan
 */

#ifndef EL_H_
#define EL_H_

#include "DiGraph.h"

class EL {
 public:
  DIGRAPH* dg;
  DIGRAPH* dq;
  VertexID e1, e2;

  EL(DIGRAPH* _dg, DIGRAPH* _dq) {
    dg = _dg;
    dq = _dq;
  }

  void run(VertexID _e1, VertexID _e2) {
    // check if (dg, dq) |= (e1, e2)
    e1 = _e1, e2 = _e2;
    // search candidate
    DIGRAPH* ge1q = search(e1);
    DIGRAPH* ge2q = search(e2);

    // product
    DIGRAPH* gq = product(ge1q, ge2q);

    // check
    check(gq);
  }

  /*
   * G_{e}^Q
   */
  DIGRAPH* search(VertexID e) {
    DIGRAPH* geq = new DIGRAPH();
    geq->initEL();
    // ge: subgraph at e in dg

    return geq;
  }

  /*
   * G^Q = G_{e1}^Q \times G_{e2}^Q
   */
  DIGRAPH* product(DIGRAPH* ge1q, DIGRAPH* ge2q) {
    DIGRAPH* gq = new DIGRAPH();

    return gq;
  }

  /*
   * check e1 = e2 by q via subIso
   */
  bool check(DIGRAPH* gq) {

  }
};


#endif /* EL_H_ */
