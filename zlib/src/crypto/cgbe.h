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
  mpz_t I;
  mpz_t r, g, x, gx, gx_1, n;
  gmp_randstate_t r_state;

  // scheme functions
  CGBE();
  ~CGBE();
  void encrypt(mpz_t& m, mpz_t& c);
  void decrypt(mpz_t& c, mpz_t& m);
  void decrypt(mpz_t& c, mpz_t& m, int cnt);

  void setvalue(mpz_t& _d, mpz_t& _s);
  void setvalue(mpz_t& _d, int _s);
  void mul(mpz_t& _rs, mpz_t& _l, mpz_t& _r);
  void add(mpz_t& _rs, mpz_t& _l, mpz_t& _r);
  bool isZero(mpz_t& _m);

 private:
  // utilities
  void genRand(mpz_t& _r, int _size);
  void _mulmod(mpz_t& res, mpz_t& a, mpz_t& b, mpz_t& mod);
  void _addmod(mpz_t& res, mpz_t& a, mpz_t& b, mpz_t& mod);

  void generator();
};

#endif /* CGBE_H_ */
