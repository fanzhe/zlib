#ifndef TRANSGSPANGRAPH
#define TRANSGSPANGRAPH

#include "../gSpan/gspan.h"
#include "../utility/graph.h"

bool Convert(GSPAN::Graph& from, GRAPH& to);
bool Convert(GRAPH& from, GSPAN::Graph& to);
bool Convert(GSPAN::DFSCode& dfs_code, GRAPH& to);
bool Convert(GSPAN::DFS* dfs_code, int dfs_size, GRAPH& to);

#endif
