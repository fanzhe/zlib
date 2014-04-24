/*
 * BigMatrix.h
 *
 *  Created on: Apr 24, 2014
 *      Author: zfan
 */

#ifndef BIGMATRIX_H_
#define BIGMATRIX_H_

#include <gmp.h>

class BigMatrix {
 public:
  int row, col;
  mpz_t** matrix;

  BigMatrix(int _row, int _col);
  ~BigMatrix();

  void matrixFree ();
  void setValue(int _row, int _col, int _v);
  void setValue(int _row, int _col, mpz_t _v);

 private:
  void matrixInit();
};


#endif /* BIGMATRIX_H_ */
