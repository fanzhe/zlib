/*
 * EL.h
 *
 *  Created on: 2014�~8��2��
 *      Author: zfan
 */

#ifndef EL_H_
#define EL_H_

#include "DiGraph.h"
#include <stack>

class Pair {
 public:
  VertexID u;
  VertexID v;

  Pair() {

  }

  Pair(VertexID _u, VertexID _v)
      : u(_u),
        v(_v) {
  }

  ~Pair() {

  }

  void operator =(const Pair& _p) {
    u = _p.u;
    v = _p.v;
  }
};

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

class EL {
 public:
  DIGRAPHBASIC* dg;
  DIGRAPHBASIC* dq;
  DIGRAPHDGQ* dGq;
  VertexID e1, e2;
  CandQtoG Cq1, Cq2, Cqg;  // (dq, e1, dg), (dq, e2, dg), (dq, dGq)
  Map M;

  EL(DIGRAPHBASIC* _dq, VertexID u, DIGRAPHBASIC* _dg, VertexID v1,
     VertexID v2) {
    dg = _dg;
    dq = _dq;

    e1 = v1;
    e2 = v2;
    // initialize size
    dq->initEL(u);
    dg->initEL(v1);

    dGq = new DIGRAPHDGQ();
  }

  /**
   * main portal
   * 1. search Cq1 and Cq2
   * 2. refine Cq1 and Cq2
   * 3. produce dGq by Cq1 and Cq2
   * 4. minimize dGq
   * 5. final check mapping
   */
  void run() {
//    dq->removeAllInEdges(6);
//    dq->printGraph(cout);
//    return;
    // check if (dg, dq) |= (e1, e2)

    // search candidate Cq1 and Cq2
    dq->initEdgeVisited();
    search(Cq1, e1);
    refineCQ(Cq1);
    dq->initEdgeVisited();
    search(Cq2, e2);
    refineCQ(Cq2);

    // product dGq
    dq->initEdgeVisited();
    productGQ();

    cout << "product graph before: " << endl;
    dGq->printGraph(cout);

    // minimize dGq
    minimizeGQ();

    cout << "product graph after: " << endl;
    dGq->printGraph(cout);

    // check
    check();
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

  void insertInverse(VertexID u, VertexID up, VertexID& vid) {
    // (up, u)
    DGQVertex tmp_dGqv;
    // if up is check before
    if (Cqg.find(up) != Cqg.end()) {
      // for each *dGqvid* that u mapped in Cqg
      for (set<VertexID>::iterator itq2 = Cqg[u].begin(); itq2 != Cqg[u].end();
          itq2++) {
        VertexID dGqvid = *itq2;
        const DGQVertex& dGqv = dGq->getVLabel(dGqvid);
        VertexID v1 = dGqv.vp.u;
        VertexID v2 = dGqv.vp.v;

        // for each *dGqvidp* that up map to in Cqg
        for (set<VertexID>::iterator it1 = Cqg[up].begin();
            it1 != Cqg[up].end(); it1++) {
          VertexID dGqvidp = *it1;
          const DGQVertex& dGqv = dGq->getVLabel(dGqvidp);
          VertexID v1p = dGqv.vp.u;
          VertexID v2p = dGqv.vp.v;

          // (v1p, v1) and (v2p, v2) are edges, and label are consistent
          if (dg->isEdge(v1p, v1) && dg->isEdge(v2p, v2)) {
            if (dg->getELabel(v1p, v1) == dg->getELabel(v2p, v2)) {
              dGq->insertEdge(dGqvidp, dGqvid, dq->getELabel(up, u));
            }
          }
        }
      }
    } else {  // never check
      // for each *dGqvid* that u mapped in Cqg
      for (set<VertexID>::iterator itq2 = Cqg[u].begin(); itq2 != Cqg[u].end();
          itq2++) {
        VertexID dGqvid = *itq2;
        const DGQVertex& dGqv = dGq->getVLabel(dGqvid);
        VertexID v1 = dGqv.vp.u;
        VertexID v2 = dGqv.vp.v;

        // for each v1p that up maps in Cq1
        for (set<VertexID>::iterator it1 = Cq1[up].begin();
            it1 != Cq1[up].end(); it1++) {
          VertexID v1p = *it1;
          // for each v2p that up maps in Cq2
          for (set<VertexID>::iterator it2 = Cq2[up].begin();
              it2 != Cq2[up].end(); it2++) {
            VertexID v2p = *it2;

            // (v1p, v1) and (v2p, v2) are edges, and label are consistent
            if (dg->isEdge(v1p, v1) && dg->isEdge(v2p, v2)) {
              if (dg->getELabel(v1p, v1) == dg->getELabel(v2p, v2)) {
                // insert vertex
                tmp_dGqv.set(up, Pair(v1p, v2p));
                dGq->insertVertex(vid, tmp_dGqv);
                // insert edge
                dGq->insertEdge(vid, dGqvid, dg->getELabel(v1p, v1));
                Cqg[up].insert(vid);
                vid++;
              }
            }
          }
        }
      }
    }
  }

  void insert(VertexID u, VertexID up, VertexID& vid) {
    // (u, up)
    DGQVertex tmp_dGqv;

    // if up is check before
    if (Cqg.find(up) != Cqg.end()) {
      // for each *dGqvid* that u mapped in Cqg
      for (set<VertexID>::iterator itq2 = Cqg[u].begin(); itq2 != Cqg[u].end();
          itq2++) {
        VertexID dGqvid = *itq2;
        const DGQVertex& dGqv = dGq->getVLabel(dGqvid);
        VertexID v1 = dGqv.vp.u;
        VertexID v2 = dGqv.vp.v;

        // for each *dGqvidp* that up map to in Cqg
        for (set<VertexID>::iterator it1 = Cqg[up].begin();
            it1 != Cqg[up].end(); it1++) {
          VertexID dGqvidp = *it1;
          const DGQVertex& dGqv = dGq->getVLabel(dGqvidp);
          VertexID v1p = dGqv.vp.u;
          VertexID v2p = dGqv.vp.v;

          // (v1, v1p) and (v2, v2p) are edges, and label are consistent
          if (dg->isEdge(v1, v1p) && dg->isEdge(v2, v2p)) {
            if (dg->getELabel(v1, v1p) == dg->getELabel(v2, v2p)) {
              dGq->insertEdge(dGqvid, dGqvidp, dq->getELabel(u, up));
            }
          }
        }
      }
    } else {  // never check
      // for each *dGqvid* that u mapped in Cqg
      for (set<VertexID>::iterator itq2 = Cqg[u].begin(); itq2 != Cqg[u].end();
          itq2++) {
        VertexID dGqvid = *itq2;
        const DGQVertex& dGqv = dGq->getVLabel(dGqvid);
        VertexID v1 = dGqv.vp.u;
        VertexID v2 = dGqv.vp.v;

        // for each v1p that up maps in Cq1
        for (set<VertexID>::iterator it1 = Cq1[up].begin();
            it1 != Cq1[up].end(); it1++) {
          VertexID v1p = *it1;
          // for each v2p that up maps in Cq2
          for (set<VertexID>::iterator it2 = Cq2[up].begin();
              it2 != Cq2[up].end(); it2++) {
            VertexID v2p = *it2;

            // (v1, v1p) and (v2, v2p) are edges, and label are consistent
            if (dg->isEdge(v1, v1p) && dg->isEdge(v2, v2p)) {
              if (dg->getELabel(v1, v1p) == dg->getELabel(v2, v2p)) {
                // insert vertex
                tmp_dGqv.set(up, Pair(v1p, v2p));
                dGq->insertVertex(vid, tmp_dGqv);
                // insert edge
                dGq->insertEdge(dGqvid, vid, dg->getELabel(v1, v1p));
                Cqg[up].insert(vid);
                vid++;
              }
            }
          }
        }
      }
    }
  }

  /*
   * TODO
   * G^Q = G_{e1}^Q \times G_{e2}^Q
   */
  void productGQ() {
    VertexID u = dq->e;
    VertexID up;
    stack<VertexID> S;
    S.push(u);

    VertexID vid = 1;

    // initialize: insert vertex (v1, v2)
    VertexID v1 = *(Cq1[u].begin());
    VertexID v2 = *(Cq2[u].begin());
    DGQVertex tmp_dGqv(u, Pair(v1, v2));          // use in the following
    Cqg[u].insert(vid);          // q -> dGq
    dGq->insertVertex(vid++, tmp_dGqv);          // dGq -> (vid, dgqv)

    while (!S.empty()) {
      u = S.top();
      S.pop();

      // for each unvisited edge (u, up) in dq
      for (typename DIGRAPHBASIC::AdjList::iterator itq1 = dq->getOutEdge()[u]
          .begin(); itq1 != dq->getOutEdge()[u].end(); itq1++) {
        up = itq1->first;
        // (u, up) is visited
        if (itq1->second.isVisited) {
          continue;
        }
        itq1->second.isVisited = true;
        S.push(up);
        insert(u, up, vid);
      }

      // for each unvisited edge (up, u) in dq
      for (typename DIGRAPHBASIC::AdjListBool::iterator itq1 =
          dq->getInVertex()[u].begin(); itq1 != dq->getInVertex()[u].end();
          itq1++) {
        up = itq1->first;
        if (dq->getOutEdge()[up][u].isVisited) {
          continue;
        }
        dq->getOutEdge()[up][u].isVisited = true;
        S.push(up);
        insertInverse(u, up, vid);
      }
    }
  }

  /**
   * optimization: minimize dGq
   */
  void minimizeGQ() {
    // dGq, dq, Cqg
    bool changed = true;

    while (changed) {
      bool terminate = false;
      // for each u in dq
      for (typename DIGRAPHBASIC::VLabels::iterator itq1 =
          dq->getVLabel().begin(); itq1 != dq->getVLabel().end(); itq1++) {
        VertexID u = itq1->first;

        // for each up in dq, where (u, up)
        for (typename DIGRAPHBASIC::AdjList::iterator itq2 = dq->getOutEdge()[u]
            .begin(); itq2 != dq->getOutEdge()[u].end(); itq2++) {
          VertexID up = itq2->first;

          // start do something
          // for each v in dGq, v \in Cqg[u]
          for (set<VertexID>::iterator itGq1 = Cqg[u].begin();
              itGq1 != Cqg[u].end();) {
            VertexID v = *itGq1;

            bool flag = true;
            // for each vp in dGq, u \in Cqg[up]
            for (set<VertexID>::iterator itGq2 = Cqg[up].begin();
                itGq2 != Cqg[up].end(); itGq2++) {
              VertexID vp = *itGq2;

              if (dGq->isEdge(v, vp)) {
                flag = false;
                break;
              }
            }

            // no edge
            if (flag) {
              // remove v from Cqg[u] and dGq
              dGq->removeVertex(v);
              Cqg[u].erase(itGq1++);
              terminate = true;
            } else {
              itGq1++;
            }
          }
        }

        // for each up in dq, where (up, u)
        for (typename DIGRAPHBASIC::AdjListBool::iterator itq2 =
            dq->getInVertex()[u].begin(); itq2 != dq->getInVertex()[u].end();
            itq2++) {
          VertexID up = itq2->first;

          // start do something
          // for each v in dGq, v \in Cqg[u]
          for (set<VertexID>::iterator itGq1 = Cqg[u].begin();
              itGq1 != Cqg[u].end();) {
            VertexID v = *itGq1;

            bool flag = true;
            // for each vp in dGq, u \in Cqg[up]
            for (set<VertexID>::iterator itGq2 = Cqg[up].begin();
                itGq2 != Cqg[up].end(); itGq2++) {
              VertexID vp = *itGq2;

              if (dGq->isEdge(vp, v)) {
                flag = false;
                break;
              }
            }

            // no edge
            if (flag) {
              // remove v
              dGq->removeVertex(v);
              Cqg[u].erase(itGq1++);
              terminate = true;
            } else {
              itGq1++;
            }
          }
        }

      }

      if (terminate)
        changed = true;
      else
        changed = false;
    }
  }

  /*
   * TODO
   * check e1 <=> e2 by dGq and Cqg
   */
  void check() {
    // dq, dGq, Cqg
    /**
     * Cqg[1] -> {v1 ...}
     * Cqg[2] -> {v2 ...}
     * Cqg[3] -> {v3 ...}
     * ...
     * Cqg[m] -> {vn ...}
     */

    // initialize the first mapping
    dq->initEdgeVisited();
    VertexID u = dq->e;
    VertexID v = *(Cqg[u].begin());
    M[u] = v;
//    enumMatch(0 + 1);
    _enumMatch(1, u);
  }

  void enumMatch(int depth) {
//    if (depth == Cqg.size()) {
//      // end
//
//    }
//
//    CandQtoG::iterator it = Cqg.begin() + depth;
//    VertexID u = it->first;
//    for (set<VertexID>::iterator it1 = it->second.begin(); it1 != Cqg.end();
//        it1++) {
//      VertexID v = *it1;
//
//    }
  }

  void _enumMatch(int depth, VertexID u) {
//    cout << "d: " << depth << " ";
    if (depth == dq->Vcnt) {
      // check
      // output M first
      cout << "++++Mapping:" << endl;
      for (typename DIGRAPHBASIC::VLabels::iterator it =
          dq->getVLabel().begin(); it != dq->getVLabel().end(); it++) {
        VertexID u = it->first;
        cout << M[u] << " ";
      }
      cout << endl;
    }

    VertexID v = M[u];
    cout << v << " ";
    // for each vp in dGq, where (v, vp)
    for (typename DIGRAPHDGQ::AdjList::iterator itgq = dGq->getOutEdge()[v]
        .begin(); itgq != dGq->getOutEdge()[v].end(); itgq++) {
      VertexID vp = itgq->first;
      VertexID up = dGq->getVLabel(vp).u;

      // if (u, up) is visited
      if (dq->getOutEdge()[u][up].isVisited)
        continue;
      dq->getOutEdge()[u][up].isVisited = true;

      M[up] = vp;
      _enumMatch(depth + 1, up);

      dq->getOutEdge()[u][up].isVisited = false;
    }

    // for each vp in dGq, where (vp, v)
    for (typename DIGRAPHDGQ::AdjListBool::iterator itgq = dGq->getInVertex()[v]
        .begin(); itgq != dGq->getInVertex()[v].end(); itgq++) {
      VertexID vp = itgq->first;
      VertexID up = dGq->getVLabel(vp).u;

      // if (up, u) is visited
      if (dq->getOutEdge()[up][u].isVisited)
        continue;
      dq->getOutEdge()[up][u].isVisited = true;

      M[up] = vp;
      _enumMatch(depth + 1, up);

      dq->getOutEdge()[up][u].isVisited = false;
    }
  }

//  void enumMatch(int depth, VertexID u) {
//    cout << "d: " << depth << " ";
//    if (depth == dq->Ecnt) {
//      // check
//      // output M first
//      cout << "++++Mapping:" << endl;
//      for (typename DIGRAPHBASIC::VLabels::iterator it =
//          dq->getVLabel().begin(); it != dq->getVLabel().end(); it++) {
//        VertexID u = it->first;
//        cout << M[u] << " ";
//      }
//      cout << endl;
//    }
//
//    VertexID v = M[u];
////    cout << v << " ";
//    // for each up, where (u, up)
//    for (typename DIGRAPHBASIC::AdjList::iterator it =
//        dq->getOutEdge()[u].begin(); it != dq->getOutEdge()[u].end(); it++) {
//      VertexID up = it->first;
//
//      // (u, up) is visited
//      if (it->second.isVisited) {
//        continue;
//      }
//      it->second.isVisited = true;
//
//      // for each vp
//      for (set<VertexID>::iterator it1 = Cqg[up].begin(); it1 != Cqg[up].end();
//          it1++) {
//        VertexID vp = *it1;
//
//        // if (v, vp) is not an edge
//        if (!dGq->isEdge(v, vp)) {
//          continue;
//        }
//
//        M[up] = vp;
//        enumMatch(depth + 1, up);
//      }
//
//      it->second.isVisited = false;
//    }
//
//    // for each up, where (up, u)
//    for (typename DIGRAPHBASIC::AdjListBool::iterator it = dq->getInVertex()[u]
//        .begin(); it != dq->getInVertex()[u].end(); it++) {
//      VertexID up = it->first;
//
//      // (up, u) is visited
//      if (dq->getOutEdge()[up][u].isVisited) {
//        continue;
//      }
//      dq->getOutEdge()[up][u].isVisited = true;
//
//      // for each vp
//      for (set<VertexID>::iterator it1 = Cqg[up].begin(); it1 != Cqg[up].end();
//          it1++) {
//        VertexID vp = *it1;
//
//        // if (vp, v) is not an edge
//        if (!dGq->isEdge(vp, v)) {
//          continue;
//        }
//        M[up] = vp;
//        enumMatch(depth + 1, up);
//      }
//
//      dq->getOutEdge()[up][u].isVisited = false;
//    }
//  }

}
;

#endif /* EL_H_ */
