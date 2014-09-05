/*
 * ullman.cpp
 *
 *  Created on: Sep 2, 2014
 *      Author: zfan
 */

#include "../math/matrix.h"
#include "../utility/graph.h"

class Ullman {
 public:

  Ullman(GRAPH* _q, GRAPH* _g) {
    q = _q;
    g = _g;
    row_size = q->V();
    col_size = g->V();
    _ullman();
  }

  ~Ullman() {

  }

  GRAPH* q;
  GRAPH* g;
  int** m;

  int total_count;
  bool result;
  int row_size;
  int col_size;
  double p1, p2, p3;

  void _ullman() {
    clock_t start, end;
    m = matrix_init(row_size, col_size);

    for (int i = 0; i < q->V(); i++) {
      for (int j = 0; j < g->V(); j++) {
        if (q->getLabel(i) == g->getLabel(j)
            && q->getDegree(i) <= g->getDegree(j)) {
          m[i][j] = 1;
        } else {
          m[i][j] = 0;
        }
      }
    }

    start = clock();
    if (!row_col_prune()) {
      result = false;
      return;
    }
    end = clock();
    p1 = gettime(start, end);

    // neighborhood-based prune
    start = clock();
    connect_prune();
    if (!row_col_prune()) {
      result = false;
      return;
    }
    end = clock();
    p2 = gettime(start, end);

//    matrix_print(m, row_size, col_size);
    start = clock();
    int* col = new int[q->V()];
    int* row = new int[g->V()];
    for (int i = 0; i < g->V(); i++) {
      row[i] = 0;
    }
    result = false;
    _enum(0, col, row);
    end = clock();
    p3 = gettime(start, end);
  }

  bool row_col_prune() {
    for (int i = 0; i < q->V(); i++) {
      bool flag2 = true;
      for (int j = 0; j < g->V(); j++) {
        if (m[i][j] == 1) {
          flag2 = false;
          break;
        }
      }
      if (flag2) {
        return false;
      }
    }
    return true;
  }

  void _subiso(int* row, int* col) {
//    matrix_print(col, row_size);
    for (int i = 0; i < q->V(); i++) {
      for (int j = 0; j < q->V(); j++) {
        if (i < j && q->edge(i, j)) {
          int _i = col[i];
          int _j = col[j];
          if (!g->edge(_i, _j)) {
            return;
          }
        }
      }
    }
    result = true;
  }

  void _enum(int i_row, int* col, int* row) {
    // found
    if (result == true) {
      return;
    }

    // done
    if (i_row == row_size) {
      _subiso(row, col);
      return;
    }

    // enumeration
    for (int j = 0; j < col_size; j++) {
      if (row[j] == 0 && m[i_row][j] == 1) {

        row[j] = 1;
        col[i_row] = j;

        _enum(i_row + 1, col, row);

        if (result == true) {
          return;
        }

        row[j] = 0;
      }
    }
  }

  void connect_prune() {
    bool chng;
    do {
      chng = false;
      for (int i = 0; i < row_size; i++) {
        for (int j = 0; j < col_size; j++) {
          if (m[i][j] == 1) {
            for (int k = 0; k < q->getDegree(i); k++) {
              bool flag = false;
              int x = q->_adjList[i][k].v;
              for (int l = 0; l < g->getDegree(j); l++) {
                int y = g->_adjList[j][l].v;
                if (m[x][y] == 1) {
                  flag = true;
                  break;
                }
              }

              if (!flag) {
                chng = true;
                m[i][j] = 0;
                break;
              }
            }
          }
        }
      }
    } while (chng);
  }

};

