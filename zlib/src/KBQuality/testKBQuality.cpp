/*
 * testKBQuality.cpp
 *
 *  Created on: 2015¦~1¤ë19¤é
 *      Author: zfan
 */

#include "testKBQuality.h"
#include "../utility/GlobalDefinition.h"
#include "../utility/Pair.h"
#include "../utility/utilityFunction.h"
#include "GraphSimCal.h"

TestKBQuality::TestKBQuality() {

}

void TestKBQuality::loadFromInputFile(const char* input_g_name, int _g_cnt,
                                      const char* input_q_name, int _q_cnt) {

  // start graph load
  m_InputStream.open(input_g_name);
  ASSERT(m_InputStream.is_open());
  g_cnt = _g_cnt;
  graphDB.resize(g_cnt);

  for (int i = 0; i < g_cnt; i++) {
    DIGRAPHBASIC *dg = new DIGRAPHBASIC();

    // begin read
    string line;
    while (getline(m_InputStream, line)) {
      istringstream iss(line);
      VertexID in_v_id;
      VertexLabel v_l;
      iss >> in_v_id >> v_l;  // vertex ID
      dg->insertVertex(in_v_id, v_l);

      VertexID out_v_id;
      EdgeLabel e_l = 0;
      while (iss >> out_v_id) {
        if (!dg->isVertex(out_v_id)) {
          dg->insertVertex(out_v_id, out_v_id);
        }
        if (!dg->isEdge(in_v_id, out_v_id)) {
          dg->insertEdge(in_v_id, out_v_id, e_l);
        }
      }
    }

    // output test
//      dg->printGraph(cout);
    graphDB[i] = dg;
  }

  m_InputStream.close();
  // end graph load

  // start query load
  m_InputStream.open(input_q_name);
  ASSERT(m_InputStream.is_open());
  q_cnt = _q_cnt;
  queryDB.resize(q_cnt);

  // pass header '#'
  string line;
  getline(m_InputStream, line);

  for (int i = 0; i < q_cnt; i++) {
    DIKEYS *dq = new DIKEYS();

    // begin read

    while (getline(m_InputStream, line)) {
      istringstream iss(line);

      if (line[0] == '#') {
        break;
      }

      VertexID in_v_id;
      VertexLabel v_idx, v_t;
      iss >> in_v_id >> v_idx >> v_t;  // vertex ID
      dq->insertVertex(in_v_id, Pair(v_idx, v_t));

      VertexID out_v_id;
      EdgeLabel e_l = 0;
      while (iss >> out_v_id) {
        if (!dq->isVertex(out_v_id)) {
          dq->insertVertex(out_v_id, Pair(out_v_id, -1));
        }
        dq->insertEdge(in_v_id, out_v_id, e_l);
      }
    }

    // output test
//      dq->printGraph(cout);
    dq->getDiameter(0);
//      cout << "diameter from x: " << dq->diameter << endl;
    queryDB[i] = dq;
  }

  m_InputStream.close();
  // end query load
}

void TestKBQuality::loadDNeighbor() {
  // for each graph
  for (int i = 0; i < g_cnt; i++) {
    DIGRAPHBASIC *dg = graphDB[i];

    // for each entity e
    for (typename DIGRAPHBASIC::VLabels::iterator eit = dg->getVLabel().begin();
        eit != dg->getVLabel().end(); eit++) {
      VertexID e = eit->first;
      // e is not a value data
      if (dg->getVLabel(e) < 0) {
        continue;
      }
      cout << "d-neighbor nodes from " << e << endl;

      // initialize nodes within d hops from e
      set<VertexID>* visit_v = new set<VertexID>();

      // for each Q(x) where x and e are with same type
      for (int j = 0; j < q_cnt; j++) {
        DIKEYS *dq = queryDB[j];

        // x and e are same type
        if (dq->getVLabel(0).v != dg->getVLabel(e)) {
          continue;
        }

        // construct nodes by Q(x)
        dg->getDNeighbor(e, dq->diameter, *visit_v);
      }  // end of Q
      GdVDB[e] = visit_v;
    }  // end of e
  }  // end of G
}

void TestKBQuality::comSim() {
  // for each graph
  for (int i = 0; i < g_cnt; i++) {
    DIGRAPHBASIC *G = graphDB[i];

      // for each Q
      for (int j = 0; j < q_cnt; j++) {
        DIKEYS *Q = queryDB[j];

        MapIntHset simset;
        GraphSimCal gsc;
        bool sim_flag = gsc.SimCal(G, Q, simset);

        cout << "simulation result of Q" << j << ": " << endl;
        for (MapIntHset::iterator it1 = simset.begin(); it1 != simset.end(); it1++) {
          int u = it1->first;
          cout << u << "-> ";
          for (unordered_set<int>::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
            int v = *it2;
            cout << v << ", ";
          }
          cout << endl;
        }
      }  // end of Q
  }  // end of G
}
bool TestKBQuality::checkPredicateObjectFeasibility(DIGRAPHBASIC *dg,
                                                    EdgeLabel p_1, VertexID o_1,
                                                    DIKEYS *dq, EdgeLabel p_Q,
                                                    VertexID o_Q) {
  // check feasibility
  // edge predicate p = p_Q
  if (p_1 != p_Q) {
    return false;
  }

  // o_Q_t is y, _y or d
  VertexLabel o_t = dg->getVLabel(o_1);
  Pair o_Q_t(dq->getVLabel(o_Q));
  if (o_Q_t.u == 1 || o_Q_t.u == 2 || o_Q_t.u == 4 || o_Q_t.u == 5) {
    if (o_Q_t.v == o_t) {
      return true;
    } else {
      return false;
    }
  }

  // does nothing, if o_Q_t is y*
  return true;
}

void TestKBQuality::pairing() {
  // for each graph
  for (int i = 0; i < g_cnt; i++) {
    DIGRAPHBASIC *G = graphDB[i];

    // for each (e_1, e_2) with same type
    for (typename DIGRAPHBASIC::VLabels::iterator eit1 = G->getVLabel().begin();
        eit1 != G->getVLabel().end(); eit1++) {
      VertexID e_1 = eit1->first;
      DIGRAPHBASIC *G_1 = GdDB[e_1];
      // e_1 is not a value data
      if (G->getVLabel(e_1) < 0) {
        continue;
      }

      for (typename DIGRAPHBASIC::VLabels::iterator eit2 = eit1;
          eit2 != G->getVLabel().end(); eit2++) {
        VertexID e_2 = eit2->first;
        DIGRAPHBASIC *G_2 = GdDB[e_2];
        // e_1 is not a value data
        // e_1 != e_2 and
        // they are same type
        if (G->getVLabel(e_2) < 0 || e_1 == e_2
            || G->getVLabel(e_1) != G->getVLabel(e_2)) {
          continue;
        }

        // check by Q
        for (int j = 0; j < q_cnt; j++) {
          DIKEYS *Q = queryDB[j];
          // TODO
//          if (checkPairing(G_1, G_2, e_1, e_2, Q)) {
//            L[pair(e_1, e_2)] = Q;
//          }
        }  // end of Q
      }  // end of e_2
    }  // end of e_1
  }  // end of G
}

bool TestKBQuality::checkPairing(DIGRAPHBASIC *G_1, DIGRAPHBASIC *G_2,
                                 VertexID e_1, VertexID e_2, DIKEYS *Q) {
  // for each triple of Q

}
