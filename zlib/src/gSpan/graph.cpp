#include "gspan.h"
#include <cstring>
#include <string>
#include <iterator>
#include <strstream>
#include <set>
#include <iostream>
#include <cstdio>

#include <assert.h>

namespace GSPAN {

template<class T, class Iterator>
void tokenize(const char *str, Iterator iterator) {
  std::istrstream is(str, std::strlen(str));
  std::copy(std::istream_iterator<T>(is), std::istream_iterator<T>(), iterator);
}

void Graph::buildEdge() {
  char buf[512];
  std::map<std::string, unsigned int> tmp;

  unsigned int id = 0;
  for (int from = 0; from < (int) size(); ++from) {
    for (Vertex::edge_iterator it = (*this)[from].edge.begin();
        it != (*this)[from].edge.end(); ++it) {
      if (directed || from <= it->to)
        std::sprintf(buf, "%d %d %d", from, it->to, it->elabel);
      else
        std::sprintf(buf, "%d %d %d", it->to, from, it->elabel);

      // Assign unique id's for the edges.
      if (tmp.find(buf) == tmp.end()) {
        it->id = id;
        tmp[buf] = id;
        ++id;
      } else {
        it->id = tmp[buf];
      }
    }
  }

  edge_size_ = id;
}

#if MATLAB
/* Convert a Matlab graph structure into the graph object. --sn
 */
void Graph::read (const mxArray* graph)
{
  mxArray* nodelabels = mxGetField (graph, 0, "nodelabels");
  mxArray* edges = mxGetField (graph, 0, "edges");

  if (nodelabels == NULL || edges == NULL) {
    mexPrintf ("Graph structures are missing the \"nodelabels\" "
        "or \"edges\" fields.\n");

    return;
  }

  if (mxIsUint32 (nodelabels) == 0 || mxIsUint32 (edges) == 0) {
    mexPrintf ("\"nodelabels\" or \"edges\" not of type Uint32.\n");

    return;
  }

  uint32_T* nmat = (uint32_T*) mxGetPr (nodelabels);
  assert (nmat != NULL);
  unsigned int nodes = mxGetM (nodelabels);
  this->resize (nodes);
  for (unsigned int i = 0; i < nodes; ++i) {
    (*this)[i].label = nmat[i];
#ifdef	DEBUG
    mexPrintf ("inserting node %d with label %d\n", i, nmat[i]);
#endif
  }

  /* emat is column organised.
   */
  unsigned int edgecount = mxGetM (edges);
  if (edgecount > 0) {
    uint32_T* emat = (uint32_T*) mxGetPr (edges);
#ifdef	DEBUG
    mexPrintf ("edges M, N: %d, %d\n", mxGetM (edges), mxGetN (edges));
#endif
    assert (emat != NULL);

    for (unsigned int i = 0; i < edgecount; ++i) {
      unsigned int from = emat[i+0*edgecount] - 1;	// index correction
      unsigned int to = emat[i+1*edgecount] - 1;// index correction
      unsigned int elabel = emat[i+2*edgecount];

      assert (from < nodes && to < nodes);
      if (from >= nodes || to >= nodes) {
        mexErrMsgTxt ("Edge from/to indices out of bounds.");
        return;
      }

      /* Warning: this makes edges undirected, which is not what we want (or
       * do we?).
       */
#ifdef	DEBUG
      mexPrintf ("inserting edge from %d to %d, label %d\n",
          from, to, elabel);
#endif
      (*this)[from].push (from, to, elabel);
      if (directed == false) {
#ifdef	DEBUG
        mexPrintf ("  reverse: inserting edge from %d to %d, label %d\n",
            to, from, elabel);
#endif
        (*this)[to].push (to, from, elabel);
      }
    }
  }
  buildEdge ();
}
#endif

#if MATLAB
void Graph::mexprint (void)
{
  unsigned int edgecount = 0;
  for (int from = 0; from < (int) size (); ++from) {
#ifdef	DEBUG
    mexPrintf ("   %d: %u\n", from+1, (*this)[from].label);
#endif
    for (Vertex::edge_iterator it = (*this)[from].edge.begin ();
        it != (*this)[from].edge.end (); ++it)
    {
      edgecount += 1;
    }
  }

  mexPrintf ("   graph (%d nodes, %d edges)\n", (int) size (),
      edgecount);

#ifdef	DEBUG
  mexPrintf ("edges\n");
  for (int from = 0; from < (int)size (); ++from)
  {
    for (Vertex::edge_iterator it = (*this)[from].edge.begin ();
        it != (*this)[from].edge.end (); ++it)
    {
      mexPrintf ("   %u %u %u\n", from+1, it->to+1, it->elabel);
    }
  }
#endif
}
#endif

#if MATLAB
mxArray* Graph::writemex (void)
{
  const char* fieldnames[] = {"nodelabels", "edges",};
  mxArray* gs = mxCreateStructMatrix (1, 1, 2, fieldnames);
  assert (gs != NULL);

  /* First do the node labels.
   */
  mxArray* nodelabels = mxCreateNumericMatrix (size (), 1, mxUINT32_CLASS, 0);
  assert (nodelabels != NULL);
  uint32_T* nodelabels_P = (uint32_T*) mxGetPr (nodelabels);
  assert (nodelabels_P != NULL);

  unsigned int edgecount = 0;
  for (int from = 0; from < (int) size (); ++from) {
    nodelabels_P[from] = (*this)[from].label;

    for (Vertex::edge_iterator it = (*this)[from].edge.begin ();
        it != (*this)[from].edge.end (); ++it)
    {
      edgecount += 1;
    }
  }
  mxSetField (gs, 0, "nodelabels", nodelabels);

  /* Now do the edges.
   */
  mxArray* edges = mxCreateNumericMatrix (edgecount, 3, mxUINT32_CLASS, 0);
  assert (edges != NULL);
  uint32_T* edges_P = (uint32_T*) mxGetPr (edges);
  //assert (edges_P != NULL);

  unsigned int eidx = 0;
  for (int from = 0; from < (int)size (); ++from) {
    for (Vertex::edge_iterator it = (*this)[from].edge.begin ();
        it != (*this)[from].edge.end (); ++it)
    {
      if (directed || from <= it->to) {
        edges_P[eidx+0*edgecount] = from+1;
        edges_P[eidx+1*edgecount] = it->to+1;
        edges_P[eidx+2*edgecount] = it->elabel;
      } else {
        // Reverse edge direction
        edges_P[eidx+0*edgecount] = it->to+1;
        edges_P[eidx+1*edgecount] = from+1;
        edges_P[eidx+2*edgecount] = it->elabel;
      }
      eidx += 1;
    }
  }
  mxSetField (gs, 0, "edges", edges);

  return (gs);
}
#endif

std::istream &Graph::read(std::istream &is) {
  std::vector<std::string> result;
  char line[1024];

  static int count = -1;

  clear();

  count++;

  while (true) {

    unsigned int pos = is.tellg();
    if (!is.getline(line, 1024))
      break;
    result.clear();

    tokenize<std::string>(line, std::back_inserter(result));
    //cout << line << endl;
    if (result.empty()) {
      // do nothing
    } else if (result[0] == "t") {
      if (!empty()) {
        is.seekg(pos, std::ios_base::beg);
        break;
      } else {
      }
    } else if (result[0] == "v" && result.size() >= 3) {
      unsigned int id = atoi(result[1].c_str());
      this->resize(id + 1);
      (*this)[id].label = atoi(result[2].c_str());

    } else if (result[0] == "e" && result.size() >= 4) {
      int from = atoi(result[1].c_str());
      int to = atoi(result[2].c_str());
      int elabel = atoi(result[3].c_str());

      if ((int) size() <= from || (int) size() <= to) {
        std::cerr << "Format Error:  define vertex lists before edges"
                  << std::endl;
        std::cerr << "count: " << count << " Size: " << size() << ", from: "
                  << from << ", to: " << to << std::endl;
        exit(-1);
      }

      (*this)[from].push(from, to, elabel);
      if (directed == false)
        (*this)[to].push(to, from, elabel);
    }
  }

  buildEdge();

  return is;
}

std::ostream &Graph::write(std::ostream &os) {
  char buf[512];
  std::set<std::string> tmp;

  for (int from = 0; from < (int) size(); ++from) {
    os << "v " << from << " " << (*this)[from].label << std::endl;

    for (Vertex::edge_iterator it = (*this)[from].edge.begin();
        it != (*this)[from].edge.end(); ++it) {
      if (directed || from <= it->to) {
        std::sprintf(buf, "%d %d %d", from, it->to, it->elabel);
      } else {
        std::sprintf(buf, "%d %d %d", it->to, from, it->elabel);
      }
      tmp.insert(buf);
    }
  }

  for (std::set<std::string>::iterator it = tmp.begin(); it != tmp.end();
      ++it) {
    os << "e " << *it << std::endl;
  }

  return os;
}

void Graph::check(void) {
  for (int from = 0; from < (int) size(); ++from) {

    for (Vertex::edge_iterator it = (*this)[from].edge.begin();
        it != (*this)[from].edge.end(); ++it) {
      assert(it->from >= 0 && it->from < size());
      assert(it->to >= 0 && it->to < size());
    }
  }
}

}

