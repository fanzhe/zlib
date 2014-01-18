#ifndef GSPAN_H
#define GSPAN_H

#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include "../utility/GlobalDefinition.h"

#if MATLAB
#include <mex.h>
#endif

typedef struct IntPair {
  int first;
  int second;

  IntPair() {
  }
  IntPair(int f, int s)
      : first(f),
        second(s) {
  }
} IntPair;

namespace GSPAN {

template<class T> inline void _swap(T &x, T &y) {
  T z = x;
  x = y;
  y = z;
}

struct Edge {
  int from;
  int to;
  int elabel;
  unsigned int id;
  Edge()
      : from(0),
        to(0),
        elabel(0),
        id(0) {
  }
};

class Vertex {
 public:
  typedef std::vector<Edge>::iterator edge_iterator;

  int label;
  std::vector<Edge> edge;

  void push(int from, int to, int elabel) {
    edge.resize(edge.size() + 1);
    edge[edge.size() - 1].from = from;
    edge[edge.size() - 1].to = to;
    edge[edge.size() - 1].elabel = elabel;
    return;
  }
};

class Graph : public std::vector<Vertex> {
 private:
  unsigned int edge_size_;
 public:
  typedef std::vector<Vertex>::iterator vertex_iterator;

  Graph(bool _directed) {
    directed = _directed;
  }
  ;
  bool directed;

  unsigned int edge_size() {
    return edge_size_;
  }
  unsigned int vertex_size() {
    return (unsigned int) size();
  }  // wrapper
  void buildEdge();
  std::istream &read(std::istream &);  // read
  std::ostream &write(std::ostream &);  // write
#if MATLAB
      void read (const mxArray* graph);
      void mexprint (void);
      mxArray* writemex (void);
#endif
  void check(void);

  Graph()
      : edge_size_(0),
        directed(false) {
  }
  ;
};

class DFS {
 public:
  VertexID from;
  VertexID to;
  VertexLabel fromlabel;
  EdgeLabel elabel;
  VertexLabel tolabel;
  friend bool operator ==(const DFS &d1, const DFS &d2) {
    return (d1.from == d2.from && d1.to == d2.to && d1.fromlabel == d2.fromlabel
        && d1.elabel == d2.elabel && d1.tolabel == d2.tolabel);
  }
  friend bool operator !=(const DFS &d1, const DFS &d2) {
    return (!(d1 == d2));
  }
  friend bool operator <(const DFS &d1, const DFS& d2) {
    if (d1.fromlabel == d2.fromlabel) {
      if (d1.elabel == d2.elabel)
        return d1.tolabel < d2.tolabel;
      else
        return d1.elabel < d2.elabel;
    } else
      return d1.fromlabel < d2.fromlabel;
  }
  DFS()
      : from(0),
        to(0),
        fromlabel(0),
        elabel(0),
        tolabel(0) {
  }
  ;
};

typedef std::vector<int> RMPath;

struct DFSCode : public std::vector<DFS> {
 private:
  RMPath rmpath;
 public:
  const RMPath& buildRMPath();

  /* Convert current DFS code into a graph.
   */
  bool toGraph(Graph &);

  /* Clear current DFS code and build code from the given graph.
   */
  void fromGraph(Graph &g);

  /* Return number of nodes in the graph.
   */
  unsigned int nodeCount(void);

  unsigned int hashCode(void) const;

  HashCode getStartDiffPos(DFSCode& diff);

  void push(int from, int to, int fromlabel, int elabel, int tolabel) {
    resize(size() + 1);
    DFS &d = (*this)[size() - 1];

    d.from = from;
    d.to = to;
    d.fromlabel = fromlabel;
    d.elabel = elabel;
    d.tolabel = tolabel;
  }
  void pop() {
    resize(size() - 1);
  }
  std::ostream &write(std::ostream &);  // write
};

struct PDFS {
  unsigned int id;	// ID of the original input graph
  unsigned int inner_support;
  Edge *edge;
  PDFS *prev;
  PDFS()
      : id(0),
        edge(0),
        prev(0) {
  }
  ;
};

class History : public std::vector<Edge*> {
 private:
  std::vector<int> edge;
  std::vector<int> vertex;

 public:
  bool hasEdge(unsigned int id) {
    if (edge[id])
      return true;
    else
      return false;
  }
  bool hasVertex(unsigned int id) {
    if (vertex[id])
      return true;
    else
      return false;
  }
  void build(Graph &, PDFS *);
  History() {
  }
  ;
  History(Graph& g, PDFS *p) {
    build(g, p);
  }

};

class Projected : public std::vector<PDFS> {
 public:
  void push(int id, Edge *edge, PDFS *prev) {
    resize(size() + 1);
    PDFS &d = (*this)[size() - 1];
    d.id = id;
    d.edge = edge;
    d.prev = prev;
  }
};

typedef std::vector<Edge*> EdgeList;

bool get_forward_pure(Graph&, Edge *, int, History&, EdgeList &);
bool get_forward_rmpath(Graph&, Edge *, int, History&, EdgeList &);
bool get_forward_root(Graph&, Vertex&, EdgeList &);
Edge *get_backward(Graph&, Edge *, Edge *, History&);

class gSpan_Reporter {
 public:
  virtual void Report(int id, int support, std::vector<IntPair>& gid_list,
                      DFSCode& dfs) = 0;
  virtual void Report(int id, int support, std::vector<GraphID>& gid_list,
                      DFSCode& dfs) = 0;
  virtual void ReportInfrequent(int id, int support,
                                std::vector<GraphID>& gid_list,
                                DFSCode& dfs) = 0;
  virtual size_t Size() = 0;
};

class gSpan {

 public:
  typedef std::map<int, std::map<int, std::map<int, Projected> > > Projected_map3;
  typedef std::map<int, std::map<int, Projected> > Projected_map2;
  typedef std::map<int, Projected> Projected_map1;
  typedef std::map<int, std::map<int, std::map<int, Projected> > >::iterator Projected_iterator3;
  typedef std::map<int, std::map<int, Projected> >::iterator Projected_iterator2;
  typedef std::map<int, Projected>::iterator Projected_iterator1;
  typedef std::map<int, std::map<int, std::map<int, Projected> > >::reverse_iterator Projected_riterator3;

 private:

  std::vector<Graph> TRANS;
  DFSCode DFS_CODE;
  DFSCode DFS_CODE_IS_MIN;
  Graph GRAPH_IS_MIN;

  int callCount;
#if MATLAB
  bool mex;  // Shall we output to matlab structures?
  class mexOutputG {
  public:
    struct mexOutputG* next;
    mxArray* graph;
    double par;
    std::map<unsigned int, unsigned int> counts;
  };
  mexOutputG* mexOutputRoot;
  mexOutputG* mexOutputLast;
  void mexAppendGraph (Graph* g, unsigned int ID, double par,
      std::map<unsigned int, unsigned int>& counts);
#endif

  unsigned int infrequent_edge_ID;
  unsigned int ID;
  unsigned int minsup;
  unsigned int maxpat_min;	// lower bound on node count
  unsigned int maxpat_max;	// upper bound on node count
  bool where;
  bool enc;
  bool directed;
  std::ostream* os;
  gSpan_Reporter* reporter;

  /* Singular vertex handling stuff
   * [graph][vertexlabel] = count.
   */
  std::map<unsigned int, std::map<unsigned int, unsigned int> > singleVertex;
  std::map<unsigned int, unsigned int> singleVertexLabel;

  /* Graph boosting variables
   */
  bool boost;
  double boostTau;	// lower bound to be a suitable pattern.
  unsigned int boostN;	// Number of top-n graphs to collect
  double boostWeightSum;	// \sum_{i=1}^L y_i d_i
  int boostType;	// 1: 1.5-class, 2: 2-class LPBoosting
  std::vector<double> boostY;  // +1/-1 labels
  std::vector<double> boostWeights;  // sample weights

  std::vector<Graph> bestGraphs;
  std::vector<double> bestGraphsY;
  std::vector<double> bestGraphsGain;
  std::vector<std::map<unsigned int, unsigned int> > bestGraphsCounts;
  unsigned int boostseen;
  unsigned int boostmax;
  int (*support_func)(int graph_size);

  /* Transparent pointers for gain function and gain bound.
   */
  double (gSpan::*gain)(Projected &projected, double y);
  double (gSpan::*gainbound)(Projected &projected);

  /* Graph boosting methods
   */
  double gainbound2(Projected &projected);
  double gainbound1d5(Projected &projected);

  double gain2(Projected &projected, double y);
  double gain1d5(Projected &projected, double y);

  void report_boosting(Projected &projected, unsigned int sup, double gain,
                       double yval);
  void report_boosting_inter(Graph &g, unsigned int sup, double gain,
                             double yval,
                             std::map<unsigned int, unsigned int>& GYcounts);
  void report_single(Graph &g, std::map<unsigned int, unsigned int>& ncount);

  bool is_min();
  bool project_is_min(Projected &);

  std::map<unsigned int, unsigned int> support_counts(Projected &projected);
  unsigned int support(Projected&);
  void project(Projected &);
  void report(Projected &, unsigned int, bool frequent = true);

  std::istream &read(std::istream &);
#if MATLAB
  void read (const mxArray* graphs);
#endif

  void run_intern(void);

 public:
  gSpan(void);

  /* boostType: 1 for 1.5-class LPBoosting, 2 for 2-class LPBoosting
   */
  void boost_setup(unsigned int _boostN, double _boostTau,
                   unsigned int _boostmax, std::vector<double>& _boostY,
                   std::vector<double>& _boostWeights, int boostType);

  void run(std::istream &is, std::ostream &_os, gSpan_Reporter& _reporter,
           int (*_support_func)(int), unsigned int _minsup,
           unsigned int _maxpat_min, unsigned int _maxpat_max, bool _enc,
           bool _where, bool _directed);

#if MATLAB
  // For use with Matlab.
  void run_graphs (const mxArray* graphs, int nlhs, mxArray* plhs[],
      unsigned int _minsup,
      unsigned int _maxpat_min, unsigned int _maxpat_max,
      bool _enc,
      bool _where,
      bool _directed);
#endif
};
}
;

#endif
