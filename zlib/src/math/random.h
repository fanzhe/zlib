/*
 * random.h
 *
 *  Created on: Mar 13, 2014
 *      Author: zfan
 */

#ifndef RANDOM_H_
#define RANDOM_H_

#include <ctime>
#include <cstdlib>
#include <cstdio>

using namespace std;

class Random {
 public:
  Random() {
    int seed = static_cast<int>(time(0));
    srand(seed);
  }

  ~Random() {
  }

  int genRanInt(int range) {
    return rand() % range;
  }
};

#endif /* RANDOM_H_ */
