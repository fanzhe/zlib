#include "gspan.h"
#include <cstring>
#include <string>
#include <iterator>
#include <set>
#include <vector>
#include <utility>

namespace GSPAN {

#define max(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })

#define min(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; })

/* Build a DFS code from a given graph.
 */
void DFSCode::fromGraph(Graph &g) {
  clear();

  EdgeList edges;
  for (unsigned int from = 0; from < g.size(); ++from) {
    if (get_forward_root(g, g[from], edges) == false)
      continue;

    for (EdgeList::iterator it = edges.begin(); it != edges.end(); ++it)
      push(from, (*it)->to, g[(*it)->from].label, (*it)->elabel,
           g[(*it)->to].label);
  }
}

bool DFSCode::toGraph(Graph &g) {
  g.clear();

  for (DFSCode::iterator it = begin(); it != end(); ++it) {
    g.resize(max(it->from, it->to) + 1);

    if (it->fromlabel != -1)
      g[it->from].label = it->fromlabel;
    if (it->tolabel != -1)
      g[it->to].label = it->tolabel;

    g[it->from].push(it->from, it->to, it->elabel);
    if (g.directed == false)
      g[it->to].push(it->to, it->from, it->elabel);
  }

  g.buildEdge();

  return (true);
}

unsigned int DFSCode::nodeCount(void) {
  unsigned int nodecount = 0;

  for (DFSCode::iterator it = begin(); it != end(); ++it)
    nodecount = max(nodecount, (unsigned int) (max (it->from, it->to) + 1));

  return (nodecount);
}

std::ostream &DFSCode::write(std::ostream &os) {
  if (size() == 0)
    return os;

  os << "(" << (*this)[0].fromlabel << ") " << (*this)[0].elabel << " (0f"
     << (*this)[0].tolabel << ")";

  for (unsigned int i = 1; i < size(); ++i) {
    if ((*this)[i].from < (*this)[i].to) {
      os << " " << (*this)[i].elabel << " (" << (*this)[i].from << "f"
         << (*this)[i].tolabel << ")";
    } else {
      os << " " << (*this)[i].elabel << " (b" << (*this)[i].to << ")";
    }
  }

  return os;
}

unsigned int DFSCode::getStartDiffPos(DFSCode& diff) {
  unsigned int pos = 0u;

  unsigned int min_size = (unsigned int) min(size(), diff.size());

  for (; pos < min_size; pos++) {
    if ((*this)[pos].from != diff[pos].from || (*this)[pos].to != diff[pos].to
        || (*this)[pos].fromlabel != diff[pos].fromlabel
        || (*this)[pos].elabel != diff[pos].elabel
        || (*this)[pos].tolabel != diff[pos].tolabel) {
      return pos;
    }

  }
  return pos;
}

unsigned int DFSCode::hashCode() const {
  size_t s = size();
  unsigned int hashcode = 5381u;

  for (unsigned int i = 0; i < s; i++) {
    hashcode = ((hashcode << 5) + hashcode) + ((*this)[i].to);
    hashcode = ((hashcode << 5) + hashcode) + ((*this)[i].from);
    hashcode = ((hashcode << 5) + hashcode) + ((*this)[i].fromlabel);
    hashcode = ((hashcode << 5) + hashcode) + ((*this)[i].elabel);
    hashcode = ((hashcode << 5) + hashcode) + ((*this)[i].tolabel);
  }

  return 0x7fffffff & hashcode;
}
}

