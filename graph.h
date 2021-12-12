// graph.h <Starter Code>
// Sharbel Homa
//
// Basic graph class using adjacency matrix representation.  Currently
// limited to a graph with at most 100 vertices.
//
// University of Illinois at Chicago
// CS 251: Spring 2021
// Project #7 - Openstreet Maps
//

#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
#include <set>
#include <map>

using namespace std;

template<typename VertexT, typename WeightT>
class graph {
 private:
  map<VertexT, map<VertexT, WeightT>> adjacencyList;
  int edgeCount = 0;

 public:
  //
  // NumVertices
  //
  // Returns the # of vertices currently in the graph.
  //
  int NumVertices() const {
    return adjacencyList.size();
  }

  //
  // NumEdges
  //
  // Returns the # of edges currently in the graph.
  //
  int NumEdges() const {
    return edgeCount;
  }

  //
  // addVertex
  //
  // Adds the vertex v to the graph if there's room, and if so
  // returns true.  If the graph is full, or the vertex already
  // exists in the graph, then false is returned.
  //
  bool addVertex(VertexT v) {
      if (adjacencyList.count(v) > 0) return false;
      map<VertexT, WeightT> m;
      adjacencyList[v] = m;
      return true;
  }

  //
  // addEdge
  //
  // Adds the edge (from, to, weight) to the graph, and returns
  // true.  If the vertices do not exist or for some reason the
  // graph is full, false is returned.
  //
  // NOTE: if the edge already exists, the existing edge weight
  // is overwritten with the new edge weight.
  //
  bool addEdge(VertexT from, VertexT to, WeightT weight) {
      if (adjacencyList.count(from) == 0) return false;
      if (adjacencyList.count(to) == 0) return false;
      if (adjacencyList.at(from).count(to) == 0) edgeCount++;
      adjacencyList[from][to] = weight;
      return true;
  }

  //
  // getWeight
  //
  // Returns the weight associated with a given edge.  If
  // the edge exists, the weight is returned via the reference
  // parameter and true is returned.  If the edge does not
  // exist, the weight parameter is unchanged and false is
  // returned.
  //
  bool getWeight(VertexT from, VertexT to, WeightT& weight) const {
      if (adjacencyList.count(from) == 0) return false;
      if (adjacencyList.count(to) == 0) return false;
      if (adjacencyList.at(from).count(to) == 0) return false;
      weight = adjacencyList.at(from).at(to);
      return true;
  }

  //
  // neighbors
  //
  // Returns a set containing the neighbors of v, i.e. all
  // vertices that can be reached from v along one edge.
  // Since a set is returned, the neighbors are returned in
  // sorted order; use foreach to iterate through the set.
  //
  set<VertexT> neighbors(VertexT v) const {
    set<VertexT> N;
    if (adjacencyList.count(v) == 0) return N;
    for (auto neighbor : adjacencyList.at(v)) {
        N.insert(neighbor.first);
    }
    return N;
  }

  //
  // getVertices
  //
  // Returns a vector containing all the vertices currently in
  // the graph.
  //
  vector<VertexT> getVertices() const {
    vector<VertexT> V;
    
    for (auto vertex : adjacencyList) {
        V.push_back(vertex.first);
    }
    
    return V;
  }

  //
  // dump
  //
  // Dumps the internal state of the graph for debugging purposes.
  //
  // Example:
  //    graph<string,int>  G;
  //    ...
  //    G.dump(cout);  // dump to console
  //
  void dump(ostream& output) const {
    output << "***************************************************" << endl;
    output << "********************* GRAPH ***********************" << endl;

    output << "**Num vertices: " << NumVertices() << endl;
    output << "**Num edges: " << NumEdges() << endl;

    output << endl;
    for (auto vertex : adjacencyList) {
        output << vertex.first << ": ";
        for (auto edge : vertex.second) {
            output << "(" << vertex.first << "," << edge.first;
            output << "," << edge.second << ") ";
        }
        cout << endl;
    }
    output << "**************************************************" << endl;
  }
};
