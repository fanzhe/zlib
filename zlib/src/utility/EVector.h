/*
 * EVector.h
 *
 *  Created on: May 5, 2014
 *      Author: zfan
 */

#ifndef EVECTOR_H_
#define EVECTOR_H_

#include <vector>

template<class T>
class ECVector : public std::vector<T> {
 public:
  ECVector(size_t size)
      : std::vector<T>(size) {
  }
  ECVector()
      : std::vector<T>() {
  }
  ~ECVector() {
    if (_Myfirst != 0) {
      this->_Alval.deallocate(_Myfirst, _Myend - _Myfirst);
    }
    _Myfirst = 0, _Mylast = 0, _Myend = 0;
  }

  void DeallocateContainingObjects() {
    _Destroy(_Myfirst, _Mylast);
  }

  void clearWithoutErase() {
    _Mylast = _Myfirst;
  }

  inline reference operator[](size_type _Pos) {
    return (*(_Myfirst + _Pos));
  }

  inline const_reference operator[](size_type _Pos) const {
    return (*(_Myfirst + _Pos));
  }
};

#endif /* EVECTOR_H_ */
