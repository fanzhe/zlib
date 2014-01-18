#ifndef SUBGRAPHGEN_H
#define SUBGRAPHGEN_H

#include "../gSpan/gspan.h"
#include <vector>

using namespace std;

class SubGraphGen {
 public:
  std::vector<GSPAN::Graph> TRANS;
  GSPAN::DFSCode DFS_CODE;
  GSPAN::DFSCode DFS_CODE_IS_MIN;
  GSPAN::Graph GRAPH_IS_MIN;
  int graph_size;

  vector<GSPAN::DFSCode> Subgraphs;
 public:
  size_t Size() {
    return Subgraphs.size();
  }

  void Start(GSPAN::DFSCode& dfs_code) {
    GSPAN::Graph super_graph;
    dfs_code.toGraph(super_graph);
    TRANS.push_back(super_graph);
    graph_size = (int) dfs_code.size();

    GSPAN::gSpan::Projected_map3 root;
    GSPAN::EdgeList edges;

    for (unsigned int from = 0; from < super_graph.size(); ++from) {
      if (get_forward_root(super_graph, super_graph[from], edges)) {
        for (GSPAN::EdgeList::iterator it = edges.begin(); it != edges.end();
            ++it) {
          root[super_graph[from].label][(*it)->elabel][super_graph[(*it)->to]
              .label].push(0, *it, 0);
        }
      }
    }

    for (GSPAN::gSpan::Projected_iterator3 fromlabel = root.begin();
        fromlabel != root.end(); fromlabel++) {
      for (GSPAN::gSpan::Projected_iterator2 elabel = fromlabel->second.begin();
          elabel != fromlabel->second.end(); elabel++) {
        for (GSPAN::gSpan::Projected_iterator1 tolabel = elabel->second.begin();
            tolabel != elabel->second.end(); tolabel++) {
          DFS_CODE.push(0, 1, fromlabel->first, elabel->first, tolabel->first);
          _Start(tolabel->second);
          DFS_CODE.pop();
        }

      }
    }

    return;
  }

 protected:

  void _Start(GSPAN::Projected& projected) {
    if (!is_min())
      return;

    Subgraphs.push_back(DFS_CODE);

    if (DFS_CODE.size() == graph_size)
      return;

    const GSPAN::RMPath &rmpath = DFS_CODE.buildRMPath();
    int minlabel = DFS_CODE[0].fromlabel;
    int maxtoc = DFS_CODE[rmpath[0]].to;

    GSPAN::gSpan::Projected_map3 new_fwd_root;
    GSPAN::gSpan::Projected_map2 new_bck_root;
    GSPAN::EdgeList edges;

    for (unsigned int n = 0; n < projected.size(); ++n) {

      unsigned int id = projected[n].id;
      GSPAN::PDFS *cur = &projected[n];
      GSPAN::History history(TRANS[id], cur);

      for (int i = (int) rmpath.size() - 1; i >= 1; --i) {
        GSPAN::Edge *e = get_backward(TRANS[id], history[rmpath[i]],
                                      history[rmpath[0]], history);
        if (e)
          new_bck_root[DFS_CODE[rmpath[i]].from][e->elabel].push(id, e, cur);
      }

      if (get_forward_pure(TRANS[id], history[rmpath[0]], minlabel, history,
                           edges)) {
        for (GSPAN::EdgeList::iterator it = edges.begin(); it != edges.end();
            ++it) {
          new_fwd_root[maxtoc][(*it)->elabel][TRANS[id][(*it)->to].label].push(
              id, *it, cur);
        }
      }

      for (int i = 0; i < (int) rmpath.size(); ++i) {
        if (get_forward_rmpath(TRANS[id], history[rmpath[i]], minlabel, history,
                               edges)) {
          for (GSPAN::EdgeList::iterator it = edges.begin(); it != edges.end();
              ++it) {
            new_fwd_root[DFS_CODE[rmpath[i]].from][(*it)->elabel][TRANS[id][(*it)
                ->to].label].push(id, *it, cur);
          }
        }
      }
    }

    for (GSPAN::gSpan::Projected_iterator2 to = new_bck_root.begin();
        to != new_bck_root.end(); ++to) {
      for (GSPAN::gSpan::Projected_iterator1 elabel = to->second.begin();
          elabel != to->second.end(); ++elabel) {
        DFS_CODE.push(maxtoc, to->first, -1, elabel->first, -1);
        _Start(elabel->second);
        DFS_CODE.pop();
      }
    }

    for (GSPAN::gSpan::Projected_riterator3 from = new_fwd_root.rbegin();
        from != new_fwd_root.rend(); ++from) {
      for (GSPAN::gSpan::Projected_iterator2 elabel = from->second.begin();
          elabel != from->second.end(); ++elabel) {
        for (GSPAN::gSpan::Projected_iterator1 tolabel = elabel->second.begin();
            tolabel != elabel->second.end(); ++tolabel) {
          DFS_CODE.push(from->first, maxtoc + 1, -1, elabel->first,
                        tolabel->first);
          _Start(tolabel->second);
          DFS_CODE.pop();
        }
      }
    }

    return;
  }

  bool is_min() {
    if (DFS_CODE.size() == 1)
      return (true);

    DFS_CODE.toGraph(GRAPH_IS_MIN);
    DFS_CODE_IS_MIN.clear();

    GSPAN::gSpan::Projected_map3 root;
    GSPAN::EdgeList edges;

    for (unsigned int from = 0; from < GRAPH_IS_MIN.size(); ++from)
      if (get_forward_root(GRAPH_IS_MIN, GRAPH_IS_MIN[from], edges))
        for (GSPAN::EdgeList::iterator it = edges.begin(); it != edges.end();
            ++it)
          root[GRAPH_IS_MIN[from].label][(*it)->elabel][GRAPH_IS_MIN[(*it)->to]
              .label].push(0, *it, 0);

    GSPAN::gSpan::Projected_iterator3 fromlabel = root.begin();
    GSPAN::gSpan::Projected_iterator2 elabel = fromlabel->second.begin();
    GSPAN::gSpan::Projected_iterator1 tolabel = elabel->second.begin();

    DFS_CODE_IS_MIN.push(0, 1, fromlabel->first, elabel->first, tolabel->first);

    return (project_is_min(tolabel->second));
  }

  bool project_is_min(GSPAN::Projected& projected) {
    const GSPAN::RMPath& rmpath = DFS_CODE_IS_MIN.buildRMPath();
    int minlabel = DFS_CODE_IS_MIN[0].fromlabel;
    int maxtoc = DFS_CODE_IS_MIN[rmpath[0]].to;

    {
      GSPAN::gSpan::Projected_map1 root;
      bool flg = false;
      int newto = 0;

      for (int i = rmpath.size() - 1; !flg && i >= 1; --i) {
        for (unsigned int n = 0; n < projected.size(); ++n) {
          GSPAN::PDFS *cur = &projected[n];
          GSPAN::History history(GRAPH_IS_MIN, cur);
          GSPAN::Edge *e = get_backward(GRAPH_IS_MIN, history[rmpath[i]],
                                        history[rmpath[0]], history);
          if (e) {
            root[e->elabel].push(0, e, cur);
            newto = DFS_CODE_IS_MIN[rmpath[i]].from;
            flg = true;
          }
        }
      }

      if (flg) {
        GSPAN::gSpan::Projected_iterator1 elabel = root.begin();
        DFS_CODE_IS_MIN.push(maxtoc, newto, -1, elabel->first, -1);
        if (DFS_CODE[DFS_CODE_IS_MIN.size() - 1]
            != DFS_CODE_IS_MIN[DFS_CODE_IS_MIN.size() - 1])
          return false;
        return project_is_min(elabel->second);
      }
    }

    {
      bool flg = false;
      int newfrom = 0;
      GSPAN::gSpan::Projected_map2 root;
      GSPAN::EdgeList edges;

      for (unsigned int n = 0; n < projected.size(); ++n) {
        GSPAN::PDFS *cur = &projected[n];
        GSPAN::History history(GRAPH_IS_MIN, cur);
        if (get_forward_pure(GRAPH_IS_MIN, history[rmpath[0]], minlabel,
                             history, edges)) {
          flg = true;
          newfrom = maxtoc;
          for (GSPAN::EdgeList::iterator it = edges.begin(); it != edges.end();
              ++it)
            root[(*it)->elabel][GRAPH_IS_MIN[(*it)->to].label].push(0, *it,
                                                                    cur);
        }
      }

      for (int i = 0; !flg && i < (int) rmpath.size(); ++i) {
        for (unsigned int n = 0; n < projected.size(); ++n) {
          GSPAN::PDFS *cur = &projected[n];
          GSPAN::History history(GRAPH_IS_MIN, cur);
          if (get_forward_rmpath(GRAPH_IS_MIN, history[rmpath[i]], minlabel,
                                 history, edges)) {
            flg = true;
            newfrom = DFS_CODE_IS_MIN[rmpath[i]].from;
            for (GSPAN::EdgeList::iterator it = edges.begin();
                it != edges.end(); ++it)
              root[(*it)->elabel][GRAPH_IS_MIN[(*it)->to].label].push(0, *it,
                                                                      cur);
          }
        }
      }

      if (flg) {
        GSPAN::gSpan::Projected_iterator2 elabel = root.begin();
        GSPAN::gSpan::Projected_iterator1 tolabel = elabel->second.begin();
        DFS_CODE_IS_MIN.push(newfrom, maxtoc + 1, -1, elabel->first,
                             tolabel->first);
        if (DFS_CODE[DFS_CODE_IS_MIN.size() - 1]
            != DFS_CODE_IS_MIN[DFS_CODE_IS_MIN.size() - 1])
          return false;
        return project_is_min(tolabel->second);
      }
    }

    return true;
  }
};

#endif
