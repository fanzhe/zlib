/*
 * cgbe.h
 *
 *  Created on: Apr 24, 2014
 *      Author: zfan
 */

#ifndef CGBE_H_
#define CGBE_H_

#include "../utility/GlobalDefinition.h"
#include <gmp.h>
#include <gmpxx.h>

class CGBE {
 public:
  // variables
  mpz_t encoding;
  mpz_t r, g, x, gx, gx_1, n;
  gmp_randstate_t r_state;

  // scheme functions
  CGBE();
  ~CGBE();
  void generator();
  void encrypt(mpz_t& m, mpz_t& c);
  void decrypt(mpz_t& c, mpz_t& m);
  void decrypt(mpz_t& c, mpz_t& m, int cnt);

  // utilities
  void genRand(mpz_t& _r, int _size);
  void _mulmod(mpz_t& res, mpz_t& a, mpz_t& b, mpz_t& mod);
  void _addmod(mpz_t& res, mpz_t& a, mpz_t& b, mpz_t& mod);
};

#endif /* CGBE_H_ */
