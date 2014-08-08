#ifndef UTILITYFUNCTION_H
#define UTILITYFUNCTION_H

//#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <sstream>
#include <ctime>
#include <map>
#include <tr1/unordered_map>

#include "GlobalDefinition.h"
//#include "graph.h"

using namespace std::tr1;

class GRAPH;

using namespace std;

template<class T>
void setToVector(set<T>& _set, vector<T>& _vec) {
  ASSERT(_set.size() == _vec.size());

  int i = 0;
  for (typename set<T>::iterator it = _set.begin(); it != _set.end(); it++) {
    _vec[i] = *it;
    i++;
  }
}

template<class T1, class T2>
void printHashTableTT(unordered_map<T1, T2>& _map) {
  for (typename unordered_map<T1, T2>::iterator it = _map.begin();
      it != _map.end(); it++) {
    cout << it->first << " " << it->second;
    cout << endl;
  }
}

template<class T1, class T2>
void printMapTT(map<T1, T2>& _map) {
  for (typename map<T1, T2>::iterator it = _map.begin(); it != _map.end();
      it++) {
    cout << it->first << " " << it->second;
    cout << endl;
  }
}

template<class T>
void printSet(set<T>& _set) {
  cout << "set size: " << _set.size();
  cout << endl << "set elements: ";
  int _cnt = 0;
  for (typename set<T>::iterator it = _set.begin(); it != _set.end(); it++) {
    cout << *it << " ";
    if (_cnt++ % 100 == 0)
      cout << endl;
  }
  cout << endl;
}

template<class T>
void printVector(vector<T>& _vec) {
  for (int i = 0; i < _vec.size(); i++) {
    cout << _vec[i] << " ";
  }
  cout << endl;
}

template<class T>
void printVectorSet(vector<set<T> >& _vec_set) {
  for (int i = 0; i < _vec_set.size(); i++) {
    cout << i << ": ";
    for (typename set<T>::iterator it = _vec_set[i].begin();
        it != _vec_set[i].end(); it++) {
      cout << *it << ", ";
    }
    cout << endl;
  }
  cout << endl;
}

template<class T>
void clearVectorSet(vector<set<T> >& _vec_set) {
  for (int i = 0; i < _vec_set.size(); i++) {
    _vec_set[i].clear();
  }
  _vec_set.clear();
}

template<class T>
void printMatrix(T* _vec, int _size) {
  for (int i = 0; i < _size; i++) {
    for (int j = 0; j < _size; j++) {
      cout << _vec[MATRIX_INDEX(i, j, _size)] << " ";
    }
    cout << endl;
  }
}

template<class T>
void printArray(T* _vec, int _size) {
  for (int i = 0; i < _size; i++) {
    cout << _vec[i] << " ";
  }
  cout << endl;
}

template<class T>
void setArray(T* _vec, int _size, T _init) {
  for (int i = 0; i < _size; i++) {
    _vec[i] = _init;
  }
}

template<class T>
void removeEleInSet(set<T>& _set, T& e) {
  _set.erase(e);
}

string convertInt(int number);

template<class T>
void setToString(set<T>& _set, string& _str) {
  _str = "";
  for (typename set<T>::iterator it = _set.begin(); it != _set.end(); it++) {
    _str = _str + convertInt(*it);
  }
}

template<class T>
void vectorToString(vector<T>& _vec, int _size, string& _str) {
  _str = "";

  set<T> _set;
  for (int i = 0; i < _size; i++) {
    _set.insert(_vec[i]);
  }

  setToString(_set, _str);
}

template<class T>
void twoSetsIntersection(set<T>& a, set<T>& b, set<T>& c) {
  typename set<T>::iterator it1 = a.begin();
  typename set<T>::iterator it2 = b.begin();
  for (; it1 != a.end() && it2 != b.end();) {
    if (*it1 < *it2)
      ++it1;
    else if (*it2 < *it1)
      ++it2;
    else {
      c.insert(*it1);
      ++it1;
      ++it2;
    }
  }
}

template<class T>
bool twoSetsAreEqual(set<T>& a, set<T>& b) {
  if (a.size() != b.size())
    return false;

  typename set<T>::iterator it1 = a.begin();
  typename set<T>::iterator it2 = b.begin();
  for (; it1 != a.end() && it2 != b.end(); it1++, it2++) {
    if (*it1 != *it2)
      return false;
  }

  return true;
}

template<class T>
bool isInVector(T& a, vector<T>& b) {
  for (int i = 0; i < b.size(); i++) {
    if (b[i] == a) {
      return true;
    }
  }
  return false;
}

void setVertexNeighborToSet(set<VertexID>& _set, VertexID v, GRAPH* g);
double gettime(clock_t s, clock_t e);
long long sumUpVertexLabelCnt(VertexLabelMapCnt a, VertexLabelMapCnt b);
long long permutation(int k, int n);
long long factorial(int r);
ULong pairFunction(VertexID a, VertexID b);

#endif
