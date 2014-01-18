#ifndef UTILITYFUNCTION_H
#define UTILITYFUNCTION_H

//#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <sstream>

#include "GlobalDefinition.h"
//#include "graph.h"

class GRAPH;

using namespace std;

template<class T>
void printSet(set<T>& _set) {
  cout << "set size: " << _set.size();
  cout << endl << "set elements: ";
  for (typename set<T>::iterator it = _set.begin(); it != _set.end(); it++) {
    cout << *it << " ";
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

void setVertexNeighborToSet(set<VertexID>& _set, VertexID v, GRAPH* g);

#endif
