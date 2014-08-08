#include "utilityFunction.h"

#include <set>

#include "graph.h"
#include "GlobalDefinition.h"

void setVertexNeighborToSet(set<VertexID>& _set, VertexID v, GRAPH* g) {
  for (int i = 0; i < g->getDegree(v); i++) {
    VertexID v = g->_adjList[v][i].v;
    _set.insert(v);
  }
}

string convertInt(int number) {
  stringstream ss;  //create a stringstream
  ss << number;  //add number to the stream
  return ss.str();  //return a string with the contents of the stream
}

double gettime(clock_t s, clock_t e) {
  return ((double) (e - s)) / CLOCKS_PER_SEC;
}

long long sumUpVertexLabelCnt(VertexLabelMapCnt _a, VertexLabelMapCnt _b) {
  long long res = 1;
  for (VertexLabelMapCnt::iterator it = _a.begin(); it != _a.end(); it++) {
    res *= permutation(_a[it->first], _b[it->first]);
  }
  return res;
}

long long permutation(int k, int n) {
  ASSERT(k <= n);
  int f = 1;
  for (int i = n; i >= (n - k + 1); i--) {
    f = f * i;
  }
  return f;
}

long long factorial(int r) {
  int f = 1;
  for (int i = 2; i <= r; i++) {
    f = f * i;
  }
  return f;
}

/*
 * From:
 * http://stackoverflow.com/questions/919612/mapping-two-integers-to-one-in-a-unique-and-deterministic-way
 */
ULong pairFunction(VertexID a, VertexID b) {
  return a >= b ? (ULong) a * (ULong) a + a + b : a + (ULong) b * (ULong) b;
}
