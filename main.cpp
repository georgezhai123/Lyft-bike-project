#include "Graph.h"
#include "Map.h"
#include "cs225/PNG.h"
#include "tests/tests.cpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <cmath>
#define pi 3.14159265358979323846

using namespace std;

int main() {
    vector<Edge> edges = create_edges();
    vector<vector<string>> nodes_data = get_nodes_data();
    vector<vector<string>> data = get_graph_data();
    map<string, vector<string>> location_map = create_location_map(nodes_data);
    map<unsigned int,string> nodemap = create_node_map_rev(nodes_data);
    //construct graph
    Graph graph(edges, nodes_data.size());


    // //print graph
    // graph.printGraph();
    

    //bfs 
    graph.BFS(0);
    // for (auto i: graph.BFSresult)
    // std::cout << i << ' ';
    // cout << endl;


    //finding heuristic solution for the lyft bike maintenance to travel the least distance to visit every station in San Francisco 
    //find MST
    Graph* G = graph.FindMST();
    // G->printEdge();
    // input is the starting position, which can be user determined 
    G->DFS(0);
    vector<unsigned int>dfs_path = G->dfs; //heuristic SOLUTION using traingular inequality(assume an edge exists for dfs tour to happen)
    // G->printdfs();// This path is the the heuristic solution to TSP(i.e the "optimal"(least cost) route that the lyft bike maintenance should take to visit all interested stations)



    //shortest path between any start station and end station. Here is finding the shortest path from station 0 to all the stations
    graph.shortest_path(0);
    //User can provide the start (start must match the line above) and end, and the shortest_a_to_b will print out the shortest path from start to end
    vector<unsigned int> shortest_path = graph.path_to_take(0,166);
    // for (auto i: shortest_path)
    // std::cout << i << ' ' <<endl;
    // // //delete G after use
    // // // delete G;
    


    //Find the popularity of the station(number of visits at each station)
    //get accesibility ratio (15 bikes / number of visist)
    map<string,double> station_accessibility = get_station_accessibility();
    // print_dict(station_accessibility);



    //draw map
    // DON'T CHANGE THIS LINE
    Map SFMap(1792,1418,-122.523815,-122.348528,37.701743);


    SFMap.readFromFile("SFmap.png");
    Map* map = SFMap.draw_stations(nodes_data); //map pointer points to a map with ALL the nodes plotted
    map->draw_edges(data, location_map);
    map->writeToFile("Generated_Map.png");
    map->draw_shortest_path(shortest_path, location_map,nodemap);
    map->writeToFile("Shortest_path.png");
    return 0;
}