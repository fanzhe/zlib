#include "GraphSimCal.h"
#include <list>

GraphSimCal::GraphSimCal() {

}

bool GraphSimCal::SimCal(DIGRAPHBASIC * datagraph, DIKEYS * querygraph,
                         MapIntHset & simset) {
  MapIntHset remove;
  bool flag;
  flag = graphSimMain(datagraph, querygraph, simset, remove);
  remove.clear();

  return flag;
}

bool GraphSimCal::graphSimMain(DIGRAPHBASIC * dg, DIKEYS * qg,
                               MapIntHset & simset, MapIntHset & remove) {
  bool flag1 = graphInitialization(dg, qg, simset, remove);
  if (flag1 == false) {
    return false;
  }
  bool flag2 = graphSimRefinement(dg, qg, simset, remove);
  if (flag2 == false) {
    return false;
  }

  return true;
}

bool GraphSimCal::graphInitialization(DIGRAPHBASIC * dg, DIKEYS * qg,
                                      MapIntHset & simset,
                                      MapIntHset & remove) {
  unordered_set<int> pre_dgraph_vertices;
  unordered_set<int> pre_dgraph_vertices_temp;
  unordered_set<int> hssim;
  unordered_set<int> pre_sim_vertices;

  //***********pre(V)*****************
  // for each node of G
  typename DIGRAPHBASIC::VLabels::iterator iter1 = dg->getVLabel().begin();
  int vb = -1;
  while (iter1 != dg->getVLabel().end()) {
    vb = iter1->first;
    if (dg->getOutDegree(vb) > 0) {
      pre_dgraph_vertices.insert(vb);
    }
    iter1++;
  }

  //
  typename DIKEYS::VLabels::iterator iter2;
  int vq = -1;
  // for each node of Q
  for (iter2 = qg->getVLabel().begin(); iter2 != qg->getVLabel().end();
      iter2++) {
    //***********sim(v)******************
    vq = iter2->first;
    if (qg->getOutDegree(vq) == 0) {
      iter1 = dg->getVLabel().begin();
      vb = -1;
      while (iter1 != dg->getVLabel().end()) {
        vb = iter1->first;
        // TODO: check feasibility
        if (checkNodeFeasibility(vb, vq)) {
          hssim.insert(vb);
        }
        iter1++;
      }
    } else {
      iter1 = dg->getVLabel().begin();
      vb = -1;
      while (iter1 != dg->getVLabel().end()) {
        vb = iter1->first;
        if (checkNodeFeasibility(vb, vq) && dg->getOutDegree(vb) > 0) {
          hssim.insert(vb);
        }
        iter1++;
      }
    }
    if (hssim.size() < 1) {
      return false;
    }
    simset.insert(PairIntHset(vq, hssim));
    //************pre(sim(v))***************
    unordered_set<int>::iterator iter3;
    int vn = -1;
    for (iter3 = hssim.begin(); iter3 != hssim.end(); iter3++) {
      vn = *iter3;

      for (typename DIGRAPHBASIC::AdjList::iterator iter1 = dg->getOutEdge()[vn]
          .begin(); iter1 != dg->getOutEdge()[vn].end(); iter1++) {
        pre_sim_vertices.insert(iter1->first);
      }

    }
    //*****remove(v):=pre(V)/pre(sim(v))******
    for (iter3 = pre_dgraph_vertices.begin();
        iter3 != pre_dgraph_vertices.end(); iter3++) {
      pre_dgraph_vertices_temp.insert(*iter3);
    }
    for (iter3 = pre_sim_vertices.begin(); iter3 != pre_sim_vertices.end();
        iter3++) {
      pre_dgraph_vertices_temp.erase(*iter3);
    }
    remove.insert(PairIntHset(vq, pre_dgraph_vertices_temp));

    pre_dgraph_vertices_temp.clear();
    pre_sim_vertices.clear();
    hssim.clear();
  }  // end of for q

  return true;
}

// TODO: here
bool GraphSimCal::graphSimRefinement(DIGRAPHBASIC *dg, DIKEYS *qg,
                                     MapIntHset & simset, MapIntHset & remove) {
  MapIntInt r_m;
  vector<vector<int> > counter(dg.getGraphNodeNum(),
                               vector<int>(qg.getGraphNodeNum()));
  //int *p =  new int [b.getGraphNodeNum() * COUNTER_COL];//TEST!!!!!!!!!!!!!!!
  //int (*counter)[COUNTER_COL] = (int(*)[COUNTER_COL])p;

  counterInitilization(counter, simset, dg, qg);
  //Refinement
  int u = findNoEmptyRemove(remove, qg);
  while (u != -2) {
    int u_n = -1;
    vector<int>::iterator iter_nb;
    ////////////////////////////////////
    for (iter_nb = qg.vertices[u].nbset.begin();
        iter_nb != qg.vertices[u].nbset.end(); iter_nb++) {
      u_n = *iter_nb;
      int w = -1;
      hash_set<int>::iterator iter_remove;
      for (iter_remove = remove.find(u)->second.begin();
          iter_remove != remove.find(u)->second.end(); iter_remove++) {
        w = *iter_remove;
        hash_set<int>::iterator iter_sim_hs = simset.find(u_n)->second.find(w);
        if (iter_sim_hs != simset.find(u_n)->second.end()) {
          simset.find(u_n)->second.erase(iter_sim_hs);
          if (simset.find(u_n)->second.size() < 1) {
            return false;
          }
          //Update Counter
          int updc = -1;
          vector<int>::iterator iter_dpdc;
          for (iter_dpdc = dg.vertices[w].nbset.begin();
              iter_dpdc != dg.vertices[w].nbset.end(); iter_dpdc++) {
            updc = *iter_dpdc;
            if (counter[updc][u_n] > 0) {
              counter[updc][u_n]--;
            }
          }
          //go on
          int w_n = -1;
          vector<int>::iterator iter_wb;
          for (iter_wb = dg.vertices[w].nbset.begin();
              iter_wb != dg.vertices[w].nbset.end(); iter_wb++) {
            w_n = *iter_wb;
            if (counter[w_n][u_n] == 0) {
              remove.find(u_n)->second.insert(w_n);
            }
          }
        }
      }
    }
    remove.find(u)->second.clear();
    u = findNoEmptyRemove(remove, qg);
  }  // end of while

  counter.clear();
  return true;
}

int GraphSimCal::findNoEmptyRemove(MapIntHset & remove, QGraph & qg) {
  vector<QVertex>::iterator iter = qg.vertices.begin();
  int vq = -1;
  while (iter != qg.vertices.end()) {
    vq = iter->id;
    iter++;
    if (remove.find(vq)->second.size() != 0) {
      return vq;
    }
  }
  return -2;
}

int GraphSimCal::myIntersection(vector<int> & vec1, hash_set<int> & hs2) {
  set<int> t;
  hash_set<int> hs1;
  vector<int>::iterator iter1;
  hash_set<int>::iterator iter2;
  hash_set<int>::iterator iter3;
  set<int>::iterator iter4;

  for (iter1 = vec1.begin(); iter1 != vec1.end(); iter1++) {
    iter2 = hs2.find(*iter1);
    if (iter2 != hs2.end()) {
      t.insert(*iter1);
    }
    hs1.insert(*iter1);
  }

  for (iter2 = hs2.begin(); iter2 != hs2.end(); iter2++) {
    iter3 = hs1.find(*iter2);
    if (iter3 != hs1.end()) {
      t.insert(*iter2);
    }
  }

  hs1.clear();
  return t.size();
}

void GraphSimCal::counterInitilization(vector<vector<int> > & counter,
                                       MapIntHset & simset, Graph & dg,
                                       QGraph & qg) {
  vector<Vertex>::iterator iter1 = dg.vertices.begin();
  int vb = -1;
  while (iter1 != dg.vertices.end()) {
    vb = iter1->id;
    int vq = -1;
    vector<QVertex>::iterator iter2 = qg.vertices.begin();
    while (iter2 != qg.vertices.end()) {
      vq = iter2->id;
      int s = myIntersection(dg.vertices[vb].nbset, simset.find(vq)->second);
      counter[vb][vq] = s;
      iter2++;
    }
    iter1++;
  }
}
