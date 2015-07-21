/*
 * Pair.h
 *
 *  Created on: 2015¦~1¤ë19¤é
 *      Author: zfan
 */

#ifndef PAIR_H_
#define PAIR_H_

#include "GlobalDefinition.h"

class Pair {
 public:
  VertexID u;
  VertexID v;

  Pair() {

  }

  Pair(const Pair& _p) {
    u = _p.u;
    v = _p.v;
  }

  Pair(VertexID _u, VertexID _v)
      : u(_u),
        v(_v) {
  }

  ~Pair() {

  }

  Pair& operator =(const Pair& _p) {
    u = _p.u;
    v = _p.v;
    return *this;
  }

  bool operator ==(const Pair& _p1) {
    return (_p1.u == u && _p1.v == v) || (_p1.u == v && _p1.v == u);
  }

  friend inline ostream& operator <<(ostream& out, const Pair& obj) {
    out << "(" << obj.u << ", " << obj.v << ")";
    return out;
  }
};

#endif /* PAIR_H_ */
