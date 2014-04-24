/*
 * BigMatrix.cpp
 *
 *  Created on: Apr 24, 2014
 *      Author: zfan
 */

#include "BigMatrix.h"
#include <stdlib.h>
#include <stdio.h>

BigMatrix::BigMatrix(int _row, int _col) {
  row = _row;
  col = _col;
  matrixInit();
}

BigMatrix::~BigMatrix() {
  matrixFree();
}

void BigMatrix::matrixInit() {
  matrix = (mpz_t**) malloc(row * sizeof(mpz_t*));
  for (int i = 0; i < row; i++) {
    *(matrix + i) = (mpz_t*) malloc(col * sizeof(mpz_t));
  }

  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      mpz_init(matrix[i][j]);
      mpz_set_d(matrix[i][j], 0);
    }
  }
}

void BigMatrix::matrixFree() {
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++)
      mpz_clear(matrix[i][j]);
  }

  for (int i = 0; i < row; i++) {
    free(matrix[i]);
  }
  free(matrix);
}

void BigMatrix::setValue(int _row, int _col, int _v) {
  mpz_set_d(matrix[_row][_col], _v);
}

void BigMatrix::setValue(int _row, int _col, mpz_t _v) {
  mpz_set(matrix[_row][_col], _v);
}
