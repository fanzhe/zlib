/*
 * BoolVar.h
 *
 *  Created on: Sep 5, 2014
 *      Author: zfan
 */

#ifndef BOOLVAR_H_
#define BOOLVAR_H_

class BoolVar {
 public:
  bool determined;
  bool value;

  BoolVar() {
    determined = false;
    value = false;
  }

  ~BoolVar() {

  }


};



#endif /* BOOLVAR_H_ */
