#ifndef GLOBALDEFINITION_H
#define GLOBALDEFINITION_H

#include <assert.h>
#include <map>
#include <set>
#include <iostream>
#include <tr1/unordered_map>

using namespace std;
using namespace std::tr1;

typedef int GraphID;
typedef int VertexID;
typedef int EdgeID;
typedef int Label;
typedef Label EdgeLabel;
typedef Label VertexLabel;
typedef unsigned int HashCode;
typedef unordered_map<VertexLabel, set<VertexID> > VertexLabelMap;
typedef unordered_map<VertexLabel, int> VertexLabelMapCnt;
typedef unsigned int HashCode;

typedef int Algorithm;

typedef int Status;

#define 		OK					(1)

#undef			ASSERT
#define			ASSERT(f)			assert((f))

#define			NO_EDGE				(-1)
#define     NO_VERTEX     (-1)
#define			NO_LABEL 			(-1)
#define 		INVALID_GRAPH_ID 	(-1)
#define			ALGORITHM_FGINDEX 	(-1)  // useless, for gspan compile#define 		FILENAME_BUFF_SIZE 	(256)#define			DEFAULT_VERTEX_NUMBER	(256)//#define 		max(X,Y) 			((X) > (Y) ? : (X) : (Y);)//#define 		min(X,Y) 			((X) < (Y) ? : (X) : (Y);)#define     DEFAULT_MAX_VCNT    (10000)

#define     MATRIX_INDEX(u,v,Vcnt)    ((u) * (Vcnt) + (v))

#define     DEFAULTMSGSIZE  2048
#define     DEFAULTRANDOM   32
#endif
