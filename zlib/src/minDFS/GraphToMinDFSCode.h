#ifndef GRAPHTOMINDFSCODE_H
#define GRAPHTOMINDFSCODE_H

#include "SubGraphGen.h"
#include "Trans_GSPANGraph_GRAPH.h"

#include "../utility/GlobalDefinition.h"

class GraphToMinDFSCode : public SubGraphGen {
 public:
  GraphToMinDFSCode(void) {
  }
  ~GraphToMinDFSCode(void) {
  }

  Status Init(GRAPH* graph) {
    m_Graph = graph;

    m_IsFound = false;

    TRANS.clear();
    DFS_CODE.clear();
    DFS_CODE_IS_MIN.clear();
    GRAPH_IS_MIN.clear();
    m_MinDFSCode.clear();

    return OK;
  }

  Status ConvertGRAPH(GSPAN::DFSCode& dfs_code) {
    GSPAN::Graph super_graph;
    Convert(*m_Graph, super_graph);
    TRANS.push_back(super_graph);
    graph_size = m_Graph->E();

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

    dfs_code = m_MinDFSCode;
    return OK;
  }
 protected:
  void _Start(GSPAN::Projected& projected) {
    if (m_IsFound)
      return;

    if (!is_min())
      return;

    if (DFS_CODE.size() == graph_size) {
      m_IsFound = true;
      m_MinDFSCode = DFS_CODE;
      return;
    }

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

 protected:
  GRAPH* m_Graph;
  GSPAN::DFSCode m_MinDFSCode;
  bool m_IsFound;
};

#endif
