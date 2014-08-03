/*
 * Simulation.h
 *
 *  Created on: 2014¦~8¤ë2¤é
 *      Author: zfan
 */

#ifndef SIMULATION_H_
#define SIMULATION_H_

#include "DiGraph.h"

class Simulation {
 public:
  Simulation(DIGRAPH* _dg, DIGRAPH* _dq) {
    dg = _dg;
    dq = _dq;
  }
  DIGRAPH* dg;
  DIGRAPH* dq;

  void run() {
    // check dg \Sim dq
  }
};

#endif /* SIMULATION_H_ */
