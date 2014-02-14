#include "subIso.h"

#include <utility>
#include <vector>

#include "../minDFS/GraphToMinDFSCode.h"
#include "../utility/utilityFunction.h"

SubIso::SubIso(GRAPH* _q, GRAPH* _g)
    : q(_q),
      g(_g) {
  /**
   * start label, which is the same to that
   * of the first vertex of the query by default
   */
  start_label = q->getLabel(0);
  res = false;
}

bool SubIso::isVisited(vector<int>& canMatVertex, int dep, int v) {
  // check if v is in path already
  bool isFind = false;
  for (int j = 0; j < dep; j++) {
    if (canMatVertex[j] == v) {
      isFind = true;
      break;
    }
  }
  return isFind;
}

void SubIso::calVertexLabelMap() {
  q->setVertexLabelMap();
  q->setVertexLabelMapCnt();

  cout << "++++++++++ query graph +++++++++++" << endl;
  q->printGraphNew(cout);

  cout << endl;

  g->setVertexLabelMap();
  g->setVertexLabelMapCnt();

  cout << "---------- graph data ------------" << endl;
  g->printGraphNew(cout);
}

void SubIso::genAllCanReg(set<VertexID>& rootVertexSet) {
  GRAPH* cr = new GRAPH();
  GRAPH* cm = new GRAPH();

  q->initSubIso();
  cm->initEqvCls(q->V());

  // for each starting vertex
  for (set<VertexID>::iterator it = rootVertexSet.begin();
      it != rootVertexSet.end(); it++) {

    VertexID r_vertex = *it;

    // gen Cand. Reg.
    if (!genCanReg(r_vertex, cr)) {
      continue;
    }

    // find Cand. Match
    genAllCanMatch(r_vertex, cr, cm);

    // reset Cand. Reg.
    cr->makeEmpty();
  }

  q->clearSubIso();
  cm->clearEqvCls();

  delete cr;
  delete cm;
}

bool SubIso::isMapped(VertexLabelMapCnt& vlabels_map_cnt) {
  for (VertexLabelMapCnt::iterator it = vlabels_map_cnt.begin();
      it != vlabels_map_cnt.end(); it++) {
//    cout << "it->" << it->second << endl;
    if (it->second > 0) {
      return false;
    }
  }
  return true;
}

bool SubIso::genCanReg(VertexID r_vertex, GRAPH* cr) {
  // BFS with q->Vcnt hops
  cout << "root vertex: " << endl;
  cout << r_vertex << endl;

  // BFS to gen can. reg.
  set<VertexID> visit_v;
  q->setVertexLabelMapCnt();
  g->BFSwithConst(r_vertex, q->V(), visit_v, q->vlabels_map_cnt);

  cout << "print visit set: " << endl;
  printSet(visit_v);

  // judge the situation of map cnt of q
  if (!isMapped(q->vlabels_map_cnt)) {
    cout << "genCanReg returns false" << endl;
    return false;
  }

  // generate Can. Reg. by induced subgraph of g
  // already set the vertex label map
  g->getInducedSubGraph(visit_v, cr);

  cout << endl << "************** Candidate Region: ***************" << endl;
  cr->printGraphNew(cout);

  return true;
}

void SubIso::genAllCanMatch(VertexID r_vertex, GRAPH* cr, GRAPH* cm) {
  vector<VertexID> canMatVertex;
  canMatVertex.resize(q->V());

  // generate all Can. Match by DFS-like traversal on cr
  // (1). q.V() vertexes
  // (2). Induced from cr

  q->setVertexLabelMapCnt();
  canMatVertex[0] = r_vertex;  // set
  q->vlabels_map_cnt[cr->getLabel(r_vertex)]--;  // set

  genCanMatch(0 + 1, cr, canMatVertex, cm);
}

void SubIso::cacheAllSubOf(GSPAN::DFSCode& dfs_code) {
  SubGraphGen* sg = new SubGraphGen();

  // generate all subgraphs of dfs_code
  sg->Start(dfs_code);

  for (int i = 0; i < sg->Size(); i++) {
    if (sg->Subgraphs[i].nodeCount() == q->V()) {
      // cache the hash codes of subgraphs with size q->V()
      cache.ifHasCm.insert(sg->Subgraphs[i].hashCode());
    }
  }

  delete sg;
}

bool SubIso::isCanMatChecked(GRAPH* cm) {
  GraphToMinDFSCode mindfs;

  GSPAN::DFSCode dfs_code;
  mindfs.Init(cm);

  mindfs.ConvertGRAPH(dfs_code);

  cout << "============= hash code ============" << endl;
  cout << dfs_code.hashCode() << endl;

  if (cache.ifHasCm.find(dfs_code.hashCode()) == cache.ifHasCm.end()) {

    cache.ifHasCm.insert(dfs_code.hashCode());

    // we further cache all the size-V() subgraph of cm into ifHasCm
    cacheAllSubOf(dfs_code);

    return true;
  } else {
    return false;
  }
}

void SubIso::genCanMatch(int dep, GRAPH* cr, vector<VertexID>& canMatVertex,
                         GRAPH* cm) {

  // generate set canonical label
  string str;
  vectorToString(canMatVertex, dep, str);

//  cout << "canonical string: " << str << endl;

  // has already check this combination
  if (cache.ifHasString.find(str) != cache.ifHasString.end()) {
    return;
  }

  cache.ifHasString.insert(str);

  if (dep == q->V()) {
    // condition
    // judge if all labels are covered
    if (!isMapped(q->vlabels_map_cnt)) {
      cout << "genCanMatch returns false" << endl;
      return;
    }

    // generate cm
    cr->getInducedSubGraph(canMatVertex, cm);

    cout << "============= can match vertex: =============" << endl;
    printVector(canMatVertex);

    cout << "------------- can match: -------------" << endl;
    cm->printGraphNew(cout);

    // if cm is judged before by minDFSCode
    if (!isCanMatChecked(cm)) {
      cout << "!!!!!!!!!!!! cm is checked !!!!!!!!!!! " << endl;
      cm->makeEmpty();
      return;
    }

    // generate equivalent class of cm
    cm->resetEqvCls();
    cm->genEqvCls();

    cout << "------------- Eqv Cls --------------" << endl;
    printVectorSet(cm->eqv_cls);

    // do matching
    doMatch(canMatVertex, cm);

    // reset cm for next match
    cm->makeEmpty();
  }

  // for each vertex u in canMatVertex
  for (int i = 0; i < dep; i++) {
    VertexID u = canMatVertex[i];

    // for each neighbor vertex v of u
    for (int j = 0; j < cr->getDegree(u); j++) {
      VertexID v = cr->_adjList[u][j].v;
      VertexLabel v_l = cr->getLabel(v);

      // v is visited
      if (isVisited(canMatVertex, dep, v)) {
        continue;
      }

      // no quota for the label of v
      if (q->vlabels_map_cnt[v_l] <= 0) {
        continue;
      }

      // select v
      canMatVertex[dep] = v;
      q->vlabels_map_cnt[v_l]--;
      genCanMatch(dep + 1, cr, canMatVertex, cm);
      q->vlabels_map_cnt[v_l]++;
    }
  }
}

void SubIso::doMatch(vector<VertexID>& canMatVertex, GRAPH* cm) {
  // reset subIso: initialize M, col, row ...
  q->resetSubIso(cm);

  // match (q, cm)
  q->isSubgraphOf(cm);
}

void SubIso::doSubIso() {
  if (g->vlabels_map_cnt.find(start_label) == g->vlabels_map_cnt.end()) {
    cout << "no Can. Reg." << endl;
    res = false;
    return;
  }

  genAllCanReg(g->vlabels_map[start_label]);
}

bool SubIso::isSubIso() {
  // calculate vertex label mapping
  // TODO
  // may not need all maps
  // only need start vertex
  calVertexLabelMap();

  doSubIso();

  return res;
}
