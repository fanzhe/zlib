/*
 * cgbe.cpp
 *
 *  Created on: Apr 24, 2014
 *      Author: zfan
 */

#include "cgbe.h"
#include <iostream>
using namespace std;

CGBE::CGBE() {
  mpz_init(encoding);
  mpz_init(r);
  mpz_init(g);
  mpz_init(x);
  mpz_init(gx);
  mpz_init(gx_1);
  mpz_init(n);
  mpz_init(I);

  // random
  gmp_randinit_default(r_state);
  gmp_randinit_mt(r_state);

  generator();
}

CGBE::~CGBE() {
  mpz_clear(encoding);
  mpz_clear(r);
  mpz_clear(g);
  mpz_clear(x);
  mpz_clear(gx);
  mpz_clear(gx_1);
  mpz_clear(n);
  mpz_clear(I);
}

void CGBE::setvalue(mpz_t& _d, mpz_t& _s) {
  mpz_set(_d, _s);
}

void CGBE::setvalue(mpz_t& _d, int _s) {
  mpz_set_d(_d, _s);
}

void CGBE::mul(mpz_t& _rs, mpz_t& _l, mpz_t& _r) {
  _mulmod(_rs, _l, _r, n);
}

void CGBE::add(mpz_t& _rs, mpz_t& _l, mpz_t& _r) {
  _addmod(_rs, _l, _r, n);
}

void CGBE::genRand(mpz_t& _r, int _size) {
  mpz_urandomb(_r, r_state, _size);
}

void CGBE::generator() {
  // encoding
  mpz_set_d(encoding, 2147483647);

  // x, n
  mpz_set_str(
      n,
      "16594900510468848618779859529048970562434151887982758146333908610126995606875959583150754096904939163058088732836622155984787148951754484085080055100022863834734805390568247562546896289033380553816126638204887528488669669650795595078243509793710242842807549471564349489475385605450605971121686746302940877979192119330073120037897310684720105742251875954769621467756551735273135476454843803189963924528556657594267637567873522001475725298702714900996470201476441839588209221083986823968709674978326806081379946096271872621299301620422089624262355737113475982440526827509351111547011077378815198009079208558801443913617",
      10);
  genRand(x, DEFAULTMSGSIZE);

  // g, gx, gx_1
  mpz_set_d(g, 3);
  mpz_powm(gx, g, x, n);
  mpz_invert(gx_1, gx, n);
}

void CGBE::encrypt(mpz_t& m, mpz_t& c) {
  genRand(r, DEFAULTRANDOM);
  mpz_mul(c, m, r);
  _mulmod(c, c, gx, n);
}

void CGBE::decrypt(mpz_t& c, mpz_t& m) {
  // m = c * gx_1 % n
  _mulmod(m, c, gx_1, n);

  // m = m % encoding
  mpz_mod(m, m, encoding);
}

bool CGBE::isZero(mpz_t& _m) {
  if (mpz_cmp_d(_m, 0) == 0) {
    return true;
  }
  return false;
}

void CGBE::decrypt(mpz_t& c, mpz_t& m, int cnt) {
  mpz_t gx_m, _cnt;
  mpz_init(gx_m);
  mpz_init(_cnt);
  mpz_set_d(_cnt, cnt);

  // gx_m = gx_1 ^ _cnt
  mpz_powm(gx_m, gx_1, _cnt, n);

  // m = (c * gx_m) % n
  _mulmod(m, c, gx_m, n);

  // m = m % encoding
  mpz_mod(m, m, encoding);

  mpz_clear(_cnt);
  mpz_clear(gx_m);
}

void CGBE::_addmod(mpz_t& res, mpz_t& a, mpz_t& b, mpz_t& mod) {
  mpz_add(res, a, b);
  mpz_mod(res, res, mod);
}

void CGBE::_mulmod(mpz_t& res, mpz_t& a, mpz_t& b, mpz_t& mod) {
  mpz_mul(res, a, b);
  mpz_mod(res, res, mod);
}
