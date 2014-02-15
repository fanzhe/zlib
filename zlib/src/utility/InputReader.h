#ifndef INPUTREADER_H
#define INPUTREADER_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <fstream>
#include "GlobalDefinition.h"
#include "graph.h"

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

class InputReader {
 public:
  InputReader(const char* input_file_name);
  ~InputReader();

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
  bool GetNextGraph_MultiVertexLabel_Original(GRAPH& graph);

 public:
  char m_FileName[256];
  ifstream m_InputStream;

  char m_LineHeader[32];
  GraphID m_CurGraphID;
  void ReadFirstLine();
};

#endif
