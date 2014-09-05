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
#include <stack>
#include <vector>

using namespace std;

class DGQVertex {
 public:
  VertexID u;  // query vertex
  Pair vp;  // graph vertices pair

  DGQVertex() {

  }

  DGQVertex(VertexID _u, const Pair& _vp)
      : u(_u),
        vp(_vp) {
  }

  ~DGQVertex() {

  }

  void operator=(const DGQVertex& _dgqv) {
    u = _dgqv.u;
    vp = _dgqv.vp;
  }

  friend inline ostream& operator<<(ostream& out, const DGQVertex& obj) {
    out << "(" << obj.u << ", (" << obj.vp.u << ", " << obj.vp.v << "))";
    return out;
  }

  void set(VertexID _u, const Pair& _vp) {
    u = _u;
    vp = _vp;
  }
};


/**
 * TODO:
 * 1. We need to treat *data value* and *variable* separately
 * 2.
 */
class EL {
 public:
  typedef unordered_map<VertexID, set<VertexID> > CandQtoG;

  DIGRAPHBASIC* dg;
  DIGRAPHBASIC* dq;
  VertexID e1, e2;
  CandQtoG Cq1, Cq2;  // (dq, e1, dg), (dq, e2, dg)

  Map M;
  MapVerPair MVP;

  int Mcnt;

  MapPairHash AllXPairHash;
  MapListPair LXPair;
  bool terminate;
  bool determined;

  EL(DIGRAPHBASIC* _dq, VertexID u, DIGRAPHBASIC* _dg, VertexID _e1,
     VertexID _e2) {
    dg = _dg;
    dq = _dq;

    e1 = _e1;
    e2 = _e2;
    // initialize size
    dq->initEL(u);
    // TODO:
    // BFS to locate $G_{d_Q}$
    dg->initEL(e1);
  }

  /**
   * main portal
   * 1. search Cq1 and Cq2
   * 2. refine Cq1 and Cq2
   * 3. produce dGq by Cq1 and Cq2
   * 4. minimize dGq
   * 5. check mapping
   * 6. get results (T/F or a set of vertex pairs)
   */
  void run() {
    // check if (dg, dq) |= (e1, e2)
    // search candidate Cq1 and Cq2
    dq->initEdgeVisited();
    search(Cq1, e1);
    refineCQ(Cq1);
    dq->initEdgeVisited();
    search(Cq2, e2);
    refineCQ(Cq2);

    _run();

    // finish
    cout << "finish!" << endl;
  }

  bool checkLabelAndDeg(VertexID u, VertexID v) {
    if (dq->getVLabel(u) != dg->getVLabel(v)) {
      return false;
    }
    for (LabelCnt::iterator it = dq->_outLabelCnt[u].begin();
        it != dq->_outLabelCnt[u].end(); it++) {
      Label ul = it->first;
      if (dg->_outLabelCnt[v].find(ul) == dg->_outLabelCnt[v].end())
        return false;
      if (dq->_outLabelCnt[u][ul] > dg->_outLabelCnt[v][ul]) {
        return false;
      }
    }

    for (LabelCnt::iterator it = dq->_inLabelCnt[u].begin();
        it != dq->_inLabelCnt[u].end(); it++) {
      Label ul = it->first;
      if (dg->_inLabelCnt[v].find(ul) == dg->_inLabelCnt[v].end())
        return false;
      if (dq->_inLabelCnt[u][ul] > dg->_inLabelCnt[v][ul]) {
        return false;
      }
    }
    return true;
  }

  /**
   * refine the candidate set before product
   * trick: special case
   */
  void refineCQ(CandQtoG& Cq) {
    bool finish = true;
    while (finish) {
      bool terminate = false;
      // for each u in dq
      for (typename DIGRAPHBASIC::VLabels::iterator itq =
          dq->getVLabel().begin(); itq != dq->getVLabel().end(); itq++) {
        VertexID u = itq->first;

        if (Cq[u].size() == 1) {
          VertexID v = *Cq[u].begin();
          for (typename DIGRAPHBASIC::VLabels::iterator itq1 = dq->getVLabel()
              .begin(); itq1 != dq->getVLabel().end(); itq1++) {
            VertexID u1 = itq1->first;
            if (u == u1) {
              continue;
            }

            // erase v
            if (Cq[u1].find(v) != Cq[u1].end()) {
              Cq[u1].erase(v);
              terminate = true;
            }
          }
        }
      }

      if (terminate) {
        finish = true;
      } else {
        finish = false;
      }
    }
  }

  /*
   * TODO:
   * G_{e}^Q
   */
  void search(CandQtoG& Cq, VertexID v) {
    // L(q, e) != L(g, e)
    VertexID u = dq->e;
    VertexID up, vp;
    if (!checkLabelAndDeg(u, v)) {
      return;
    }
    Cq[u].insert(v);
    stack<VertexID> S;
    S.push(u);

    while (!S.empty()) {
      u = S.top();
      S.pop();

      // for each unvisited edge (u, up) in dq
      for (typename DIGRAPHBASIC::AdjList::iterator itq = dq->getOutEdge()[u]
          .begin(); itq != dq->getOutEdge()[u].end(); itq++) {
        up = itq->first;
        // (u, up) is visited
        if (itq->second.isVisited) {
          continue;
        }
        itq->second.isVisited = true;
        // Cq[up] was check before
        // TODO: we can do more here
        if (Cq.find(up) != Cq.end()) {
          continue;
        }

        S.push(up);

        // for each v in dg, u -> v
        for (set<VertexID>::iterator itq1 = Cq[u].begin(); itq1 != Cq[u].end();
            itq1++) {
          v = *itq1;
          // pick vp in dg
          for (typename DIGRAPHBASIC::AdjList::iterator itg =
              dg->getOutEdge()[v].begin(); itg != dg->getOutEdge()[v].end();
              itg++) {
            vp = itg->first;
            if (dq->getELabel(u, up) != dg->getELabel(v, vp))
              continue;
            if (!checkLabelAndDeg(up, vp))
              continue;
            Cq[up].insert(vp);
          }
        }
      }

      // for each unvisited edge (up, u) in dq
      for (typename DIGRAPHBASIC::AdjListBool::iterator itq =
          dq->getInVertex()[u].begin(); itq != dq->getInVertex()[u].end();
          itq++) {
        up = itq->first;
        if (dq->getOutEdge()[up][u].isVisited) {
          continue;
        }
        dq->getOutEdge()[up][u].isVisited = true;
        // Cq[up] was check before
        if (Cq.find(up) != Cq.end())
          continue;

        S.push(up);

        // for each v in dg, u -> v
        for (set<VertexID>::iterator itq1 = Cq[u].begin(); itq1 != Cq[u].end();
            itq1++) {
          v = *itq1;
          // pick vp in dg
          for (typename DIGRAPHBASIC::AdjListBool::iterator itg = dg
              ->getInVertex()[v].begin(); itg != dg->getInVertex()[v].end();
              itg++) {
            vp = itg->first;
            if (dq->getELabel(up, u) != dg->getELabel(vp, v))
              continue;
            if (!checkLabelAndDeg(up, vp))
              continue;
            Cq[up].insert(vp);
          }
        }
      }
    }

    cout << "---- match test" << endl;
    for (CandQtoG::iterator it = Cq.begin(); it != Cq.end(); it++) {
      cout << it->first << " ";
      for (set<VertexID>::iterator it1 = it->second.begin();
          it1 != it->second.end(); it1++) {
        cout << *it1 << " ";
      }
      cout << endl;
    }

    return;
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
    terminate = false;
    determined = false;

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
    cout << "gogogo" << endl;
    enumMatch(itMVP);

    // no mapping => false
    if (AllXPairHash.size() == 0) {
      terminate = false;
      determined = true;
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
    if (terminate)
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
        terminate = true;
        determined = true;
      }
      //
      return;
    }

    // intersect to avoid redundant
//    set<VertexID> temp;
//    twoSetsIntersection(itCq1->second, itCq2->second, temp);

    VertexID u = postItMVP->first;
    // for each v1
    // TODO: A bug here, cannot just outEdge
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
        if (terminate) {
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
