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

  void matrixInit();
  void matrixFree ();

};


#endif /* BIGMATRIX_H_ */
