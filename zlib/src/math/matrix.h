/*
 * matrix.h
 *
 *  Created on: Sep 2, 2014
 *      Author: zfan
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include "../utility/GlobalDefinition.h"

void matrix_print(int** a, int a_r, int a_c) {
  for (int i = 0; i < a_r; i++) {
    for (int j = 0; j < a_c; j++)
      cout << a[i][j] << " ";
    cout << endl;
  }
  cout << endl;
}

void matrix_print(int* a, int s) {
  for (int i = 0; i < s; i++) {
    cout << a[i] << " ";
  }
  cout << endl;
}

void matrix_cpy(int** mi, int **m, int a_r, int a_c) {
  for (int i = 0; i < a_r; i++) {
    for (int j = 0; j < a_c; j++)
      mi[i][j] = m[i][j];
  }
}

void matrix_trans(int** a, int a_r, int a_c, int** _a) {
  for (int i = 0; i < a_r; i++) {
    for (int j = 0; j < a_c; j++) {
      _a[j][i] = a[i][j];
    }
  }
}

void matrix_mul(int** a, int a_r, int a_c, int** b, int b_r, int b_c, int** c) {
  int tmp = 0;

  for (int i = 0; i < a_r; i++) {
    for (int j = 0; j < b_c; j++) {
      c[i][j] = 0;
      for (int k = 0; k < a_c; k++) {
        //c[i][j] += a[i][k] * b[k][j];
        tmp = a[i][k] * b[k][j];
        c[i][j] = c[i][j] + tmp;
      }
    }
  }
}

int** matrix_init(int _row, int _col) {
  int** m = new int* [_row];
  for (int i = 0; i < _row; i++) {
    m[i] = new int [_col];
  }

  for (int i = 0; i < _row; i++) {
    for (int j = 0; j < _col; j++) {
      m[i][j] = 0;
    }
  }
  return m;
}

void readMatrix(int** &m, int& row, int& col) {
  // TODO
  ifstream infile("test.txt");

  if (!infile) {
    cout << "error" << endl;
    return;
  }
  string line;

  vector<int> c_v;
  vector<vector<int> > r_v;

  while (getline(infile, line)) {
    istringstream ip(line);
    int e;
    while (ip >> e) {
      c_v.push_back(e);
    }

    r_v.push_back(c_v);
    //cout << r_v.size() << endl;
    col = c_v.size();
    c_v.clear();
  }

  row = r_v.size();

  m = new int*[row];
  for (int i = 0; i < row; i++) {
    m[i] = new int[col];
  }

  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      m[i][j] = r_v[i][j];
    }
  }

  //matrix_print(m, row, col);

}

#endif /* MATRIX_H_ */
