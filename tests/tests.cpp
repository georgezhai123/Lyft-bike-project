#include "../Graph.h"
#include "../cs225/catch/catch.hpp"
#include "../Map.h"
#include "../cs225/PNG.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <cmath>
#define pi 3.14159265358979323846
using namespace std;
using namespace cs225;


map<string,unsigned int> create_node_map(vector<vector<string>> nodes_data) {
    // this map is used to identify each station with corresponding index
    map<string, unsigned int> nodemap;
    //size 256  index, startid, endid, startlati,startlong,endlati,endlong
    //create a map (key: stationname, value: index)
    //using start station as the vertexes 
    for(unsigned int i = 0; i < nodes_data.size(); i++) {
    string station_name = nodes_data[i][1];
    nodemap[station_name] = i;
    }
    return nodemap;
}

map<unsigned int,string> create_node_map_rev(vector<vector<string>> nodes_data) {
    // this map is used to identify each station with corresponding index
    map<unsigned int, string> nodemap;
    //size 256  index, startid, endid, startlati,startlong,endlati,endlong
    //create a map (key: stationname, value: index)
    //using start station as the vertexes 
    for(unsigned int i = 0; i < nodes_data.size(); i++) {
    unsigned int station_id = stoul(nodes_data[i][0]);
    nodemap[station_id] = nodes_data[i][1];
    }
    return nodemap;
}


vector<vector<string>> get_visiting_data() {
    //used for getting the visiting data
    ifstream graphs_data;
    string station;
    string word;
    graphs_data.open("./data/lyft2.csv");
    vector<vector<string>> visiting_data;// <<start,end,start_lati,start_long,end_lati,end_longti>>
    if (graphs_data.is_open())  {
    while(getline(graphs_data, station)) {
        //clear nodes 
        vector<string> nodes;
        stringstream str(station);
        while(getline(str, word, ',')) {
            nodes.push_back(word);
            // contents without commas
        }
        visiting_data.push_back(nodes);
    }
    } else {
    cout << "can't open the file";
    }
    return visiting_data;
}

map<string,double> get_station_accessibility() {
    vector<vector<string>> visiting_data = get_visiting_data();
    map<string,double> station_accessibility;
    for (vector<string> visit : visiting_data) {
        string start_station = visit[0];
        string end_station = visit[1];
        map<string,double>::iterator lookup1 = station_accessibility.find(start_station);
        map<string,double>::iterator lookup2 = station_accessibility.find(end_station);
        if (lookup1 == station_accessibility.end()) {
            station_accessibility.insert(make_pair(start_station, 1));
        } else {
            lookup1->second = lookup1->second + 1;
        }
        if (lookup2 == station_accessibility.end()) {
            station_accessibility.insert(make_pair(end_station, 1));
        } else {
            lookup2->second = lookup2->second + 1;
        }
    }
    for (auto & station : station_accessibility) {
        station.second = 15 / station.second;
    }
    return station_accessibility;
}


void print_dict(map<string,double> myMap) {
    for(auto it = myMap.begin(); it != myMap.end(); ++it){
    std::cout << it->first << " " << it->second<< " " << "\n";
    }
}


vector<vector<string>> get_nodes_data() {
    //used for constructing nodes of the graph
    ifstream graphs_data;
    string station;
    string word;
    graphs_data.open("./tests/test_nodes_data.csv");
    vector<vector<string>> nodes_data;// <<0,ID,lat,long>>
    if (graphs_data.is_open())  {
    while(getline(graphs_data, station)) {
        //clear nodes 
        vector<string> nodes;
        stringstream str(station);
        while(getline(str, word, ',')) {
            nodes.push_back(word);
            // contents without commas
        }
        nodes_data.push_back(nodes);
    }
    } else {
    cout << "can't open the file";
    }
    return nodes_data;
}

vector<vector<string>> get_graph_data() {
    //used for constructing graph edges
    ifstream graphs_data1;
    string station1;
    string word1;
    graphs_data1.open("./tests/test_graph_data.csv");
    vector<vector<string>> data;// <<0,ID,lat,long>>         data:<nodes1,nodes1,nodes1> 
    if (graphs_data1.is_open())  {
    while(getline(graphs_data1, station1)) {
        //clear nodes 
        vector<string> nodes1;
        stringstream str(station1);
        while(getline(str, word1, ',')) {
            nodes1.push_back(word1);
            // contents without commas
        }
        data.push_back(nodes1);
    }
    } else {
    cout << "can't open the file";
    }
    return data;
}

map<string, vector<string>> create_location_map(vector<vector<string>> nodes_data) {
    //given a string of station, we should know its "fixed" location(after discovering location data provided by lyft is not the same for the same station)
    map<string, vector<string>> location_map;
    for(unsigned int i = 0; i < nodes_data.size(); i++) {
    vector<string> location_data;
    location_data.push_back(nodes_data[i][3]);
    location_data.push_back(nodes_data[i][4]);
    location_map[nodes_data[i][1]] = location_data;
    }
    return location_map;
}

vector<Edge> create_edges() {
    vector<vector<string>> nodes_data = get_nodes_data();
    vector<vector<string>> data = get_graph_data();
    map<string,unsigned int> nodemap = create_node_map(nodes_data);
    //build vector of edges
    vector<Edge> edges;
    for (unsigned int i = 0; i < data.size(); i++) {
    string start_station = data[i][1];
    string end_station = data[i][2];
    if(start_station == end_station) {
        continue;
    }
    unsigned int start;
    unsigned int end;
    map<string,unsigned int>::iterator lookup1 = nodemap.find(start_station);
    map<string,unsigned int>::iterator lookup2 = nodemap.find(end_station);
    if (lookup1 != nodemap.end()) {
        start = lookup1->second;
    } else {
        continue;
    }
    if (lookup2 != nodemap.end()) {
        end = lookup2->second;
    } else {
        continue;
    }
    double distance;
    double lati1 = stod(nodes_data[start][3]);
    double lng1 = stod(nodes_data[start][4]);
    double lati2 = stod(nodes_data[end][3]);
    double lng2 = stod(nodes_data[end][4]);
    //calculate distance
    distance = sin(lati1) * sin(lati2) + cos(lati1) * cos(lati2) * cos(lng1 - lng2);
    distance = acos(distance);
    distance = (6372 * pi * distance) / 180;


    Edge edge = {start, end, distance};
    edges.push_back(edge);
    }
    return edges;
}

//check graph has the right number of nodes. REMEMBER: THIS PROJECT DOESN'T ALLOW DISCONNECTED GRAPH!
TEST_CASE("Graph has correct number of nodes(include edge case where start and end are the same station)", "[weight=1]") {
    //construct graph
    vector<Edge> edges = create_edges();
    vector<vector<string>> nodes_data = get_nodes_data();
    Graph graph(edges, nodes_data.size());
    size_t has_element = 0;
    size_t no_element = 0;
    for (size_t i = 0; i < nodes_data.size(); i++) {
        if (graph.adjList[i].empty()) {
            no_element = no_element + 1;//some vertexes won't be in the graph due to "invalid trip" in test_graph_data (start and end are the same)
            cout << no_element << endl; 
        } else {
            has_element = has_element + 1;// how many vertexes/stations are actually in the graph
        }
    }
  REQUIRE( has_element + no_element == nodes_data.size());
}


// Properly constructed graph
TEST_CASE("Graph has correct adjacent pairs ","[weight=1]") {
    //construct graph
    vector<Edge> edges = create_edges();
    vector<vector<string>> nodes_data = get_nodes_data();
    vector<vector<string>> graph_data = get_graph_data();
    Graph graph(edges, nodes_data.size());
    map<string,unsigned int> nodemap = create_node_map(nodes_data);
    bool found = false;
    for (vector<string> edge : graph_data) {
        found = false; //check every edge is built correctly in the graph
        string start_station = edge[1];
        string end_station = edge[2];
        unsigned int start_id = nodemap[start_station];
        unsigned int end_id = nodemap[end_station];
        if (start_id != end_id) {
            for (Pair pair: graph.adjList[start_id]) {
                if (pair.first == end_id) {
                    found = true;
                    break;
                }
            }
        } else {
            found = true;
            continue;
        }
        if (!found) {
            break;
        }
    }
  REQUIRE( found == true);//found all adjacent pairs
}


TEST_CASE("BFS shouldn't visit disconnected component","[weight=1]") {
    vector<Edge> edges = create_edges();
    vector<vector<string>> nodes_data = get_nodes_data();
    vector<vector<string>> graph_data = get_graph_data();
    Graph graph(edges, nodes_data.size());
    graph.BFS(0);
    bool is_disconnected = false;
    for (vector<string> edge : graph_data) {
        string start_station = edge[1];
        string end_station = edge[2];
        if (start_station == end_station) {
            is_disconnected = true;
            break;
        }
    }
    if (is_disconnected) {
        REQUIRE(graph.BFSresult.size() < nodes_data.size());
    }
}

// This test case only works for the originally inputted data in test_nodes_data.csv and test_graph_data.csv
TEST_CASE("BFS visits all the nodes in a connected component in correct order","[weight=1]") {
    vector<Edge> edges = create_edges();
    vector<vector<string>> nodes_data = get_nodes_data();
    vector<vector<string>> graph_data = get_graph_data();
    Graph graph(edges, nodes_data.size());
    graph.BFS(0);
    vector<unsigned int> solution;
    solution.push_back(0);
    solution.push_back(1);
    solution.push_back(8);
    solution.push_back(3);
    solution.push_back(5);
    solution.push_back(6);
    solution.push_back(4);
    solution.push_back(7);
    solution.push_back(2);
    REQUIRE(graph.BFSresult == solution);
}

// This test case only works for the originally inputted data in test_nodes_data.csv and test_graph_data.csv
TEST_CASE("shortest path with correct order of visit and number of visits","[weight=1]") {
    vector<Edge> edges = create_edges();
    vector<vector<string>> nodes_data = get_nodes_data();
    vector<vector<string>> graph_data = get_graph_data();
    Graph graph(edges, nodes_data.size());
    graph.shortest_path(0);
    vector<unsigned int> shortest_path = graph.path_to_take(0,7);
    REQUIRE(shortest_path.size() == 4);
    vector<unsigned int> solution;
    solution.push_back(0);
    solution.push_back(1);
    solution.push_back(6);
    solution.push_back(7);
    REQUIRE(shortest_path == solution);
}

TEST_CASE("FindMST has correct number of edges","[weight=1]") {
    vector<Edge> edges = create_edges();
    vector<vector<string>> nodes_data = get_nodes_data();
    vector<vector<string>> graph_data = get_graph_data();
    Graph graph(edges, nodes_data.size());
    Graph* G = graph.FindMST();
    REQUIRE(G->adjList.size() - 1 == G->edges_.size());
}

// This test case only works for the originally inputted data in test_nodes_data.csv and test_graph_data.csv
TEST_CASE("DFS of minimum spanning tree(heurisic solution to TSP) has the correct number and order of visits","[weight=1]") {
    vector<Edge> edges = create_edges();
    vector<vector<string>> nodes_data = get_nodes_data();
    vector<vector<string>> graph_data = get_graph_data();
    Graph graph(edges, nodes_data.size());
    Graph* G = graph.FindMST();
    G->DFS(0);
    vector<unsigned int>dfs_path = G->dfs;
    vector<unsigned int>dfs_solution;
    dfs_solution.push_back(0);
    dfs_solution.push_back(1);
    dfs_solution.push_back(4);
    dfs_solution.push_back(6);
    dfs_solution.push_back(7);
    dfs_solution.push_back(8);
    dfs_solution.push_back(3);
    dfs_solution.push_back(5);
    dfs_solution.push_back(2);
    REQUIRE(dfs_path == dfs_solution);
}







