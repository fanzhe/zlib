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

