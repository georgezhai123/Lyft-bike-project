#include "Graph.h"
#include <iostream>
#include <vector>
#include <tuple>
#include <cfloat>
#include <queue>
#include <map>
using namespace std;

// Graph Constructor
Graph::Graph(vector<Edge>& edges, unsigned int n)
{
    edges_ = edges;
    // resize the vector to hold `n` elements/vertexes of type vector<Edge> 
    adjList.resize(n); 

    // add edges to the undirected graph
    for (auto &edge: edges)
    {
        unsigned int src = edge.src;
        unsigned int dest = edge.dest;
        double weight = edge.weight;
        bool exists = false;
        // if already pushed A TO B, don't push B TO A again
        for (Pair pair: adjList[src]) {
            if (pair.first == dest) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            // insert at the end
            adjList.at(src).push_back(make_pair(dest, weight));

            // for undirected graph
            adjList.at(dest).push_back(make_pair(src, weight));
        }
    }
}

//print the graph
void Graph::printGraph() {
    for (unsigned int i = 0; i < adjList.size(); i++) {
        // Function to print all neighboring vertices of a given vertex
        for (Pair v: adjList[i]) {
            cout << "(" << i << ", " << v.first << ", " << v.second << ") ";
        }
        cout << endl;
    }
}

void Graph::printEdge() {
    for (size_t i = 0; i < edges_.size(); i++) {
        cout << edges_[i].src << " " << edges_[i].dest << endl;
    }
}

void Graph::printdfs() {
    for(size_t i = 0; i < dfs.size(); i++) {
        cout << dfs[i] << " ";
    }
    cout << endl;
}

//print the path from a to b
void Graph::shortest_a_to_b(unsigned int end,unsigned int start) {
    if (get<0>(shortest_path_result[end]) == start) {
        cout << start << endl;
        return;
    }
    else {
        cout << end << endl;
        shortest_a_to_b(get<1>(shortest_path_result[end]),start);
    }
}

//store the shortest path from a to b as a vector
vector<unsigned int> Graph::path_to_take(unsigned int start, unsigned int end) {
    vector<unsigned int> shortest_path;
    unsigned int temp = end;
    shortest_path.push_back(end);
    while(temp != start) {
        unsigned int predecessor = get<1>(shortest_path_result[temp]);
        shortest_path.push_back(predecessor);
        temp = predecessor;
    }
    reverse(shortest_path.begin(),shortest_path.end());
    return shortest_path;
}


// Using Kruskal's Algorithm to find MST
Graph* Graph::FindMST() {
    unsigned int number_of_nodes = adjList.size();
    //create disjoint forest
    set.addelements(number_of_nodes);
    map<triplet,bool> Edge_existed;
    //create priority queue based on min edges/minheap
    auto compare = [](triplet a, triplet b) { return get<2>(a) > get<2>(b); }; // minheap using >
    priority_queue<triplet, vector<triplet>, decltype(compare)> pq(compare);
    for (unsigned int i = 0; i < number_of_nodes; i++) {
        for (unsigned int j = 0; j < adjList[i].size(); j++) {
            unsigned int desti = adjList[i][j].first;
            double weight = adjList[i][j].second;
            triplet curr = make_tuple(i,desti,weight);
            triplet curr_rev = make_tuple(desti,i,weight);
            map<triplet,bool>::iterator lookup1 = Edge_existed.find(curr);
            map<triplet,bool>::iterator lookup2 = Edge_existed.find(curr_rev);
            if (lookup1 == Edge_existed.end() && lookup2 == Edge_existed.end()) {
                pq.push(curr);
                //visited the edge, make sure not pushing in repeated edges
                Edge_existed.insert({curr,true}); // tuple(i,desti,weight)
                Edge_existed.insert({curr_rev,true});
            }
        }
    }
    unsigned int edges = 0;
    vector<Edge> Edges;//empty graph
    //|| size(0) != number_of_nodes
    while(edges < number_of_nodes - 1) {
        triplet min = pq.top();
        pq.pop();
        unsigned int src = get<0>(min);
        unsigned int dest = get<1>(min);
        double cost = get<2>(min);
        if (set.find(src) != set.find(dest)) { //not in the same set, need to union
            Edge edge = {src, dest, cost}; //build edge
            Edges.push_back(edge);
            edges++;
            set.setunion(set.find(src), set.find(dest));//path compression
        }
    }
    //MST V = E + 1
    Graph* mst = new Graph(Edges, number_of_nodes);
    return mst;
}

void Graph::shortest_path(unsigned int start) {
    vector<bool> visited;
    unsigned int number_of_nodes = adjList.size();
    visited.resize(number_of_nodes);
    for (unsigned int i = 0; i < number_of_nodes; i++) {
        visited[i] = false;
    }
    for (unsigned int i = 0; i < number_of_nodes; i++) {
        shortest_path_result.push_back(make_tuple(i,0,DBL_MAX));//predescessor 0 means NAN for initialization purpose
    }
    // cout << get<2>(shortest_path_result[169]) << endl;
    //start from here...
    shortest_path_result.at(start) = make_tuple(start,start,0.0);
    //create a priority queue with triplets (v,p,d)  Priority Queue based on vertices
    auto compare = [](triplet a, triplet b) { return get<2>(a) > get<2>(b); }; // minheap using >
    priority_queue<triplet, vector<triplet>, decltype(compare)> pq(compare);
    for (unsigned int i = 0; i < number_of_nodes; i++) {
        pq.push(shortest_path_result[i]);
    }
    unsigned int count = 0;
    while(count < number_of_nodes && !pq.empty()) {
        // cout << count << endl;
        //PQ: assuming the top is the smallest and nonvisited
        triplet min = pq.top();
        //is it a valid top? There might be multiple same node 
        //with different distances from the start inside the queue due to line 175's push into the queue
        //if we visited the node once, its distance from start is the smallest. Next time we see that node, 
        //it's not supposed to be visited again. (A,B,B,B) All other B's after the first B should be ignored.
        while(visited[get<0>(min)] && !pq.empty()) {
            pq.pop();
            min = pq.top();
        }
        visited.at(get<0>(min)) = true; 
        pq.pop();
        unsigned int v = get<0>(min);
        double d = get<2>(min);
        // cout << "size: " << adjList[v].size() << endl;
        for (Pair adj  : adjList[v]) {
            // cout << "neighbour: " << v << " " << adj.first << endl;
            if (!visited[adj.first]) { 
                double adj_dis = get<2>(shortest_path_result[adj.first]);
                // cout << adj.second + d << " " << adj_dis << endl;
                if(d + adj.second < adj_dis) {
                    // cout << "pushing:  "<< adj.first <<endl;
                    pq.push(make_tuple(adj.first, v, d + adj.second));//old copy will be at the back of the queue
                    shortest_path_result.at(adj.first) = make_tuple(adj.first, v, d + adj.second);
                }
            }
        }
        count++;
    }
}

// void Graph::BFS() {
//     for(unsigned int i = 0; i < adjList.size(); i++) {
//         if (!adjList[i].empty()) {
//             BFS(i);
//         }
//     }
// }

void Graph::BFS(unsigned int v) {
    queue<unsigned int> q;
    BFSvisited[v] = true;
    q.push(v);
    while(!q.empty()) {
        v = q.front();
        q.pop();
        BFSresult.push_back(v);
        for(Pair w : adjList[v]) {
            if (BFSvisited[w.first] == false) {
                BFSvisited[w.first] = true;
                q.push(w.first);
            }
        }
    }
}



void Graph::DFS(unsigned int v) {
    visitedV[v] = true;
    dfs.push_back(v);
    for(Pair w : adjList[v]) {
        if (visitedV[w.first] == false) {
            DFS(w.first);
        }
    }
}
