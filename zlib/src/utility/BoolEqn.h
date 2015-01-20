/*
 * BoolEqn.h
 *
 *  Created on: Aug 8, 2014
 *      Author: zfan
 */

#ifndef BOOLEQN_H_
#define BOOLEQN_H_

#include "GlobalDefinition.h"
#include "Pair.h"

class EntityPair {
 public:
  Pair p;
  bool determined;
  bool flag;

  EntityPair() {
    determined = false;
  }

  EntityPair(const Pair& _p) {
    p = _p;
    determined = false;
  }

  EntityPair(VertexID u, VertexID v)
      : p(u, v),
        determined(false),
        flag(false) {
  }

  friend inline ostream& operator <<(ostream& out, const EntityPair& obj) {
    out << obj.p;
    return out;
  }

  EntityPair& operator =(const EntityPair& _p) {
    p = _p.p;
    determined = _p.determined;
    flag = _p.flag;
    return *this;
  }

//  EntityPair& operator =(const Pair& _p) {
//    p = _p;
//    determined = false;
//    return *this;
//  }

  bool operator ==(const EntityPair& _p1) {
    return (p == _p1.p);
  }
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
