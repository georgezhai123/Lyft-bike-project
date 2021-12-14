#pragma once
#include <iostream>
#include <vector>
#include "dsets.h"
#include <map>

using namespace std;

 
// Data structure to store a graph edge
struct Edge {
    unsigned int src, dest;
    double weight;
};


typedef pair<unsigned int, double> Pair;//pair: src/dest , weight
typedef tuple<unsigned int,unsigned int,double> triplet; //src,dest,weight
 
// A class to represent a graph object
class Graph
{
public:
    // Graph Constructor
    Graph(vector<Edge> &edges, unsigned int n);
    void BFS(unsigned int v);
    void printGraph();
    void printEdge();
    void printdfs();
    void shortest_a_to_b(unsigned int end,unsigned int start);
    vector<unsigned int> path_to_take(unsigned int start, unsigned int end);
    Graph* FindMST();
    void DFS(unsigned int v);
    void shortest_path(unsigned int start);
    // a vector of vectors of Pairs to represent an adjacency list
    vector<vector<Pair>> adjList;
    vector<Edge> edges_;
    DisjointSets set;
    map<unsigned int, bool> visitedV;
    map<unsigned int, bool> BFSvisited;
    vector<unsigned int> dfs;
    vector<triplet> shortest_path_result; // <(vertex,predecessor,distance from start)>
    vector<unsigned int> BFSresult;
};