/*
 * Message.h
 *
 *  Created on: 2014¦~4¤ë24¤é
 *      Author: zfan
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "gmp.h"
#include "cgbe.h"
#include "../utility/GlobalDefinition.h"

class Message {
 public:
  mpz_t Rk;  // encrypted message
  mpz_t Ri;
  mpz_t R;
  int answer;
  int cnt;  // numbers of message that are aggregated
  int type;

  Message(int _size, int _DEFAULTENCODING) {
    mpz_init(Rk);
    mpz_init(Ri);
    mpz_init(R);

    if (_size < _DEFAULTENCODING) {
      mpz_set_d(Rk, 0);
      mpz_set_d(Ri, 1);
      type = 0;
    } else {
      mpz_set_d(Rk, 1);
      mpz_set_d(Ri, 0);
      type = 1;
    }

    answer = 0;
    cnt = 0;
  }

  void resetRi() {
    if (type == 0) {
      mpz_set_d(Ri, 1);
    } else {
      mpz_set_d(Ri, 0);
    }
  }

  void resetRk() {
    if (type == 0) {
      mpz_set_d(Rk, 0);
    } else {
      mpz_set_d(Rk, 1);
    }
  }
};

#endif /* MESSAGE_H_ */
