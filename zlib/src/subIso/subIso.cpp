#include "subIso.h"

#include <utility>
#include <vector>

#include "../minDFS/GraphToMinDFSCode.h"
#include "../utility/utilityFunction.h"

SubIso::SubIso(GRAPH* _q, GRAPH* _g)
    : q(_q),
      g(_g) {
  /**
   * TODO: use BFS on q to determine the
   * start label, which is the same to that
   * of the first vertex of the query by default
   */
  response = false;
  start_label = q->getLabel(q->getMinTreeHeight());
  tree_height = q->min_tree_height - 1;
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

void SubIso::clearVertexLabelMap() {
  q->vlabels_map.clear();
  q->vlabels_map_cnt.clear();

  g->vlabels_map.clear();
  g->vlabels_map_cnt.clear();
}

void SubIso::calVertexLabelMap() {
  q->setVertexLabelMap();
  q->setVertexLabelMapCnt();

  g->setVertexLabelMap();
  g->setVertexLabelMapCnt();
}

void SubIso::genAllCanReg(set<VertexID>& rootVertexSet) {
  GRAPH* cr = new GRAPH();
  GRAPH* cm = new GRAPH();

  q->initSubIso();
  cm->initEqvCls(q->V());
  cache.cnt_cm = cache.cnt_cr = cache.cnt_cm_p = 0;

  cout << "|CR| = " << rootVertexSet.size() << endl;
  // for each starting vertex
  for (set<VertexID>::iterator it = rootVertexSet.begin();
      it != rootVertexSet.end(); it++) {

    VertexID r_vertex = *it;
//    cout << endl << "original root: " << r_vertex << endl;

    // TODO: cannot use this ifRootVertex cache
//    cache.ifRootVertex.insert(r_vertex);

    // generate cr
    if (!genCanReg(r_vertex, cr)) {
      continue;
    }

    // find cm
    genAllCanMatch(r_vertex, cr, cm);

    if (response) {
      break;
    }

    // reset cr, we do not set cr's eqv. cls.
    cr->makeEmpty();
  }

  q->clearSubIso();
  cm->clearEqvCls();
  cm->makeEmpty();

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

bool SubIso::genCanReg(VertexID& r_vertex, GRAPH* cr) {
//  cout << "generate cr" << endl;
//  cout << "root vertex: " << r_vertex << endl;

// BFS to gen can. reg.
  set<VertexID> visit_v;
  q->setVertexLabelMapCnt();
  g->BFSwithConst(r_vertex, tree_height, visit_v, q->vlabels_map_cnt, cache);

  cout << "|cr_i|: " << visit_v.size() << endl;
//  printSet(visit_v);

// judge the situation of map cnt of q
  if (!isMapped(q->vlabels_map_cnt)) {
//    cout << "genCanReg returns false" << endl;
    return false;
  }

  // generate Can. Reg. by induced subgraph of g
  // (1) already set the vertex label map
  // (2) need to re-set the r_vertex
  g->getInducedSubGraph(visit_v, cr, r_vertex);

//  cout << "root vertex: " << r_vertex << endl;
//  cout << "************** Candidate Region: ***************" << endl;
//  cr->printGraphNew(cout);
//  cr->printGraphPartial(cout);

  // TODO equ_cls for cr.
  // to reduce |cr|.
  cr->initEqvCls(cr->V());
  cr->genEqvCls();
  q->setVertexLabelMapCnt();

  cout << "before: " << cr->E() << endl;
  cr->reduceByEqvCls(r_vertex, q->vlabels_map_cnt);
  cout << "after: " << cr->E() << endl;

//  printVectorSet(cr->eqv_cls);
  cr->clearEqvCls();

  return true;
}

void SubIso::genAllCanMatch(VertexID r_vertex, GRAPH* cr, GRAPH* cm) {
//  cout << "generate all cm" << endl;
  vector<VertexID> canMatVertex;
  canMatVertex.resize(q->V());
  cache.ifHasString.clear();

  // generate all Can. Match by DFS-like traversal on cr
  // (1). q.V() vertexes
  // (2). Induced from cr
  q->setVertexLabelMapCnt();
  canMatVertex[0] = r_vertex;  // set
  q->vlabels_map_cnt[cr->getLabel(r_vertex)]--;  // set

//  cout << "start generate cm" << endl;
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

//  cout << "============= hash code ============" << endl;
//  cout << dfs_code.hashCode() << endl;

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
  if (response)
    return;

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
//    cout << "============= can match vertex: =============" << endl;
//    printVector(canMatVertex);

    // generate cm
    // It is okay to use getInducedSubgraph()
    // as the size of canMatVertex is small, e.g., 10
    cr->getInducedSubGraph(canMatVertex, cm);

    // if cm is judged before by minDFSCode
    if (!isCanMatChecked(cm)) {
//      cout << "!!!!!!!!!!!! cm is checked !!!!!!!!!!! " << endl;
      cm->makeEmpty();
//      cout << "-" << cache.cnt_cm_p++ << endl;
      return;
    }

//    cout << "+" << cache.cnt_cm++ << endl;
    // generate equivalent class of cm
    cm->resetEqvCls();
    cm->genEqvCls();

//    cout << "------------- can match: -------------" << endl;
//    cm->printGraphNew(cout);
//    cout << "------------- Eqv Cls --------------" << endl;
//    printVectorSet(cm->eqv_cls);

    // do matching
    doMatch(cm);

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

void SubIso::doMatch(GRAPH* cm) {
  // reset subIso: initialize M, col, row ...
  q->resetSubIso(cm);

  // match (q, cm)
  if (q->isSubgraphOf(cm)) {
    // sub iso
    response = true;
//    cm->printGraphNew(cout);
  }
}

void SubIso::doSubIso() {
  if (g->vlabels_map_cnt.find(start_label) == g->vlabels_map_cnt.end()) {
//    cout << "no Can. Reg." << endl;
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

  clearVertexLabelMap();

  return response;
}
