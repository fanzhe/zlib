/*
 * EL.h
 *
 *  Created on: 2014¦~8¤ë2¤é
 *      Author: zfan
 */

#ifndef EL_H_
#define EL_H_

#include "../utility/DiGraph.h"
#include "../utility/BoolEqn.h"
#include "../utility/BoolVar.h"
#include <stack>
#include <vector>

using namespace std;

/**
 * TODO:
 * 1. We need to treat *data value* and *variable* separately
 * 2.
 */
class EL {
 public:
  DIGRAPHBASIC* dg;
  DIGRAPHBASIC* dq;
  VertexID e1, e2;

  MapVerPair MVP;

  int Mcnt;

  MapPairHash AllXPairHash;
  MapListPair LXPair;
  BoolVar terminate;

  EL(DIGRAPHBASIC* _dq, VertexID u, DIGRAPHBASIC* _dg, VertexID _e1,
     VertexID _e2) {
    dg = _dg;
    dq = _dq;

    e1 = _e1;
    e2 = _e2;
    // initialize size
    dq->initEL(u);
    dg->initEL(e1);
  }

  /**
   * Main
   * the algorithm conceptually works as follows:
   * 1. search Cq1 and Cq2
   * 2. refine Cq1 and Cq2
   * 3. produce dGq by Cq1 and Cq2
   * 4. minimize dGq
   * 5. check mapping
   * 6. get results (T/F or a set of vertex pairs)
   */
  void run() {
    // initialize flag
    terminate.value = false;
    terminate.determined = false;

    // check if (dg, dq) |= (e1, e2)
    _run();

    // finish
    cout << "finish!" << endl;
  }

  bool cmpUVInEdges(VertexID u, VertexID v, VertexID vf) {
    bool flag = false;
    if (vf == 1) {
      for (typename DIGRAPHBASIC::AdjListBool::iterator itq =
          dq->getInVertex()[u].begin(); itq != dq->getInVertex()[u].end();
          itq++) {
        VertexID up = itq->first;
        if (MVP[up] == Pair(NO_VERTEX, NO_VERTEX))
          continue;

        VertexID vp1 = MVP[up].u;
        // if (u, up) => (v, vp)
        if (!dg->isEdge(vp1, v)) {
          flag = true;
          break;
        }
      }
    } else if (vf == 2) {
      for (typename DIGRAPHBASIC::AdjListBool::iterator itq =
          dq->getInVertex()[u].begin(); itq != dq->getInVertex()[u].end();
          itq++) {
        VertexID up = itq->first;
        if (MVP[up] == Pair(NO_VERTEX, NO_VERTEX))
          continue;

        VertexID vp2 = MVP[up].v;
        // if (u, up) => (v, vp)
        if (!dg->isEdge(vp2, v)) {
          flag = true;
          break;
        }
      }
    } else {
      // error
      exit(0);
    }
    return flag;
  }

  bool cmpUVOutEdges(VertexID u, VertexID v, VertexID vf) {
    bool flag = false;
    if (vf == 1) {
      for (typename DIGRAPHBASIC::AdjList::iterator itq = dq->getOutEdge()[u]
          .begin(); itq != dq->getOutEdge()[u].end(); itq++) {
        VertexID up = itq->first;
        if (MVP[up] == Pair(NO_VERTEX, NO_VERTEX))
          continue;

        VertexID vp1 = MVP[up].u;
        // if (u, up) => (v, vp)
        if (!dg->isEdge(v, vp1)) {
          flag = true;
          break;
        }
      }
    } else if (vf == 2) {
      for (typename DIGRAPHBASIC::AdjList::iterator itq = dq->getOutEdge()[u]
          .begin(); itq != dq->getOutEdge()[u].end(); itq++) {
        VertexID up = itq->first;
        if (MVP[up] == Pair(NO_VERTEX, NO_VERTEX))
          continue;

        VertexID vp2 = MVP[up].v;
        // if (u, up) => (v, vp)
        if (!dg->isEdge(v, vp2)) {
          flag = true;
          break;
        }
      }
    } else {
      // error
      exit(0);
    }
    return flag;
  }

  /**
   * TODO:
   * 1. directly generate dependency without search step for e1 and e2.
   */
  void _run() {
    cout << "start newRun" << endl;

    // initialize the first mapping
    Mcnt = 0;

    dg->setVertexVisited();
    VertexID u = dq->e;

    // initialize mapping
    for (typename DIGRAPHBASIC::VLabels::iterator it = dq->getVLabel().begin();
        it != dq->getVLabel().end(); it++) {
      MVP[it->first] = Pair(NO_VERTEX, NO_VERTEX);
    }
    //
    MVP[u].u = e1;
    MVP[u].v = e2;
    MapVerPair::iterator itMVP = MVP.begin();
    //
    cout << "Start enumeration" << endl;
    enumMatch(itMVP);

    // no mapping => false
    if (AllXPairHash.size() == 0) {
      terminate.value = false;
      terminate.determined = true;
    }
    // end

    // see the Xpair
    cout << "X" << endl;
    for (MapListPair::iterator it = LXPair.begin(); it != LXPair.end(); it++) {
      cout << it->first << "-> ";
      for (int i = 0; i < it->second.size(); i++) {
        cout << "X" << it->second[i] << ", ";
      }
      cout << endl;
    }

    cout << "end newRun" << endl;

  }

  void enumMatch(MapVerPair::iterator itMVP) {
    // early terminate
    if (terminate.value)
      return;

    MapVerPair::iterator postItMVP = itMVP;
    postItMVP++;

    if (postItMVP == MVP.end()) {
      // check
      // print M first
      cout << "++++Mapping:" << endl;
      for (typename DIGRAPHBASIC::VLabels::iterator it =
          dq->getVLabel().begin(); it != dq->getVLabel().end(); it++) {
        VertexID u = it->first;
        cout << u << ": " << MVP[u];
      }
      cout << endl << "----Mapping" << endl;

      Mcnt++;
      LXPair[Mcnt];
      // TODO
      // generate boolean conjunction
      for (typename DIGRAPHBASIC::VLabels::iterator it =
          dq->getVLabel().begin(); it != dq->getVLabel().end(); it++) {
        VertexID u = it->first;
        if (u == dq->e)
          continue;

        VertexID v1 = MVP[u].u;
        VertexID v2 = MVP[u].v;

        // X_{(vg1, vg2)} = true
        if (v1 == v2) {
          continue;
        } else {
          // X_{(vg1, vg2)} = false
          EntityPair p(v1, v2);
          LXPair[Mcnt].push_back(p);

          // generate pair hash
          ULong ulong;
          if (v1 < v2) {
            ulong = pairFunction(v1, v2);
          } else {
            ulong = pairFunction(v2, v1);
          }

          if (AllXPairHash.find(ulong) != AllXPairHash.end()) {
            continue;
          }

          AllXPairHash[ulong].u = p.p.u;
          AllXPairHash[ulong].v = p.p.v;
        }
      }

      // early termination
      if (LXPair[Mcnt].size() == 0) {
        terminate.value = true;
        terminate.determined = true;
      }
      //
      return;
    }

    // intersect to avoid redundant
//    set<VertexID> temp;
//    twoSetsIntersection(itCq1->second, itCq2->second, temp);

    VertexID u = postItMVP->first;
    // for each v1
    // TODO: may be a bug here, only outEdge here
    for (typename DIGRAPHBASIC::AdjList::iterator itg1 = dg->getOutEdge()[itMVP
        ->second.u].begin(); itg1 != dg->getOutEdge()[itMVP->second.u].end();
        itg1++) {
      VertexID v1 = itg1->first;

      if (dg->getVLabel(v1) != dq->getVLabel(u)) {
        continue;
      }

      // if v1 is check before
      if (dg->_vVisited[v1]) {
        continue;
      }

      // check if for all (u, up) \in dq => (v1, vp) \in dg
      if (cmpUVOutEdges(u, v1, 1)) {
        continue;
      }

      // check if for all (up, u) \in dq => (vp, v1) \in dg
      if (cmpUVInEdges(u, v1, 1)) {
        continue;
      }

//      bool isInter1 = (temp.find(v1) != temp.end());

      // for each v2
      for (typename DIGRAPHBASIC::AdjList::iterator itg2 =
          dg->getOutEdge()[itMVP->second.v].begin();
          itg2 != dg->getOutEdge()[itMVP->second.v].end(); itg2++) {
        VertexID v2 = itg2->first;

        if (dg->getVLabel(v2) != dq->getVLabel(u)) {
          continue;
        }

        // check if v2 is check before
        if (dg->_vVisited[v2]) {
          continue;
        }

//        bool isInter2 = (temp.find(v2) != temp.end());

//        if (isInter1 && isInter2) {
//          if (v1 > v2) {
//            continue;
//          }
//        }

        // check if for all (u, up) \in dq => (v2, vp) \in dg
        if (cmpUVOutEdges(u, v2, 2)) {
          continue;
        }

        // check if for all (up, u) \in dq => (vp, v2) \in dg
        if (cmpUVInEdges(u, v2, 2)) {
          continue;
        }

        // That is it!
        MVP[u].u = v1;
        MVP[u].v = v2;
        dg->_vVisited[v1] = dg->_vVisited[v2] = true;
        enumMatch(postItMVP);

        // early terminate
        if (terminate.value) {
          return;
        }

        dg->_vVisited[v1] = dg->_vVisited[v2] = false;
        MVP[u].u = NO_EDGE;
        MVP[u].v = NO_EDGE;
      }
    }
  }

  // end of class
}
;

#endif /* EL_H_ */
