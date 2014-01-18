#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <map>

#include "graph.h"
#include "InputReader.h"
#include "GlobalDefinition.h"

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
      for (vector<RawEdge>::iterator it = e_list.begin(); it != e_list.end();
          it++)
        graph.insert(Edge(e_id++, it->u, it->v, it->label));

      m_InputStream >> m_LineHeader;
      m_InputStream >> m_CurGraphID;

      ASSERT(graph.E() == (int )e_list.size());
      ASSERT(graph.V() == (int )v_list.size());

      m_CurGraphID = -1;

      return true;
    }

    if (m_LineHeader[0] == H_GRAPH_CH) {
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
//        cout << "insert vertex: (" << it->id << ", " << it->label << ")" << endl;
        graph.setLabel(it->id, it->label);
      }

      int e_id = 0;
      for (vector<RawEdge>::iterator it = e_list.begin(); it != e_list.end();
          it++) {
//        cout << "insert edge: (" << it->u << ", " << it->v << ", " << it->label << ")" << endl;
        graph.insert(Edge(e_id++, it->u, it->v, it->label));
      }

      m_InputStream >> m_LineHeader;
      m_InputStream >> m_CurGraphID;

      ASSERT(graph.E() == (int )e_list.size());
      ASSERT(graph.V() == (int )v_list.size());

      return true;
    }

    else if (m_LineHeader[0] == H_VERTEX_CH) {
      RawVertex vertex;
      m_InputStream >> vertex.id;
      m_InputStream >> vertex.label;

      v_list.push_back(vertex);

      label_count_map[vertex.label]++;
    }

    else if (m_LineHeader[0] == H_EDGE_CH) {
      RawEdge edge;
      m_InputStream >> edge.u;
      m_InputStream >> edge.v;
      m_InputStream >> edge.label;

      e_list.push_back(edge);
    }

    else {
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
      graph.setV((int) v_list.size());
      graph.setGraphId(m_CurGraphID);

      for (vector<RawVertex>::iterator it = v_list.begin(); it != v_list.end();
          it++)
        graph.setLabel(it->id, it->label);

      EdgeID e_id = 0;
      for (vector<RawEdge>::iterator it = e_list.begin(); it != e_list.end();
          it++)
        graph.insert(Edge(e_id++, it->u, it->v, it->label));

      m_InputStream >> m_LineHeader;
      m_InputStream >> m_CurGraphID;

      ASSERT(graph.E() == (int )e_list.size());
      ASSERT(graph.V() == (int )v_list.size());

      m_CurGraphID = -1;

      return true;
    }

    if (m_LineHeader[0] == H_GRAPH_CH) {
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

      return true;
    }

    else if (m_LineHeader[0] == H_VERTEX_CH) {
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
    }

    else {
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


