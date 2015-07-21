/*
 * testKBQuality.cpp
 *
 *  Created on: 2015¦~1¤ë19¤é
 *      Author: zfan
 */

#include "testKBQuality.h"
#include "GlobalDefinition.h"
#include "Pair.h"
#include "utilityFunction.h"
#include "GraphSimCal.h"
#include "ProductGraphSimCal.h"

TestKBQuality::TestKBQuality() {
  global_Gp = new DIPRODUCTGRAPH();
  thetaGd = thetaOrgGd = thetaL = 0;
}

void TestKBQuality::loadFromInputFile(const char* input_g_name, int _g_cnt,
                                      const char* input_q_name, int _q_cnt,
                                      const char* org_output_Gd_folder,
                                      const char* org_output_L_folder,
                                      const char* output_Gd_folder,
                                      const char* output_L_folder,
                                      const char* output_Gp_folder) {
  // assign folder name
  this->org_output_Gd_folder.assign(org_output_Gd_folder);
  this->org_output_L_folder.assign(org_output_L_folder);
  this->output_Gd_folder.assign(output_Gd_folder);
  this->output_L_folder.assign(output_L_folder);
  this->output_Gp_folder.assign(output_Gp_folder);

  // start graph load
  m_InputStream.open(input_g_name);
  ASSERT(m_InputStream.is_open());
  g_cnt = _g_cnt;
  graphDB.resize(g_cnt);
  GdVDB.resize(g_cnt);

  for (int i = 0; i < g_cnt; i++) {
    DIGRAPHBASIC *dg = new DIGRAPHBASIC();

    // begin read
    string line;
    // read node
    while (getline(m_InputStream, line)) {
      // go to read edge
      if (line[0] == '%') {
        break;
      }
      istringstream iss(line);
      VertexID in_v_id;
      VertexLabel v_l;
      iss >> in_v_id >> v_l;  // vertex ID
      if (in_v_id < 0 && v_l < 0) {
        // data value
        dg->insertVertex(in_v_id, in_v_id);
      } else {
        // entity
        dg->insertVertex(in_v_id, v_l);
      }
    }

    // read edge
    while (getline(m_InputStream, line)) {
      istringstream iss(line);
      VertexID s_v_id;
      VertexID d_v_id;
      EdgeLabel e_l = 0;
      iss >> s_v_id >> d_v_id >> e_l;
      if (!dg->isEdge(s_v_id, d_v_id)) {
        dg->insertEdge(s_v_id, d_v_id, e_l);
      }
    }

    // output test
//    cout << "G: " << endl;
//    dg->printGraph(cout);
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

    // read node
    while (getline(m_InputStream, line)) {
      istringstream iss(line);

      if (line[0] == '%') {
        break;
      }

      VertexID v_id;
      VertexLabel v_t;
      iss >> v_id >> v_t;  // vertex ID

      if (v_id == 0) {
        // x
        dq->insertVertex(v_id, Pair(1, v_t));
      } else if (v_id < 0 && v_t == -1) {
        // d
        dq->insertVertex(v_id, Pair(5, v_id));
      } else if (v_t == -2) {
        // y*
        dq->insertVertex(v_id, Pair(3, 0));
      } else if (v_id > 0 && v_t >= 0) {
        // y
        dq->insertVertex(v_id, Pair(2, v_t));
      } else if (v_id < 0 && v_t >= 0) {
        // _y
        dq->insertVertex(v_id, Pair(4, v_t));
      }
    }

    // read edge
    while (getline(m_InputStream, line)) {
      if (line[0] == '#') {
        break;
      }
      istringstream iss(line);

      VertexID s_v_id;
      VertexID d_v_id;
      EdgeLabel e_l = 0;
      iss >> s_v_id >> d_v_id >> e_l;
      if (!dq->isEdge(s_v_id, d_v_id)) {
        dq->insertEdge(s_v_id, d_v_id, e_l);
      }
    }
    // output test
//    dq->printGraph(cout);

    dq->e = 0;
    dq->getDiameter(dq->e);
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
    for (DIGRAPHBASIC::VLabels::iterator eit = dg->getVLabel().begin();
        eit != dg->getVLabel().end(); eit++) {
      VertexID e = eit->first;
//      cout << "d-neighbor nodes from " << e;
      // e is not a value data
      if (dg->getVLabel(e) < 0) {
        continue;
      }
//      cout << "d-neighbor nodes from " << e;

      // initialize nodes within d hops from e
      unordered_set<VertexID>* visit_v = new unordered_set<VertexID>();

      int max_diameter_q = 0;
      // for each Q(x) where x and e are with same type
      for (int j = 0; j < q_cnt; j++) {
        DIKEYS *dq = queryDB[j];
        // x and e are same type
        if (dq->getVLabel(dq->e).v != dg->getVLabel(e)) {
          continue;
        }
        if (max_diameter_q < dq->diameter)
          max_diameter_q = dq->diameter;
      }  // end of Q

      if (max_diameter_q == 0)
        continue;
//      cout << " of " << max_diameter_q << " hops " << endl;
      // construct nodes by Q(x)
      dg->getDNeighbor(e, max_diameter_q, *visit_v);

      outputGd(i, dg, *visit_v, e);
//      GdVDB[i][e] = visit_v;
//      printSet(*GdVDB[i][e]);
    }  // end of e
  }  // end of G
}

void TestKBQuality::outputGd(int i, DIGRAPHBASIC* G,
                             unordered_set<VertexID>& vIndG, VertexID e) {
  DIGRAPHBASIC* indG = new DIGRAPHBASIC();
  DIGRAPHBASIC* indGp = new DIGRAPHBASIC();
  unordered_set<VertexID>* vIndGp = new unordered_set<VertexID>();

  // induced subgraph
  getInducedSubgraph(G, vIndG, indG);
  indG->e = e;

  org_E.insert(e);
  printOrgDGraphChao(org_m_GdOutputStream, indG);

  thetaOrgGd++;
  if (thetaOrgGd > DEFAULTHETAORGGD) {
    cout << "out of orgGd" << endl;
    exit(0);
  }

  // check by Q
  for (int j = 0; j < q_cnt; j++) {
    DIKEYS *Q = queryDB[j];
    // check simulation between Q and indg
    comSim(Q, indG, *vIndGp);
  }  // end of Q

  if (vIndGp->size() != 0) {
    // induced from indG
    getInducedSubgraph(indG, *vIndGp, indGp);
    indGp->e = e;
    E.insert(e);

    // out put indGp
    // TODO use file output
//    cout << "updated Gd at " << e << endl;
//    indGp->printGraph(cout);
    printDGraphChao(m_GdOutputStream, indGp);

    thetaGd++;
    if (thetaGd > DEFAULTHETAGD) {
      cout << "out of Gd" << endl;
      exit(0);
    }

    GdVDB[i][e] = vIndGp;
  } else {
    delete vIndGp;
  }

  // clear
  delete indG;
  delete indGp;
}

void TestKBQuality::printOrgDGraphChao(ofstream& out, DIGRAPHBASIC* G) {
  // output G
  // file name by G.e
  // open file
  stringstream ss;
  ss << G->e;
  string sub = ss.str();
  string filename = org_output_Gd_folder + sub;
  out.open(filename.c_str());

  //  out << "Gd" << G->e << " " << endl;
  // output node
  for (DIGRAPHBASIC::VLabels::iterator it = G->getVLabel().begin();
      it != G->getVLabel().end(); it++) {
    if (it->first > 0) {
      out << it->first << " " << it->second << endl;
    } else {
      out << it->first << " " << -1 << endl;
    }
  }

  // output edge
  for (DIGRAPHBASIC::VLabels::iterator it = G->getVLabel().begin();
      it != G->getVLabel().end(); it++) {
    VertexID u = it->first;
    for (DIGRAPHBASIC::AdjList::iterator it1 = G->getOutEdge()[u].begin();
        it1 != G->getOutEdge()[u].end(); it1++) {
      VertexID v = it1->first;
      out << u << " " << v << " " << it1->second.elabel << endl;
    }
  }
  out.close();
}

void TestKBQuality::printDGraphChao(ofstream& out, DIGRAPHBASIC* G) {
  // output G
  // file name by G.e
  // open file
  stringstream ss;
  ss << G->e;
  string sub = ss.str();
  string filename = output_Gd_folder + sub;
  out.open(filename.c_str());

//  out << "Gd" << G->e << " " << endl;
  // output node
  for (DIGRAPHBASIC::VLabels::iterator it = G->getVLabel().begin();
      it != G->getVLabel().end(); it++) {
    if (it->first > 0) {
      out << it->first << " " << it->second << endl;
    } else {
      out << it->first << " " << -1 << endl;
    }
  }
  // output edge
  for (DIGRAPHBASIC::VLabels::iterator it = G->getVLabel().begin();
      it != G->getVLabel().end(); it++) {
    VertexID u = it->first;
    for (DIGRAPHBASIC::AdjList::iterator it1 = G->getOutEdge()[u].begin();
        it1 != G->getOutEdge()[u].end(); it1++) {
      VertexID v = it1->first;
      out << u << " " << v << " " << it1->second.elabel << endl;
    }
  }
  out.close();
}

void TestKBQuality::printOrgLChao(DIGRAPHBASIC* G) {
  // output L
  unordered_set<VertexID>::iterator it1 = org_E.begin();
  unordered_set<VertexID>::iterator it2;

  // collect label
  unordered_map<VertexLabel, unordered_set<VertexID> > map_org_E;
  for (it1 = org_E.begin(); it1 != org_E.end(); it1++) {
    VertexID e = *it1;
    VertexLabel l = G->getVLabel(e);
    map_org_E[l].insert(e);
  }

  // print
  unordered_map<VertexLabel, unordered_set<VertexID> >::iterator it3;
  for (it3 = map_org_E.begin(); it3 != map_org_E.end(); it3++) {
    VertexLabel l = it3->first;
    ofstream& out = org_m_LOutputStream;
    // initial open file stream
    int org_m_LOutputStreamCnt = 0;
    int org_m_LOutputStreamIndx = 1;  // start from 1

    stringstream ss1, ss2;
    ss1 << l;
    ss2 << org_m_LOutputStreamIndx;
    string filename = org_output_L_folder + "L." + ss1.str() + "." + ss2.str();
    out.open(filename.c_str());

    for (it1 = it3->second.begin(); it1 != it3->second.end(); it1++) {
      VertexID e1 = *it1;
      for (it2 = it1; it2 != it3->second.end(); it2++) {
        VertexID e2 = *it2;
        if (e1 == e2)
          continue;
        else {
          out << e1 << " " << e2 << " " << l << endl;
          org_m_LOutputStreamCnt++;
          // check overlap
          if (org_m_LOutputStreamCnt == DEFAULTENTITYFOREACHFILE) {
            out.close();
            org_m_LOutputStreamCnt = 0;
            org_m_LOutputStreamIndx++;
            stringstream ss1, ss2;
            ss1 << l;
            ss2 << org_m_LOutputStreamIndx;
            string filename = org_output_L_folder + "L." + ss1.str() + "."
                + ss2.str();
            out.open(filename.c_str());
          }
        }
      }  // end of e2
    }  // end of e1

    // close
    out.close();
  }  // end of label
}

void TestKBQuality::printLChao(ofstream& out, DIGRAPHBASIC* G) {
  // output L

  // collect label
  unordered_map<VertexLabel, vector<Pair> > map_L;
  for (int i = 0; i < L.size(); i++) {
    VertexLabel l = G->getVLabel(L[i].u);
    map_L[l].push_back(L[i]);
  }

  // print
  unordered_map<VertexLabel, vector<Pair> >::iterator it1;
  for (it1 = map_L.begin(); it1 != map_L.end(); it1++) {
    VertexLabel l = it1->first;
    // initial open file stream
    int m_LOutputStreamCnt = 0;
    int m_LOutputStreamIndx = 1;  // start from 1

    stringstream ss1, ss2;
    ss1 << l;
    ss2 << m_LOutputStreamIndx;
    string filename = output_L_folder + "L." + ss1.str() + "." + ss2.str();
    out.open(filename.c_str());

    for (int i = 0; i < it1->second.size(); i++) {
      VertexID u = (it1->second)[i].u;
      VertexID v = (it1->second)[i].v;

      out << u << " " << v << " " << l << endl;
      m_LOutputStreamCnt++;
      // check overlap
      if (m_LOutputStreamCnt == DEFAULTENTITYFOREACHFILE) {
        out.close();
        m_LOutputStreamCnt = 0;
        m_LOutputStreamIndx++;
        stringstream ss1, ss2;
        ss1 << l;
        ss2 << m_LOutputStreamIndx;
        string filename = output_L_folder + "L." + ss1.str() + "." + ss2.str();
        out.open(filename.c_str());
      }
    }  // end of L[i]
    // close
    out.close();
  }  // end of label
}

void TestKBQuality::printGpChao(ofstream& out) {
  // output Gp
  // output node
  string filename = output_Gp_folder + "Gp";
  out.open(filename.c_str());
  DIPRODUCTGRAPH::VLabels::iterator it = global_Gp->getVLabel().begin();
  for (; it != global_Gp->getVLabel().end(); it++) {
    out << it->first << " " << it->second.first << " " << it->second.second
        << " " << it->second.third;

    if (hash_L.find(hashPair(it->second.first, it->second.second))
        != hash_L.end()) {
      out << " " << 0 << endl;
    } else {
      out << endl;
    }
  }

// output edge
  for (DIPRODUCTGRAPH::VLabels::iterator it = global_Gp->getVLabel().begin();
      it != global_Gp->getVLabel().end(); it++) {
    VertexID u = it->first;
    for (DIPRODUCTGRAPH::AdjList::iterator it1 = global_Gp->getOutEdge()[u]
        .begin(); it1 != global_Gp->getOutEdge()[u].end(); it1++) {
      VertexID v = it1->first;
      out << u << " " << v << " " << it1->second.elabel.first << " "
          << it1->second.elabel.second << " " << it1->second.elabel.third
          << endl;
    }
  }

  out.close();
}

void TestKBQuality::getInducedSubgraph(DIGRAPHBASIC *G,
                                       unordered_set<VertexID> &vs,
                                       DIGRAPHBASIC *indG) {
// insert node
  for (unordered_set<VertexID>::iterator itv = vs.begin(); itv != vs.end();
      itv++) {
    indG->insertVertex(*itv, G->getVLabel(*itv));
  }

// insert edge
  for (unordered_set<VertexID>::iterator itv1 = vs.begin(); itv1 != vs.end();
      itv1++) {
    // for each node u
    VertexID u = *itv1;
    // for each (u, v)
    for (DIGRAPHBASIC::AdjList::iterator it2 = G->getOutEdge()[u].begin();
        it2 != G->getOutEdge()[u].end(); it2++) {
      VertexID v = it2->first;
      if (vs.find(v) != vs.end()) {
        indG->insertEdge(u, v, G->getELabel(u, v));
      }
    }
  }

  return;
  for (unordered_set<VertexID>::iterator itv1 = vs.begin(); itv1 != vs.end();
      itv1++) {
    VertexID u = *itv1;
    for (unordered_set<VertexID>::iterator itv2 = itv1; itv2 != vs.end();
        itv2++) {
      VertexID v = *itv2;
      if (G->isEdge(u, v)) {
        indG->insertEdge(u, v, G->getELabel(u, v));
      }
      if (G->isEdge(v, u)) {
        indG->insertEdge(v, u, G->getELabel(v, u));
      }
    }
  }
}

//void TestKBQuality::comProductGraph(DIGRAPHBASIC *G1, DIGRAPHBASIC *G2,
//                                    DIPRODUCTGRAPH *GP) {
//  // G1 \times G2
//
//}

void TestKBQuality::comSim(DIKEYS* Q, DIGRAPHBASIC* G,
                           unordered_set<VertexID>& vIndG) {
  MapIntHset simset;
  GraphSimCal gsc;
  gsc.SimCal(G, Q, simset);

// check the simulation result
// for each node in Q,
// it has mapped nodes in Gp
  MapIntHset::iterator it1;
  DIKEYS::VLabels::iterator qit;
  bool flag = true;
  for (qit = Q->getVLabel().begin(); qit != Q->getVLabel().end(); qit++) {
    VertexID qv = qit->first;
    if (simset[qv].size() == 0) {
      flag = false;
      break;
    }
  }

  if (!flag) {
    // Q is not simulate to Gp
    return;
  } else {
    // Q is simulate to Gp
    for (it1 = simset.begin(); it1 != simset.end(); it1++) {
      for (unordered_set<VertexID>::iterator it2 = it1->second.begin();
          it2 != it1->second.end(); it2++) {
        VertexID v = *it2;
        vIndG.insert(v);
      }
    }
  }

//  cout << "simulation result of Q: " << endl;
//  for (MapIntHset::iterator it1 = simset.begin(); it1 != simset.end(); it1++) {
//    int u = it1->first;
//    cout << u << "-> ";
//    for (unordered_set<int>::iterator it2 = it1->second.begin();
//        it2 != it1->second.end(); it2++) {
//      int v = *it2;
//      cout << v << ", ";
//    }
//    cout << endl;
//  }
}

// not used
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

void TestKBQuality::comProductGraph(DIGRAPHBASIC *G, VertexID e1,
                                    unordered_set<VertexID> &G1v, VertexID e2,
                                    unordered_set<VertexID> &G2v,
                                    DIPRODUCTGRAPH *GP) {
// G1 \times G2
// no need to explicitly construct G1 and G2,
// as appose to, use only V(G1) and V(G2)

// insert (e1, e2)
  VertexID e1e2 = hashPair(e1, e2);
  GP->insertVertex(e1e2, VertexTriple(e1, e2, G->getVLabel(e1)));
  GP->e = e1e2;

// insert node
  for (unordered_set<VertexID>::iterator itv1 = G1v.begin(); itv1 != G1v.end();
      itv1++) {
    VertexID u = *itv1;
    // no need for e1 and e2
    if (u == e1 || u == e2)
      continue;

    for (unordered_set<VertexID>::iterator itv2 = G2v.begin();
        itv2 != G2v.end(); itv2++) {
      VertexID v = *itv2;

      // no need for e1 and e2
      if (v == e1 || v == e2)
        continue;

      // u and v are with same type or same value
      if (G->getVLabel(u) != G->getVLabel(v)) {
        continue;
      }

      if (u < 0) {
        GP->insertVertex(u, VertexTriple(u, v, G->getVLabel(u)));
      } else {
        GP->insertVertex(hashPair(u, v), VertexTriple(u, v, G->getVLabel(u)));
      }
    }
  }  // end of node

  // insert edge
  for (DIPRODUCTGRAPH::VLabels::iterator vit1 = GP->getVLabel().begin();
      vit1 != GP->getVLabel().end(); vit1++) {
    VertexID u1u2 = vit1->first;
    VertexID u1 = vit1->second.first;
    VertexID u2 = vit1->second.second;
    VertexLabel u1u2l = vit1->second.third;

    if (u1u2l < 0) {
      continue;
    }

    // for each (u1, v1)
    for (DIGRAPHBASIC::AdjList::iterator git1 = G->getOutEdge()[u1].begin();
        git1 != G->getOutEdge()[u1].end(); git1++) {
      VertexID v1 = git1->first;

      // for each (u2, v2)
      for (DIGRAPHBASIC::AdjList::iterator git2 = G->getOutEdge()[u2].begin();
          git2 != G->getOutEdge()[u2].end(); git2++) {
        VertexID v2 = git2->first;

        if (G->getELabel(u1, v1) != G->getELabel(u2, v2)) {
          continue;
        }
        if (G->getVLabel(v1) != G->getVLabel(v2)) {
          continue;
        }

        if (v1 < 0) {
          if (GP->isVertex(v1)) {
            GP->insertEdge(u1u2, v1,
                           EdgeTriple(G->getELabel(u1, v1), (NODEP), (NOTC)));
          }
        } else {
          VertexID v1v2 = hashPair(v1, v2);
          if (GP->isVertex(v1v2)) {
            GP->insertEdge(u1u2, v1v2,
                           EdgeTriple(G->getELabel(u1, v1), (NODEP), (NOTC)));
          }
        }
      }  // end v2
    }  // end v1
  }  // end of edge

  return;
//// insert edge
//  for (DIPRODUCTGRAPH::VLabels::iterator vit1 = GP->getVLabel().begin();
//      vit1 != GP->getVLabel().end(); vit1++) {
//    VertexID u1 = vit1->second.first;
//    VertexID u2 = vit1->second.second;
//
//    for (DIPRODUCTGRAPH::VLabels::iterator vit2 = GP->getVLabel().begin();
//        vit2 != GP->getVLabel().end(); vit2++) {
//      VertexID v1 = vit2->second.first;
//      VertexID v2 = vit2->second.second;
//
//      // not the same nodes
//      if (vit1->first == vit2->first)
//        continue;
//
//      if (!G->isEdge(u1, v1) || !G->isEdge(u2, v2))
//        continue;
//
//      // check edge predicate
//      if (G->getELabel(u1, v1) != G->getELabel(u2, v2))
//        continue;
//
//      // insert edge
//      GP->insertEdge(vit1->first, vit2->first,
//                     EdgeTriple(G->getELabel(u1, v1), (NODEP), (NOTC)));
//    }
//  }  // end of edge
}

void TestKBQuality::pairing() {
// pairing and
// output L and Gp

// for each graph
  DIPRODUCTGRAPH *Gp = new DIPRODUCTGRAPH();
  unordered_set<VertexID> vGp;

// for each graph G
  for (int i = 0; i < g_cnt; i++) {
    DIGRAPHBASIC *G = graphDB[i];

    // for each (e_1, e_2) with same type
    // e1
    for (unordered_set<VertexID>::iterator eit1 = E.begin(); eit1 != E.end();
        eit1++) {
      VertexID e_1 = *eit1;
      unordered_set<VertexID>* vG_1 = GdVDB[i][e_1];

      // e2
      for (unordered_set<VertexID>::iterator eit2 = eit1; eit2 != E.end();
          eit2++) {
        VertexID e_2 = *eit2;
        unordered_set<VertexID>* vG_2 = GdVDB[i][e_2];

        // e_1 >= e_2 and
        // they are same type
        if (e_1 >= e_2 || G->getVLabel(e_1) != G->getVLabel(e_2)) {
          continue;
        }

        // compute Gp
//        cout << "(e1, e2): " << e_1 << ", " << e_2 << endl;
        comProductGraph(G, e_1, *vG_1, e_2, *vG_2, Gp);

//        cout << "product graph Gp: " << endl;
//        Gp->printGraph(cout);

        // check by Q
        for (int j = 0; j < q_cnt; j++) {
          DIKEYS *Q = queryDB[j];
//          cout << "key Q: " << j << endl;
//          Q->printGraph(cout);

          // check simulation between Q and Gp
          comSim(Q, Gp, vGp);
        }  // end of Q

//        cout << "after sim on Q and Gp" << endl;

        if (!vGp.empty()) {
//          cout << "(e1, e2): " << e_1 << ", " << e_2 << endl;
          L.push_back(Pair(e_1, e_2));
          hash_L.insert(hashPair(e_1, e_2));
          thetaL++;
          if (thetaL > DEFAULTHETAL) {
            cout << "out of L" << endl;
            exit(0);
          }
//          cout << "vGp size: " << vGp.size() << endl;
          // 1. output L
          // for MR
          // 2. union to global product graph
          // for VC
          collectLandGp(Gp, vGp, G);
//          cout << "after collect L and Gp" << endl;
        }
        // clean Gp and vGp
        Gp->reset();
        vGp.clear();

      }  // end of e_2
    }  // end of e_1

    cout << "printing L: " << endl;
    printLChao(m_LOutputStream, G);

    // update TC info on Gp
    collectTC(G);
    // output L and Gp
    // using file output
    cout << "printing global Gp: " << endl;
    printGpChao(m_GpOutputStream);
    //  global_Gp->printGraph(cout);

    cout << "printing org L: " << endl;
    printOrgLChao(G);
  }  // end of G

  delete Gp;
}

void TestKBQuality::collectTC(DIGRAPHBASIC* G) {
  DIPRODUCTGRAPH::VLabels::iterator it = global_Gp->getVLabel().begin();
  for (; it != global_Gp->getVLabel().end(); it++) {
    VertexID u = it->first;
    VertexID v1 = it->second.first;
    VertexID v2 = it->second.second;
    // no value node
    if (u < 0)
      continue;
    GpTCIndx[v1].insert(u);
    GpTCIndx[v2].insert(u);
  }

  unordered_map<VertexID, unordered_set<VertexID> >::iterator it1 = GpTCIndx
      .begin();
  unordered_set<VertexID>::iterator it2;
  for (; it1 != GpTCIndx.end(); it1++) {
    VertexID e = it1->first;
    VertexID ee = hashPair(e, e);
    // update node
    if (!global_Gp->isVertex(ee)) {
      global_Gp->insertVertex(ee, VertexTriple(e, e, G->getVLabel(e)));
    }

    // update tc edge
    for (it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
      VertexID e1e2 = *it2;

      if (e1e2 == ee)
        continue;
      if (global_Gp->isEdge(e1e2, ee)) {
        global_Gp->getELabel(e1e2, ee).third = -1;
      } else {
        global_Gp->insertEdge(e1e2, ee, EdgeTriple(NOEDGELABEL, NODEP, -1));
      }
    }
  }
  GpTCIndx.clear();
}

void TestKBQuality::collectLandGp(DIPRODUCTGRAPH *Gp,
                                  unordered_set<VertexID>& vGp,
                                  DIGRAPHBASIC* G) {
//  DIGRAPHBASIC G1, G2;

// collect y
// for dependency
  unordered_set<VertexLabel> ys;
  for (int j = 0; j < q_cnt; j++) {
    DIKEYS *Q = queryDB[j];
    for (DIKEYS::VLabels::iterator it = Q->getVLabel().begin();
        it != Q->getVLabel().end(); it++) {
      if (it->second.u == 2) {
        ys.insert(it->second.v);
      }
    }
  }  // end of Q

  VertexID e1 = Gp->getVLabel(Gp->e).first;
  VertexID e2 = Gp->getVLabel(Gp->e).second;
  VertexID e1e2 = hashPair(e1, e2);

  if (!global_Gp->isVertex(e1e2)) {
    global_Gp->insertVertex(e1e2, Gp->getVLabel(Gp->e));
  }

  DIPRODUCTGRAPH::VLabels::iterator iter1 = Gp->getVLabel().begin();
  for (; iter1 != Gp->getVLabel().end(); iter1++) {
    VertexID u = iter1->first;
    VertexID u1 = iter1->second.first;
    VertexID u2 = iter1->second.second;
    VertexID u12l = iter1->second.third;

// u is not in the simulation result
    if (vGp.find(u) == vGp.end()) {
      continue;
    }

// insert G1 and G2 nodes
//    G1.insertVertex(u1, u12l);
//    G2.insertVertex(u2, u12l);

// insert Gp nodes
    VertexID upg;
    if (u < 0) {
      upg = u;
    } else {
      upg = hashPair(u1, u2);
    }
    if (!global_Gp->isVertex(upg)) {
      global_Gp->insertVertex(upg, Gp->getVLabel(u));
    }
// dependency
// -1 -> dep
    if (ys.find(u12l) != ys.end() && upg != e1e2) {
      if (global_Gp->isEdge(upg, e1e2)) {
        global_Gp->getELabel(upg, e1e2).second = -1;
      } else {
        global_Gp->insertEdge(upg, e1e2, EdgeTriple(NOEDGELABEL, -1, NOTC));
      }
    }
//    cout << "dependency" << endl;

    DIPRODUCTGRAPH::AdjList::iterator iter2 = Gp->getOutEdge()[u].begin();
    for (; iter2 != Gp->getOutEdge()[u].end(); iter2++) {
      VertexID v = iter2->first;
      VertexID v1 = Gp->getVLabel(v).first;
      VertexID v2 = Gp->getVLabel(v).second;
      VertexID v12l = Gp->getVLabel(v).third;
      EdgeLabel el = Gp->getELabel(u, v).first;

      // v is not in the simulation result
      if (vGp.find(v) == vGp.end()) {
        continue;
      }

      // insert G1 and G2
//      G1.insertVertex(v1, v12l);
//      G2.insertVertex(v2, v12l);

//      if (!G1.isEdge(u1, v1)) {
//        G1.insertEdge(u1, v1, el);
//      }
//      if (!G2.isEdge(u2, v2)) {
//        G2.insertEdge(u2, v2, el);
//      }

      // insert Gp
      VertexID vpg;
      if (v < 0) {
        vpg = v;
      } else {
        vpg = hashPair(v1, v2);
      }

      if (upg == vpg)
        continue;

      if (!global_Gp->isVertex(vpg)) {
        global_Gp->insertVertex(vpg, Gp->getVLabel(v));
      }

      if (global_Gp->isEdge(upg, vpg)) {
        global_Gp->getELabel(upg, vpg).first = el;
      } else {
        global_Gp->insertEdge(upg, vpg, EdgeTriple(el, NODEP, NOTC));
      }
    }
  }

// output G1 and G2
// use file output
//  cout << "G1: " << endl;
//  G1.printGraph(cout);
//  cout << "G2: " << endl;
//  G2.printGraph(cout);
}

VertexID TestKBQuality::hashPair(VertexID a, VertexID b) {
  return a >= b ? a * a + a + b : a + b * b;
}

void TestKBQuality::comSim(DIKEYS* Q, DIPRODUCTGRAPH *Gp,
                           unordered_set<VertexID>& vGp) {
  MapIntHset simset;
  ProductGraphSimCal pgsc;
  pgsc.SimCal(Gp, Q, simset);

// check the simulation result
// for each node in Q,
// it has mapped nodes in Gp
  MapIntHset::iterator it1;
  DIKEYS::VLabels::iterator qit;
  bool flag = true;
  for (qit = Q->getVLabel().begin(); qit != Q->getVLabel().end(); qit++) {
    VertexID qv = qit->first;
    if (simset[qv].size() == 0) {
      flag = false;
      break;
    }
  }

  if (!flag) {
// Q is not simulate to Gp
    return;
  } else {
// Q is simulate to Gp
    for (it1 = simset.begin(); it1 != simset.end(); it1++) {
      for (unordered_set<VertexID>::iterator it2 = it1->second.begin();
          it2 != it1->second.end(); it2++) {
        VertexID v = *it2;
        vGp.insert(v);
      }
    }
  }

//  cout << "simulation result of Q" << ": " << endl;
//  for (it1 = simset.begin(); it1 != simset.end(); it1++) {
//    int u = it1->first;
//    cout << u << "-> ";
//    for (unordered_set<int>::iterator it2 = it1->second.begin();
//        it2 != it1->second.end(); it2++) {
//      int v = *it2;
//      cout << v << ", ";
//    }
//    cout << endl;
}

