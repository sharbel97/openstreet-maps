// application.cpp <Starter Code>
// Sharbel Homa
//
// University of Illinois at Chicago
// CS 251: Spring 2021
// Project #7 - Openstreet Maps
//
// References:
// TinyXML: https://github.com/leethomason/tinyxml2
// OpenStreetMap: https://www.openstreetmap.org
// OpenStreetMap docs:
//   https://wiki.openstreetmap.org/wiki/Main_Page
//   https://wiki.openstreetmap.org/wiki/Map_Features
//   https://wiki.openstreetmap.org/wiki/Node
//   https://wiki.openstreetmap.org/wiki/Way
//   https://wiki.openstreetmap.org/wiki/Relation
//

// questions:
// 1. mimir output
// 2. neighbors func
// 3.
#include <iostream>
#include <iomanip>  /*setprecision*/
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <stack>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <limits>

#include "graph.h"
#include "tinyxml2.h"
#include "dist.h"
#include "osm.h"

using namespace std;
using namespace tinyxml2;

const int INF = numeric_limits<int>::max();
void Dijkstra(graph<long long, double>& G, long long startV, map<long long, double>& distances, map<long long,long long>& predecessors);

class prioritize {

  public:
    bool operator() (const pair<long long, double>&p1,
               const pair<long long, double>&p2) const {
                  if (p1.second == p2.second) {
                    return p1.first > p2.first;
                  }
                return p1.second > p2.second;
              }
};

int main() {
    // maps a Node ID to it's coordinates (lat, lon)
    map<long long, Coordinates>  Nodes;
    // info about each footway, in no particular order
    vector<FootwayInfo>          Footways;
    // info about each building, in no particular order
    vector<BuildingInfo>         Buildings;
    XMLDocument                  xmldoc;
    
    cout << "** Navigating UIC open street map **" << endl;
    cout << endl;
    cout << std::setprecision(8);
    
    string def_filename = "map.osm";
    string filename;
    
    cout << "Enter map filename> ";
    getline(cin, filename);
    
    if (filename == "") {
    filename = def_filename;
    }
    
    //
    // Load XML-based map file
    //
    if (!LoadOpenStreetMap(filename, xmldoc)) {
    cout << "**Error: unable to load open street map." << endl;
    cout << endl;
    return 0;
    }
    
    //
    // Read the nodes, which are the various known positions on the map:
    //
    int nodeCount = ReadMapNodes(xmldoc, Nodes);
    
    //
    // Read the footways, which are the walking paths:
    //
    int footwayCount = ReadFootways(xmldoc, Footways);
    
    //
    // Read the university buildings:
    //
    int buildingCount = ReadUniversityBuildings(xmldoc, Nodes, Buildings);
    
    //
    // Stats
    //
    assert(nodeCount == (int)Nodes.size());
    assert(footwayCount == (int)Footways.size());
    assert(buildingCount == (int)Buildings.size());
    
    cout << endl;
    cout << "# of nodes: " << Nodes.size() << endl;
    cout << "# of footways: " << Footways.size() << endl;
    cout << "# of buildings: " << Buildings.size() << endl;
    
    
    //
    // TO DO: build the graph, output stats:
    //
    graph<long long, double> G;
    for (auto coordinate : Nodes) {
        G.addVertex(coordinate.first);
    }
    
    // double distBetween2Points(double lat1, double long1, double lat2, double long2)
    // map<long long, Coordinates>  Nodes;
    // FootwayInfo: long long ID, vector<long long> Nodes.
    
    for (auto footway : Footways) {
        //long long id = footway.ID; // get the ID of the Node
        for (unsigned int i = 0; i < footway.Nodes.size()-1; i++) {
            
            double distance = distBetween2Points(Nodes[footway.Nodes[i]].Lat,
                                              Nodes[footway.Nodes[i]].Lon,
                                              Nodes[footway.Nodes[i+1]].Lat,
                                              Nodes[footway.Nodes[i+1]].Lon);
            
            G.addEdge(footway.Nodes[i], footway.Nodes[i+1], distance);
            G.addEdge(footway.Nodes[i+1], footway.Nodes[i], distance);
        }
    }
    
    cout << "# of vertices: " << G.NumVertices() << endl;
    cout << "# of edges: " << G.NumEdges() << endl;
    cout << endl;
    
    //
    // Navigation from building to building
    //
    string startBuilding, destBuilding;
    cout << "Enter start (partial name or abbreviation), or #> ";
    getline(cin, startBuilding);
    
    while (startBuilding != "#") {
        cout << "Enter destination (partial name or abbreviation)> ";
        getline(cin, destBuilding);
        
        
        //
        // TO DO: lookup buildings, find nearest start and dest nodes,
        // run Dijkstra's alg, output distance and path to destination:
        //
        Coordinates startCoords, destCoords;
        BuildingInfo startBuildingInfo, destBuildingInfo;
        for (unsigned int i = 0; i < Buildings.size(); i++) {
            if (Buildings[i].Abbrev == startBuilding) {
                startCoords = Buildings[i].Coords;
                startBuildingInfo = Buildings[i];
                break;
            }
            size_t found = Buildings[i].Fullname.find(startBuilding);
            if (found != string::npos) {
                startCoords = Buildings[i].Coords;
                startBuildingInfo = Buildings[i];
                break;
            }
        }
        
        for (unsigned int i = 0; i < Buildings.size(); i++) {
            if (Buildings[i].Abbrev == destBuilding) {
                destCoords = Buildings[i].Coords;
                destBuildingInfo = Buildings[i];
                break;
            }
            size_t found = Buildings[i].Fullname.find(destBuilding);
            if (found != string::npos) {
                destCoords = Buildings[i].Coords;
                destBuildingInfo = Buildings[i];
                break;
                
            }
        }
        
        
        
        // getting nearest startnode and destination Nodes.
        long long startNodeID= -1;
        long long endNodeID = -1;
        if (startCoords.ID != 0 && destCoords.ID != 0) {
            // we have both buildings info
            cout << "Starting point: " << endl;
            cout << " " << startBuildingInfo.Fullname << endl;
            cout << " (" << startBuildingInfo.Coords.Lat << ", ";
            cout << startBuildingInfo.Coords.Lon << ")" << endl;
            cout << "Destination point:" << endl;
            cout << " " << destBuildingInfo.Fullname << endl;
            cout << " (" << destBuildingInfo.Coords.Lat << ", ";
            cout << destBuildingInfo.Coords.Lon << ")" << endl;
            
            // search footways
            double minStartDistance = INF;
            double minDestDistance = INF;
            for (auto footway : Footways) {

                for (unsigned int i = 0; i < footway.Nodes.size(); i++) {
                    Coordinates startNodetemp, endNodetemp;
                    startNodetemp = Nodes[footway.Nodes[i]];
                    endNodetemp = Nodes[footway.Nodes[i]];
                    
                    double minDistance =
                    distBetween2Points(startCoords.Lat, startCoords.Lon,
                                       startNodetemp.Lat, startNodetemp.Lon);

                    if (minDistance < minStartDistance){
                        minStartDistance = minDistance;
                        startNodeID = startNodetemp.ID;
                    }
                    
                    minDistance =
                    distBetween2Points(destCoords.Lat, destCoords.Lon,
                                       endNodetemp.Lat, endNodetemp.Lon);
                    
                    if (minDistance < minDestDistance) {
                        minDestDistance = minDistance;
                        endNodeID = endNodetemp.ID;
                    }
                }
            }
            
            cout << endl;
            cout << "Nearest start node:" << endl;
            cout << " " << startNodeID << endl;
            cout << " (" << Nodes[startNodeID].Lat << ", ";
            cout << Nodes[startNodeID].Lon << ")" << endl;
            cout << "Nearest destination node:" << endl;
            cout << " " << endNodeID << endl;
            cout << " (" << Nodes[endNodeID].Lat << ", ";
            cout << Nodes[endNodeID].Lon << ")" << endl;
            
            // now we should have coordinates startNodeID, endNodeID
            
            map<long long,double> distances;
            map<long long,long long> predecessors;
            Dijkstra(G, startNodeID, distances, predecessors);
            
            stack<long long> path;
            long long temp = endNodeID;
            while (temp != 0) {
                path.push(temp);
                temp = predecessors[temp];
            }
            // distances[DestinationID]
            cout << endl;
            cout << "Navigating with Dijkstra..." << endl;
            if (distances[endNodeID] > 100) {
                printf("Sorry, destination unreachable\n");
            } else {
                cout << "Distance to dest: " << distances[endNodeID] << " miles" << endl;
                cout << "Path: ";
                while(path.size() > 1) {
                    cout << path.top() << "->";
                    path.pop();
                }
                cout << path.top() << endl;
                path.pop();
            }
            
        } else {
            if (startCoords.ID == 0)
                cout << "Start building not found" << endl;
        
            else if (destCoords.ID == 0)
                cout << "Destination building not found" << endl;
        }
        //
        // another navigation?
        //
        cout << endl;
        cout << "Enter start (partial name or abbreviation), or #> ";
        getline(cin, startBuilding);
    }

    //
    // done:
    //
    cout << "** Done **" << endl;
    return 0;
}



//
// Dijkstra:
//
// Performs Dijkstra's shortest weighted path algorithm from
// the given start vertex.  Returns a vector of vertices in
// the order they were visited, along with a map of (string,int)
// pairs where the string is a vertex V and the int is the 
// distance from the start vertex to V; if no such path exists,
// the distance is INF
//
void Dijkstra(graph<long long,double>& G,long long startV,
              map<long long,double>& distances,
              map<long long,long long>& predecessors)
{
  priority_queue< pair<long long, double>, 
                       vector<pair<long long,double>>,
                       prioritize> unvisitedQueue;
  set<long long> visitedSet;
  vector<long long> vertices = G.getVertices();

  for (auto currentV : vertices) {
    distances[currentV] = INF;
    predecessors[currentV] = 0;
    unvisitedQueue.push(make_pair(currentV, INF));
  }
  
  // startV has distance 0 from itself
  distances[startV] = 0;
  unvisitedQueue.push(make_pair(startV, 0));
  
  while (!unvisitedQueue.empty()) {
    // visit vertex with minimum index from startV
    long long currentV = unvisitedQueue.top().first;
    unvisitedQueue.pop();
    
    if (distances[currentV] == INF) break;
    else if (visitedSet.count(currentV) > 0 ) continue;
    else {
      visitedSet.insert(currentV);
    }

    // for each adjacent vertex to currentV
    set<long long> neighborss = G.neighbors(currentV);
    for (auto neighbor : neighborss) {
      // bool getWeight(VertexT from, VertexT to, WeightT& weight) const
      double edgeWeight;
      G.getWeight(currentV, neighbor, edgeWeight);
      double alternativePathDistance = distances[currentV] + edgeWeight;
      
      // if shorter path from startV to adjV is found,
      // update adjV's distance and predecessor
      if (alternativePathDistance < distances[neighbor]) {
        distances[neighbor] = alternativePathDistance;
        predecessors[neighbor] = currentV;
        unvisitedQueue.push(make_pair(neighbor, alternativePathDistance));
      }
    }
  }
}

