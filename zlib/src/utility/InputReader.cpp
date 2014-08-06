#include "InputReader.h"

#include <string.h>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <vector>
#include <sstream>

//#include "graph.h"
//#include "GlobalDefinition.h"

using namespace std;

InputReader::InputReader(const char* input_file_name) {
//	strcpy_s(m_FileName, FILENAME_BUFF_SIZE, input_file_name);
  strcpy(m_FileName, input_file_name);

  m_InputStream.open(m_FileName);

  if (!m_InputStream.is_open()) {
    cout << "input file " << input_file_name << " doesn't exist." << endl;
    exit(1);
  }

  ASSERT(m_InputStream.is_open());

  ReadFirstLine();
}

InputReader::InputReader(const char* input_file_name, const char* input_type) {
//  strcpy_s(m_FileName, FILENAME_BUFF_SIZE, input_file_name);
  strcpy(m_FileName, input_file_name);

  m_InputStream.open(m_FileName);

  if (!m_InputStream.is_open()) {
    cout << "input file " << input_file_name << " doesn't exist." << endl;
    exit(1);
  }

  ASSERT(m_InputStream.is_open());
}

InputReader::InputReader(const char* input_g_name, const char* input_vl_name,
                         const char* input_el_name) {
  m_InputStream.open(input_g_name);
  m_VLInputStream.open(input_vl_name);
  m_ELInputStream.open(input_el_name);

  if (!m_InputStream.is_open()) {
    cout << "input file " << input_g_name << " doesn't exist." << endl;
    exit(1);
  }

  if (!m_VLInputStream.is_open()) {
    cout << "input file " << input_vl_name << " doesn't exist." << endl;
    exit(1);
  }

  if (!m_ELInputStream.is_open()) {
    cout << "input file " << input_el_name << " doesn't exist." << endl;
    exit(1);
  }

  ASSERT(m_InputStream.is_open());
  ASSERT(m_VLInputStream.is_open());
  ASSERT(m_ELInputStream.is_open());
}

InputReader::~InputReader() {
  m_InputStream.close();
}

void InputReader::EndOfFile_Original(GRAPH& graph, vector<RawVertex>& v_list,
                                     vector<RawEdge>& e_list) {

  graph.setV((int) v_list.size());
  graph.setGraphId(m_CurGraphID);

  for (vector<RawVertex>::iterator it = v_list.begin(); it != v_list.end();
      it++)
    graph.setLabel(it->id, it->label);

  EdgeID e_id = 0;
  for (vector<RawEdge>::iterator it = e_list.begin(); it != e_list.end(); it++)
    graph.insert(Edge(e_id++, it->u, it->v, it->label));

  m_InputStream >> m_LineHeader;
  m_InputStream >> m_CurGraphID;

  ASSERT(graph.E() == (int )e_list.size());
  ASSERT(graph.V() == (int )v_list.size());
}

void InputReader::EndOfFile(GRAPH& graph, vector<RawVertex>& v_list,
                            vector<RawEdge>& e_list,
                            map<VertexLabel, int>& label_count_map) {
  map<VertexID, VertexID> id_reorder_map;

  graph.setV((int) v_list.size());
  graph.setGraphId(m_CurGraphID);

  for (vector<RawVertex>::iterator it = v_list.begin(); it != v_list.end();
      it++) {
    it->label_occurence = label_count_map[it->label];
  }
  std::sort(v_list.begin(), v_list.end());
  int new_id = 0;
  for (vector<RawVertex>::iterator it = v_list.begin(); it != v_list.end();
      it++) {
    it->label_occurence = label_count_map[it->label];

    id_reorder_map[it->id] = new_id;
    it->id = new_id++;
  }

  for (vector<RawEdge>::iterator it = e_list.begin(); it != e_list.end();
      it++) {
    it->u = id_reorder_map[it->u];
    it->v = id_reorder_map[it->v];

    if (it->u > it->v) {
      VertexID tmp = it->u;
      it->u = it->v;
      it->v = tmp;
    }
  }
  std::sort(e_list.begin(), e_list.end());

  for (vector<RawVertex>::iterator it = v_list.begin(); it != v_list.end();
      it++)
    graph.setLabel(it->id, it->label);

  EdgeID e_id = 0;
  for (vector<RawEdge>::iterator it = e_list.begin(); it != e_list.end(); it++)
    graph.insert(Edge(e_id++, it->u, it->v, it->label));

  m_InputStream >> m_LineHeader;
  m_InputStream >> m_CurGraphID;

  ASSERT(graph.E() == (int )e_list.size());
  ASSERT(graph.V() == (int )v_list.size());
}

void InputReader::_New_DiGraph_Original(
    DIGRAPHBASIC& diGraph, map<VertexID, VertexLabel>& _vLabels,
    map<EdgeID, EdgeLabel>& _eLabels, map<VertexID, set<Triple> >& _adj_list) {
  // initialize size of diGraph
//  diGraph.resetVcnt((int) _adj_list.size());

//  vertices
  for (map<VertexID, VertexLabel>::iterator it = _vLabels.begin();
      it != _vLabels.end(); it++) {
    diGraph.insertVertex(it->first, it->second);
  }

  // edges
  int e_id = 1;
  for (map<VertexID, set<Triple> >::iterator it = _adj_list.begin();
      it != _adj_list.end(); it++) {
    for (set<Triple>::iterator it1 = it->second.begin();
        it1 != it->second.end(); it1++) {
      const Triple& tri = *it1;
      diGraph.insertEdge(tri.o, tri.d, _eLabels[tri.e]);
    }
  }
}

void InputReader::_New_Graph_Original(GRAPH& graph, vector<RawVertex>& v_list,
                                      vector<RawEdge>& e_list) {
  graph.setV((int) v_list.size());
  graph.setGraphId(m_CurGraphID);

  for (vector<RawVertex>::iterator it = v_list.begin(); it != v_list.end();
      it++) {
    graph.setLabel(it->id, it->label);
  }

  int e_id = 0;
  for (vector<RawEdge>::iterator it = e_list.begin(); it != e_list.end();
      it++) {
    graph.insert(Edge(e_id++, it->u, it->v, it->label));
  }

  m_InputStream >> m_LineHeader;
  m_InputStream >> m_CurGraphID;

  ASSERT(graph.E() == (int )e_list.size());
  ASSERT(graph.V() == (int )v_list.size());
}

void InputReader::_New_Graph(GRAPH& graph, vector<RawVertex>& v_list,
                             vector<RawEdge>& e_list,
                             map<VertexLabel, int>& label_count_map) {
  map<VertexID, VertexID> id_reorder_map;

  graph.setV((int) v_list.size());
  graph.setGraphId(m_CurGraphID);

  for (vector<RawVertex>::iterator it = v_list.begin(); it != v_list.end();
      it++) {
    it->label_occurence = label_count_map[it->label];
  }
  std::sort(v_list.begin(), v_list.end());
  int new_id = 0;
  for (vector<RawVertex>::iterator it = v_list.begin(); it != v_list.end();
      it++) {
    it->label_occurence = label_count_map[it->label];

    id_reorder_map[it->id] = new_id;
    it->id = new_id++;
  }

  for (vector<RawEdge>::iterator it = e_list.begin(); it != e_list.end();
      it++) {
    it->u = id_reorder_map[it->u];
    it->v = id_reorder_map[it->v];

    if (it->u > it->v) {
      VertexID tmp = it->u;
      it->u = it->v;
      it->v = tmp;
    }
  }
  std::sort(e_list.begin(), e_list.end());

  for (vector<RawVertex>::iterator it = v_list.begin(); it != v_list.end();
      it++) {
    graph.setLabel(it->id, it->label);
  }

  int e_id = 0;
  for (vector<RawEdge>::iterator it = e_list.begin(); it != e_list.end();
      it++) {
    graph.insert(Edge(e_id++, it->u, it->v, it->label));
  }

  m_InputStream >> m_LineHeader;
  m_InputStream >> m_CurGraphID;

  ASSERT(graph.E() == (int )e_list.size());
  ASSERT(graph.V() == (int )v_list.size());
}

void InputReader::GetKBDiGraph(DIGRAPHBASIC& diGraph) {
  // read vertex label
  VertexID vid;
  VertexLabel vl;
  map<VertexID, VertexLabel> vl_map;  // org

  while (!m_VLInputStream.eof()) {
    m_VLInputStream >> vid >> vl;
    vl_map[vid] = vl;
  }

  // read edge label
  EdgeID eid;
  EdgeLabel el;
  map<EdgeID, EdgeLabel> el_map;
  while (!m_ELInputStream.eof()) {
    m_ELInputStream >> eid >> el;
    el_map[eid] = el;
  }

  // read linkage
  map<VertexID, set<Triple> > _adj_list;
  VertexID s, d, e;
  VertexLabel sl, dl;

  while (!m_InputStream.eof()) {
    m_InputStream >> s >> e >> d;
    _adj_list[s].insert(Triple(s, e, d));
  }

  // to graph
  _New_DiGraph_Original(diGraph, vl_map, el_map, _adj_list);
}

void InputReader::GetSnapDiGraph(DIGRAPHBASIC& diGraph) {
  map<VertexID, VertexID> v_map;
  map<VertexID, set<VertexID> > org_adj_list;
  map<VertexID, set<VertexID> > new_adj_list;
  VertexID org_src, org_dest, org_edge;
  VertexID new_src, new_dest, new_edge;
  VertexID id = 0;

  while (!m_InputStream.eof()) {
    m_InputStream >> org_src >> org_dest;

    if (v_map.find(org_src) == v_map.end()) {
      new_src = id;
      v_map[org_src] = id;
      id++;
    } else {
      new_src = v_map[org_src];
    }

    if (v_map.find(org_dest) == v_map.end()) {
      new_dest = id;
      v_map[org_dest] = id;
      id++;
    } else {
      new_dest = v_map[org_dest];
    }
    org_adj_list[org_src].insert(org_dest);
    new_adj_list[new_src].insert(new_dest);
  }

// to graph

}

void InputReader::GetSnapGraph(GRAPH& graph) {
  map<VertexID, VertexID> v_map;
  map<VertexID, set<VertexID> > org_adj_list;
  map<VertexID, set<VertexID> > new_adj_list;
  VertexID org_src, org_dest;
  VertexID new_src, new_dest;
  VertexID new_id = 0;

// read file
  while (!m_InputStream.eof()) {
    m_InputStream >> org_src >> org_dest;

    if (v_map.find(org_src) == v_map.end()) {
      new_src = new_id;
      v_map[org_src] = new_id;
      new_id++;
    } else {
      new_src = v_map[org_src];
    }

    if (v_map.find(org_dest) == v_map.end()) {
      new_dest = new_id;
      v_map[org_dest] = new_id;
      new_id++;
    } else {
      new_dest = v_map[org_dest];
    }
    new_adj_list[new_src].insert(new_dest);
    new_adj_list[new_dest].insert(new_src);
  }

  cout << new_adj_list.size() << endl;

// to graph
  vector<RawVertex> v_list;
  vector<RawEdge> e_list;
  map<VertexLabel, int> label_count_map;

// vertices and edges
  for (map<VertexID, set<VertexID> >::iterator it = new_adj_list.begin();
      it != new_adj_list.end(); it++) {
    RawVertex src;
    src.id = it->first;
    src.label = it->second.size();
    v_list.push_back(src);
    label_count_map[src.label]++;

    for (set<VertexID>::iterator it1 = it->second.begin();
        it1 != it->second.end(); it1++) {
      if (src.id >= *it1)
        continue;
      RawEdge edge;

      // TODO set all edge label as the same
      edge.label = 0;
      edge.u = src.id;
      edge.v = *it1;
      e_list.push_back(edge);
    }
  }

  cout << "v: " << v_list.size() << " e: " << e_list.size() << endl;

  _New_Graph(graph, v_list, e_list, label_count_map);
}

bool InputReader::GetNextGraph_MultiVertexLabel(GRAPH& graph) {
  vector<RawVertex> v_list;
  vector<RawEdge> e_list;
  map<VertexLabel, int> label_count_map;
  map<VertexID, vector<VertexID> > original_new_map;

  VertexID _v_id = 0;
  VertexLabel _v_l = 0;

  while (1) {
    m_InputStream >> m_LineHeader;

    if (m_CurGraphID < 0) {
      return false;
    }

    if (m_InputStream.eof()) {
      EndOfFile(graph, v_list, e_list, label_count_map);
      m_CurGraphID = -1;

      return true;
    }

    if (m_LineHeader[0] == H_GRAPH_CH) {
      _New_Graph(graph, v_list, e_list, label_count_map);

      return true;
    } else if (m_LineHeader[0] == H_VERTEX_CH) {
      RawVertex vertex;
      string line;
      getline(m_InputStream, line);
      istringstream iss(line);
      VertexID _t;
      iss >> _t;

      while (iss >> _v_l) {
        vertex.id = _v_id++;
        vertex.label = _v_l;
        v_list.push_back(vertex);

        label_count_map[vertex.label]++;
        original_new_map[_t].push_back(vertex.id);
      }
    } else if (m_LineHeader[0] == H_EDGE_CH) {
      VertexID _u, _v, _n_u, _n_v;
      RawEdge edge;
      m_InputStream >> _u;
      m_InputStream >> _v;
      m_InputStream >> edge.label;

      // TODO set all edge label as the same
      edge.label = 0;

      for (int i = 0; i < original_new_map[_u].size(); i++) {
        _n_u = original_new_map[_u][i];
        for (int j = 0; j < original_new_map[_v].size(); j++) {
          _n_v = original_new_map[_v][j];
          edge.u = _n_u;
          edge.v = _n_v;
          e_list.push_back(edge);
        }
      }
    } else {
      cout << "format error" << endl;
      ASSERT(0);
    }
  }
  return true;
}

bool InputReader::GetNextGraph_MultiVertexLabel_Original(GRAPH& graph) {
  vector<RawVertex> v_list;
  vector<RawEdge> e_list;
  map<VertexLabel, int> label_count_map;
  map<VertexID, vector<VertexID> > original_new_map;

  VertexID _v_id = 0;
  VertexLabel _v_l = 0;

  while (1) {
    m_InputStream >> m_LineHeader;

    if (m_CurGraphID < 0) {
      return false;
    }

    if (m_InputStream.eof()) {
      EndOfFile_Original(graph, v_list, e_list);
      m_CurGraphID = -1;

      return true;
    }

    if (m_LineHeader[0] == H_GRAPH_CH) {
      _New_Graph_Original(graph, v_list, e_list);

      return true;
    } else if (m_LineHeader[0] == H_VERTEX_CH) {
      RawVertex vertex;
      string line;
      getline(m_InputStream, line);
      istringstream iss(line);
      VertexID _t;
      iss >> _t;

      while (iss >> _v_l) {
        vertex.id = _v_id++;
        vertex.label = _v_l;
        v_list.push_back(vertex);

        label_count_map[vertex.label]++;
        original_new_map[_t].push_back(vertex.id);
      }
    } else if (m_LineHeader[0] == H_EDGE_CH) {
      VertexID _u, _v, _n_u, _n_v;
      RawEdge edge;
      m_InputStream >> _u;
      m_InputStream >> _v;
      m_InputStream >> edge.label;

      // TODO set all edge label as the same
      edge.label = 0;

      for (int i = 0; i < original_new_map[_u].size(); i++) {
        _n_u = original_new_map[_u][i];
        for (int j = 0; j < original_new_map[_v].size(); j++) {
          _n_v = original_new_map[_v][j];
          edge.u = _n_u;
          edge.v = _n_v;
          e_list.push_back(edge);
        }
      }
    } else {
      cout << "format error" << endl;
      ASSERT(0);
    }
  }
  return true;
}

bool InputReader::GetNextGraph(GRAPH& graph) {
  vector<RawVertex> v_list;
  vector<RawEdge> e_list;
  map<VertexLabel, int> label_count_map;
  map<VertexID, VertexID> id_reorder_map;

  while (1) {
    m_InputStream >> m_LineHeader;

    if (m_CurGraphID < 0)
      return false;

    if (m_InputStream.eof()) {
      EndOfFile(graph, v_list, e_list, label_count_map);
      m_CurGraphID = -1;
      return true;
    }

    if (m_LineHeader[0] == H_GRAPH_CH) {
      _New_Graph(graph, v_list, e_list, label_count_map);
      return true;
    } else if (m_LineHeader[0] == H_VERTEX_CH) {
      RawVertex vertex;
      m_InputStream >> vertex.id;
      m_InputStream >> vertex.label;

      v_list.push_back(vertex);

      label_count_map[vertex.label]++;
    } else if (m_LineHeader[0] == H_EDGE_CH) {
      RawEdge edge;
      m_InputStream >> edge.u;
      m_InputStream >> edge.v;
      m_InputStream >> edge.label;

      e_list.push_back(edge);
    } else {
      cout << "format error" << endl;
      ASSERT(0);
    }
  }

  return true;
}

bool InputReader::GetNextGraph_Original(GRAPH& graph) {
  vector<RawVertex> v_list;
  vector<RawEdge> e_list;
  while (1) {
    m_InputStream >> m_LineHeader;

    if (m_CurGraphID < 0)
      return false;

    if (m_InputStream.eof()) {
      EndOfFile_Original(graph, v_list, e_list);
      m_CurGraphID = -1;
      return true;
    }

    if (m_LineHeader[0] == H_GRAPH_CH) {
      _New_Graph_Original(graph, v_list, e_list);
      return true;
    } else if (m_LineHeader[0] == H_VERTEX_CH) {
      RawVertex vertex;
      m_InputStream >> vertex.id;
      m_InputStream >> vertex.label;

      v_list.push_back(vertex);
    } else if (m_LineHeader[0] == H_EDGE_CH) {
      RawEdge edge;
      m_InputStream >> edge.u;
      m_InputStream >> edge.v;
      m_InputStream >> edge.label;

      e_list.push_back(edge);
    } else {
      cout << "format error" << endl;
      ASSERT(0);
    }
  }

  return true;
}

void InputReader::ReadFirstLine() {
  m_InputStream >> m_LineHeader;
  ASSERT(m_LineHeader[0] == H_GRAPH_CH);

  m_InputStream >> m_LineHeader;
  ASSERT(m_LineHeader[0] == '#');

  m_InputStream >> m_CurGraphID;
}

