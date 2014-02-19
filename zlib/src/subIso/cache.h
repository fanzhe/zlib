#ifndef CATCHE_H
#define CATCHE_H

#include "../utility/GlobalDefinition.h"

class Cache {
 public:
  set<string> ifHasString;
  set<HashCode> ifHasCm;
  set<VertexID> ifRootVertex;

  int cnt_cr;
  int cnt_cm;
  int cnt_cm_p;
};

#endif
