#ifndef CATCHE_H
#define CATCHE_H

#include "../utility/GlobalDefinition.h"

class Cache {
 public:
  set<string> ifHasString;
  set<HashCode> ifHasCm;
  set<VertexID> ifRootVertex;
};

#endif
