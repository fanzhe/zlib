/*
 * testCrypto.h
 *
 *  Created on: Apr 25, 2014
 *      Author: zfan
 */

#ifndef TESTCRYPTO_H_
#define TESTCRYPTO_H_

#include "../crypto/cgbe.h"

class TestCrypto{
 public:
  TestCrypto() {
    CGBE* cgbe = new CGBE();
    mpz_t a, b, c;
    mpz_init(a);
    mpz_init(b);
    mpz_init(c);

    cgbe->setvalue(a, cgbe->encoding);
    cgbe->setvalue(b, cgbe->encoding);
    cgbe->setvalue(c, cgbe->encoding);

    cgbe->encrypt(a, a);
    cgbe->encrypt(b, b);
    cgbe->encrypt(c, c);

    int i = 0;
    for (; i < 100000; i++) {
      cgbe->add(a, a, b);
    }
//    cgbe->add(a, a, b);
//    cgbe->add(a, a, c);

    cgbe->decrypt(a, a, 1);
    gmp_printf("test result: %Zd \n", a);
  }
};


#endif /* TESTCRYPTO_H_ */
