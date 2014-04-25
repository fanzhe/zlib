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
  int answer;
  int cnt;  // numbers of message that are aggregated

  Message(int _size) {
    mpz_init(Rk);
    mpz_init(Ri);
    if (_size < DEFAULTENCODING) {
      mpz_set_d(Rk, 0);
      mpz_set_d(Ri, 1);
    } else {
      mpz_set_d(Rk, 1);
      mpz_set_d(Ri, 0);
    }

    answer = 0;
    cnt = 0;
  }
};

#endif /* MESSAGE_H_ */
