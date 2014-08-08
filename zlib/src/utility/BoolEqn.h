/*
 * BoolEqn.h
 *
 *  Created on: Aug 8, 2014
 *      Author: zfan
 */

#ifndef BOOLEQN_H_
#define BOOLEQN_H_

class Pair {
 public:
  VertexID u;
  VertexID v;

  Pair() {

  }

  Pair(VertexID _u, VertexID _v)
      : u(_u),
        v(_v) {
  }

  ~Pair() {

  }

  void operator =(const Pair& _p) {
    u = _p.u;
    v = _p.v;
  }

  bool operator ==(const Pair& _p1, const Pair& _p2) {
    return (_p1.u == _p2.u && _p1.v == _p2.v);
  }
};

class DDGVertex {
 public:
  bool x;
  Pair p;
};

class Disjunction {
 public:
};

class Conjunction {
 public:
};

class BoolEqn {
 public:
};

#endif /* BOOLEQN_H_ */
