#ifndef INPUTREADER_H
#define INPUTREADER_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <fstream>
#include "GlobalDefinition.h"
#include "graph.h"
#include "DiGraph.h"

using namespace std;

#define H_GRAPH_STR "t"
#define H_VERTEX_STR "v"
#define H_EDGE_STR "e"

#define H_GRAPH_CH 't'
#define H_VERTEX_CH 'v'
#define H_EDGE_CH 'e'

#define H_VERTEX_STR_2 "v"
#define H_EDGE_STR_2 "u"

#define H_VERTEX_CH_2 'v'
#define H_EDGE_CH_2 'u'

#define INPUT_FORMAT_1 1

typedef struct RawVertex {
  VertexID id;
  VertexLabel label;
  int label_occurence;

  bool operator <(const RawVertex& raw_vertex) const {
    if (label_occurence != raw_vertex.label_occurence)
      return label_occurence < raw_vertex.label_occurence;
    else
      return label > raw_vertex.label;
  }
} RawVertex;

typedef struct RawEdge {
  VertexID v;
  VertexID u;
  EdgeLabel label;

  bool operator <(const RawEdge& raw_edge) const {
    if (u != raw_edge.u)
      return u < raw_edge.u;
    else
      return v < raw_edge.v;
  }
} RawEdge;

typedef struct Triple {
  VertexID o;
  VertexID d;
  EdgeID e;
  Triple(VertexID _o, EdgeID _e, VertexID _d)
      : o(_o),
        e(_e),
        d(_d) {
  }

  bool operator ==(const Triple& tri) const {
    if (o == tri.o && d == tri.d) {
      return true;
    } else {
      return false;
    }
  }

  bool operator <(const Triple& tri) const {
    if (o != tri.o) {
      return o < tri.o;
    } else {
      return d < tri.d;
    }
  }
} Triple;

class InputReader {
 public:
  InputReader(const char* input_file_name);
  InputReader(const char* input_file_name, const char* input_type);
  InputReader(const char* input_g_name, const char* input_vl_name,
              const char* input_el_name);
  ~InputReader();

  void _New_DiGraph_Original(DIGRAPHBASIC& diGraph,
                             map<VertexID, VertexLabel>& _vLabels,
                             map<EdgeID, EdgeLabel>& _eLabels,
                             map<VertexID, set<Triple> >& _org_adj_list);
  void _New_Graph_Original(GRAPH& graph, vector<RawVertex>& v_list,
                           vector<RawEdge>& e_list);
  void _New_Graph(GRAPH& graph, vector<RawVertex>& v_list,
                  vector<RawEdge>& e_list,
                  map<VertexLabel, int>& label_count_map);
  void EndOfFile(GRAPH& graph, vector<RawVertex>& v_list,
                 vector<RawEdge>& e_list,
                 map<VertexLabel, int>& label_count_map);
  void EndOfFile_Original(GRAPH& graph, vector<RawVertex>& v_list,
                          vector<RawEdge>& e_list);
  bool GetNextGraph(GRAPH& graph);
  bool GetNextGraph_Original(GRAPH& graph);
  bool GetNextGraph_MultiVertexLabel(GRAPH& graph);
  void GetSnapGraph(GRAPH& graph);
  void GetSnapDiGraph(DIGRAPHBASIC& diGraph);
  void GetKBDiGraph(DIGRAPHBASIC& diGraph);
  bool GetNextGraph_MultiVertexLabel_Original(GRAPH& graph);

 public:
  char m_FileName[256];
  // for graph
  ifstream m_InputStream;

  // for KB v and e
  ifstream m_VLInputStream;
  ifstream m_ELInputStream;

  char m_LineHeader[32];
  GraphID m_CurGraphID;
  void ReadFirstLine();
};

#endif
