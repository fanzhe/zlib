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

    cgbe->setvalue(a, 1);
    cgbe->setvalue(b, 1);

    cgbe->encrypt(a, a);
    cgbe->encrypt(b, b);

    cgbe->mul(a, a, b);

    cgbe->decrypt(a, a, 2);
    gmp_printf("test result: %Zd \n", a);
  }
};


#endif /* TESTCRYPTO_H_ */
