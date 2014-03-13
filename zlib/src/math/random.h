/*
 * random.h
 *
 *  Created on: Mar 13, 2014
 *      Author: zfan
 */

#ifndef RANDOM_H_
#define RANDOM_H_

#include <time.h>

class Random {
 public:
  Random() {
  }

  ~Random() {
  }

  static int genRanInt(int range) {
    srand(time(NULL));
    return rand() % range;
  }
};

#endif /* RANDOM_H_ */
