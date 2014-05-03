#include "subIso.h"

#include <sys/types.h>
#include <ctime>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <algorithm>
#include <tr1/unordered_map>

#include "../gSpan/gspan.h"
#include "../minDFS/GraphToMinDFSCode.h"
#include "../utility/graph.h"
#include "../utility/utilityFunction.h"

using namespace std::tr1;

SubIso::SubIso(GRAPH* _q, GRAPH* _g)
    : q(_q),
      g(_g) {
  // set l_s and h
  start_label = q->start_label;
  tree_height = q->min_tree_height - 1;

  ASSERT(start_label > NO_LABEL);
  ASSERT(tree_height > NO_VERTEX);

//  cout << "tree_height: " << tree_height << endl;

  response = false;
  myStat = new STAT();
}

SubIso::~SubIso() {
  delete myStat;
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

void SubIso::genAllCanReg(vector<VertexWDeg>& rootVertex) {
  GRAPH* cr = new GRAPH();
  GRAPH* cm = new GRAPH();

  clock_t _s, _e;

  q->initSubIso();
  cm->initEqvCls(q->V());

//  cout << "|CR| = " << rootVertexSet.size() << endl;
  // for each starting vertex
  for (int i = 0; i < rootVertex.size(); i++) {

    VertexID r_vertex = rootVertex[i].u;
//    cout << endl << "original root: " << r_vertex << " " << g->getDegree(r_vertex) << endl;

    // cannot use this ifRootVertex cache
//    cache.ifRootVertex.insert(r_vertex);

    // generate cr
    _s = clock();
    if (!genCanReg(r_vertex, cr)) {
      cr->makeEmpty();
      _e = clock();
      myStat->cr_time += gettime(_s, _e);
      continue;
    }
    _e = clock();
    myStat->cr_time += gettime(_s, _e);

//    cout << "go" << endl;

    // find cm
    _s = clock();
    genAllCanMatch(r_vertex, cr, cm);
    _e = clock();
    myStat->cm_time += gettime(_s, _e);

    // TODO try a new DFS by the set of labels.

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

  // BFS to gen can. reg.
  set<VertexID> visit_v;
  q->setVertexLabelMapCnt();
  // if the # of label of r_vertex in q is 1,
  // no need to introduce other vertex with the same label
  double _s = clock();
  g->BFSwithConst(r_vertex, tree_height, visit_v, q->vlabels_map_cnt, cache);
  double _e = clock();
  myStat->cr_bfs_time += gettime(_s, _e);

//  cout << "|cr_i|: " << visit_v.size() << endl;
//  printSet(visit_v);

// judge the situation of map cnt of q
  if (!isMapped(q->vlabels_map_cnt)) {
//    cout << "genCanReg returns false" << endl;
    return false;
  }

  // generate cr by induced subgraph of g
  // (1) already set the vertex label map
  // (2) need to re-set the r_vertex
  _s = clock();
  g->getInducedSubGraph(visit_v, cr, r_vertex);
  _e = clock();
  myStat->cr_cont_time += gettime(_s, _e);
//  cout << "..." << gettime(_s, _e) << endl;

  myStat->cr_cnt++;

//  cout << "root vertex: " << r_vertex << endl;
//  cout << "************** Candidate Region: ***************" << endl;
//  cr->printGraphNew(cout);
//  cr->printGraphPartial(cout);

  int cr1_v = cr->VnI();
  int cr1_e = cr->E();
  myStat->org_cr_v += cr1_v;
  myStat->org_cr_e += cr1_e;
//  cout << "1. |V(cr_i)|: " << cr->VnI() << " |E(cr_i)|: " << cr->E() << endl;
  // generate eqv_cls for cr
  // deduce the edges of cr by eqv_cls
  _s = clock();
  canRegEqvCls(cr, r_vertex);
  _e = clock();
  myStat->nec_time += gettime(_s, _e);
//  cout << " ~~~~~~~~~~ before:" << r_vertex << "~~~~~~~~~~~~~~" << endl;
//  cr->printGraphNew(cout);
  int cr2_v = cr->VnI();
  int cr2_e = cr->E();
//  cout << "2. |V(cr_i)|: " << cr->VnI() << " |E(cr_i)|: " << cr->E() << endl;
  _s = clock();
  canRegReduce(cr, r_vertex);
  _e = clock();
  myStat->nc_time += gettime(_s, _e);

//  cout << " ========== after:" << r_vertex << " ===============" << endl;
//  cr->printGraphNew(cout);

  int cr3_v = cr->VnI();
  int cr3_e = cr->E();
  myStat->red_cr_v += cr3_v;
  myStat->red_cr_e += cr3_e;
//  cout << "3. |V(cr_i)|: " << cr->VnI() << " |E(cr_i)|: " << cr->E() << endl;
//  printHashTableTT(cr->vlabels_map_cnt);

  myStat->nec_effect_v += cr1_v - cr2_v;
  myStat->nec_effect_e += cr1_e - cr2_e;
  myStat->nc_effect_v += cr2_v - cr3_v;
  myStat->nc_effect_e += cr2_e - cr3_e;

  _s = clock();
  if (!predictCR(cr, 100000)) {
//    cout << "---------skipped!-------" << endl;
    myStat->cr_cnt_predict++;

    _e = clock();
    myStat->cr_predict_time = gettime(_s, _e);
    return false;
  }

  _e = clock();
  myStat->cr_predict_time = gettime(_s, _e);

  return true;
}

bool SubIso::predictCR(GRAPH* cr, long long limit) {
  cr->setVertexLabelMapCnt();
  q->setVertexLabelMapCnt();

  long long cnt = sumUpVertexLabelCnt(q->vlabels_map_cnt, cr->vlabels_map_cnt);
//  cout << cnt << endl;

//  cout << "predict: " << cnt << endl;
//  cout << "detailed:" << endl;
//  printHashTableTT(cr->vlabels_map_cnt);
//  cout << endl;
  if (cnt > limit) {
    return false;
  } else {
    return true;
  }
}

void SubIso::canRegReduce(GRAPH* cr, VertexID& r_vertex) {
  // TODO further reduce cr.

  q->setVertexLabelMapCnt();

  cr->setVertexLabelMap();
  cr->setVertexLabelMapCnt();

  // for each label
  for (VertexLabelMapCnt::iterator it = cr->vlabels_map_cnt.begin();
      it != cr->vlabels_map_cnt.end(); it++) {
    VertexLabel _l = it->first;

    // test to reduce single label, so far
    if (q->vlabels_map_cnt[_l] != 1) {
      continue;
    }
    // equals to 1

//    cr->setVertexLabelMap();
    set<VertexID>& _set = cr->vlabels_map[_l];
    for (set<VertexID>::iterator it1 = _set.begin(); it1 != _set.end(); it1++) {
      VertexID u = *it1;

      if (u == r_vertex)
        continue;

      set<VertexID>::iterator it2 = it1;
      for (it2++; it2 != _set.end(); it2++) {
        VertexID v = *it2;

        if (v == r_vertex)
          continue;

        // if u (v) dominate v (u)
        int _inter = 0;
        int ind_u = 0;
        int ind_v = 0;
        int diff_l_u_cnt = 0;
        int diff_l_v_cnt = 0;
        while (ind_u < cr->getDegree(u) && ind_v < cr->getDegree(v)) {
          VertexID nu = cr->_adjList[u][ind_u].v;
          VertexID nv = cr->_adjList[v][ind_v].v;
          if (cr->getLabel(nu) == _l) {
            ind_u++;
            diff_l_u_cnt++;
            continue;
          }
          if (cr->getLabel(nv) == _l) {
            ind_v++;
            diff_l_v_cnt++;
            continue;
          }

          if (nu < nv) {
            ind_u++;
          } else if (nu > nv) {
            ind_v++;
          } else {
            _inter++;
            ind_u++;
            ind_v++;
          }
        }

        if (_inter == (cr->getDegree(u) - diff_l_u_cnt)) {
          // v dominate u
          cr->removeVexAllEdges(u);
          break;
        }

        if (_inter == (cr->getDegree(v) - diff_l_v_cnt)) {
          // u dominate v
          cr->removeVexAllEdges(v);
          continue;
        }
      }
    }
  }
}

void SubIso::canRegEqvCls(GRAPH* cr, VertexID& r_vertex) {
// equ_cls for cr.
// to reduce |cr|.

// initialize
  cr->initEqvCls(cr->V());

  cr->genEqvCls();
  q->setVertexLabelMapCnt();
//  cout << "+ before: " << cr->E() << endl;
  cr->reduceByEqvCls(r_vertex, q->vlabels_map_cnt);
//  cout << "+ after: " << cr->E() << endl;
//  printVectorSet(cr->eqv_cls);

  /*
   while (cr->eqv_cls_flg == true) {
   // re-set the paras
   cr->resetEqvCls();

   cr->genEqvCls();
   q->setVertexLabelMapCnt();
   //    cout << "q: " << q->vlabels_map_cnt[11] << endl;
   //    cout << "r before: " << cr->E() << endl;
   cr->reduceByEqvCls(r_vertex, q->vlabels_map_cnt);
   //    cout << "r after: " << cr->E() << endl;
   //  printVectorSet(cr->eqv_cls);
   }
   */

// clear
  cr->clearEqvCls();
}

void SubIso::genAllCanMatch(VertexID r_vertex, GRAPH* cr, GRAPH* cm) {
  vector<VertexID> canMatVertex;
  canMatVertex.resize(q->V());
  cache.ifHasString.clear();

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
  if (q->V() > DEFAULTCACHESIZE) {
    return true;
  }

  clock_t _s, _s1, _e, _e1;
  _s = clock();
  GraphToMinDFSCode mindfs;

  GSPAN::DFSCode dfs_code;
  mindfs.Init(cm);

  mindfs.ConvertGRAPH(dfs_code);

//  cout << "============= hash code ============" << endl;
//  cout << dfs_code.hashCode() << endl;
  _e = clock();
  myStat->canon_cm_time += gettime(_s, _e);

  if (cache.ifHasCm.find(dfs_code.hashCode()) == cache.ifHasCm.end()) {

    cache.ifHasCm.insert(dfs_code.hashCode());

    // we further cache all the size-V() subgraph of cm into ifHasCm
    _s1 = clock();
    // TODO make sure.
//    cacheAllSubOf(dfs_code);
    _e1 = clock();
    myStat->decomp_cm_time += gettime(_s1, _e1);

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

  clock_t _s, _e;

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

    myStat->cm_cnt++;
    // if cm is judged before by minDFSCode
    if (!isCanMatChecked(cm)) {
//      cout << "!!!!!!!!!!!! cm is checked !!!!!!!!!!! " << endl;
      cm->makeEmpty();
      myStat->cm_cnt_prune++;
//      cout << "-" << myStat->cm_cnt_prune << endl;
      return;
    }

    // generate equivalent class of cm
    cm->resetEqvCls();
    cm->genEqvCls();

//    cout << "------------- can match: -------------" << endl;
//    cout << "|V(cm)|: " << cm->V() << " |E(cm)|: " << cm->E() << endl;
//    cm->printGraphNew(cout);
//    cout << "------------- Eqv Cls --------------" << endl;
//    printVectorSet(cm->eqv_cls);

    // do matching
    _s = clock();
    doMatch(cm);
    _e = clock();
    myStat->match_time += gettime(_s, _e);

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
  }
}

void SubIso::sortRootVertex(set<VertexID>& rootVertexOrig,
                            vector<VertexWDeg>& rootVertexNew) {
  // initialize the vector
  int i = 0;
  for (set<VertexID>::iterator it = rootVertexOrig.begin();
      it != rootVertexOrig.end(); it++) {
    rootVertexNew[i].u = *it;
    rootVertexNew[i].deg = g->getDegree(*it);
    i++;
  }

  sort(rootVertexNew.begin(), rootVertexNew.end());
}

void SubIso::doSubIso() {
  if (g->vlabels_map_cnt.find(start_label) == g->vlabels_map_cnt.end()) {
//    cout << "no Can. Reg." << endl;
    return;
  }

  // TODO sort the root vertex
  vector<VertexWDeg> rootVertex;
  rootVertex.resize(g->vlabels_map[start_label].size());

  sortRootVertex(g->vlabels_map[start_label], rootVertex);

  genAllCanReg(rootVertex);
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
