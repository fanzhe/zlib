#ifndef GLOBALDEFINITION_H
#define GLOBALDEFINITION_H

#include <assert.h>
#include <map>
#include <set>
#include <iostream>
#include <tr1/unordered_map>
#include <tr1/unordered_set>
#include <vector>
#include <cstdlib>
#include <cstring>

using namespace std;
using namespace std::tr1;

//typedef unsigned long ULong;
typedef int GraphID;
typedef long long VertexID;
typedef int EdgeID;
typedef int Label;
typedef Label EdgeLabel;
typedef Label VertexLabel;
typedef unsigned int HashCode;
typedef unordered_map<VertexLabel, set<VertexID> > VertexLabelMap;
typedef unordered_map<VertexLabel, int> VertexLabelMapCnt;
typedef unsigned int HashCode;

typedef int Algorithm;

typedef int Status;

#define     OK          (1)

#undef      ASSERT
#define     ASSERT(f)     assert((f))

#define     NO_EDGE       (-1)
#define     NO_VERTEX     (-1)
#define     NO_LABEL      (-1)
#define     INVALID_GRAPH_ID  (-1)
#define     ALGORITHM_FGINDEX   (-1)  // useless, for gspan compile#define     FILENAME_BUFF_SIZE  (256)#define     DEFAULT_VERTEX_NUMBER (256)//#define     max(X,Y)      ((X) > (Y) ? : (X) : (Y);)//#define     min(X,Y)      ((X) < (Y) ? : (X) : (Y);)
#define     DEFAULT_MAX_VCNT    (10000)

#define     MATRIX_INDEX(u,v,Vcnt)    ((u) * (Vcnt) + (v))

#define     DEFAULTMSGSIZE        2048
#define     DEFAULTRANDOM         32
#define     DEFAULALPHAFORL       0.1

#define     DEFAULTENTITYFOREACHFILE  1000000
#define     DEFAULTHETAGD             1000000
#define     DEFAULTHETAORGGD          1000000
#define     DEFAULTHETAL              1000000


/**
 * edge definition
 */
class Edge {
 public:
  VertexID v, w;
  EdgeID edge_id;

  EdgeLabel label;

  Edge(EdgeID edge_id = NO_EDGE, VertexID v = NO_EDGE, VertexID w = NO_EDGE,
       EdgeLabel l = NO_LABEL)
      : v(v),
        w(w),
        edge_id(edge_id),
        label(l) {
  }

  Edge(std::string s1, std::string s2) {
    v = atoi(s1.c_str());
    w = atoi(s2.c_str());
  }

  ~Edge() {
  }

  bool operator ==(Edge e) {
    if (e.v == v && e.w == w)
      return true;
    else if (e.w == v && e.v == w)
      return true;

    return false;
  }

  void operator =(const Edge& e) {
    this->v = e.v;
    this->w = e.w;
    this->label = e.label;
    this->edge_id = e.edge_id;
  }
};

//typedef unordered_map<VertexID, VertexLabel> VLabels;
//typedef unordered_map<VertexID, AdjElement> AdjList;
//typedef unordered_map<VertexID, AdjList> OutEdge;
//typedef unordered_map<VertexID, bool> AdjListBool;
//typedef unordered_map<VertexID, AdjListBool> InVertex;
//
typedef unordered_map<Label, int> LabelCnt;
typedef unordered_map<VertexID, LabelCnt> MapLabelCnt;

typedef unordered_map<VertexID, VertexID> Map;

template<class VLabelType, class ELabelType>
class DIGRAPH;

class DGQVertex;
class DDGVertex;
class Pair;

class VertexTriple {
 public:
  VertexID first;
  VertexID second;
  VertexLabel third;

  VertexTriple() {

  }

  VertexTriple(const VertexTriple& _p) {
    first = _p.first;
    second = _p.second;
    third = _p.third;
  }

  VertexTriple(VertexID _f, VertexID _s, VertexLabel _t)
      : first(_f),
        second(_s),
        third(_t) {
  }

  ~VertexTriple() {

  }

  VertexTriple& operator =(const VertexTriple& _p) {
    first = _p.first;
    second = _p.second;
    third = _p.third;
    return *this;
  }

  bool operator ==(const VertexTriple & _p1) {
    return (_p1.first == first && _p1.second == second && _p1.third == third);
  }

  friend inline ostream& operator <<(ostream& out, const VertexTriple& obj) {
    out << "(" << obj.first << ", " << obj.second << ", " << obj.third << ")";
    return out;
  }
};


typedef DIGRAPH<VertexLabel, EdgeLabel> DIGRAPHBASIC;
typedef DIGRAPH<DGQVertex, EdgeLabel> DIGRAPHDGQ;
typedef DIGRAPH<DDGVertex, EdgeLabel> DIGRAPHDDG;

// product graph
typedef VertexTriple EdgeTriple;
#define NOEDGELABEL (-111)
#define NODEP (-111)
#define NOTC  (-111)
typedef DIGRAPH<Pair, EdgeLabel> DIKEYS;
typedef DIGRAPH<VertexTriple, EdgeTriple> DIPRODUCTGRAPH;

typedef unsigned long ULong;
typedef unordered_map<ULong, Pair> MapPairHash;
typedef unordered_map<ULong, VertexID> MapPairVertex;
typedef unordered_map<VertexID, Pair> MapVerPair;
#endif
