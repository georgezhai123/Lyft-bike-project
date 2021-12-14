# Lyft Bike Project


## .csv files Doc/Format

Example: 0,SF-R19,SF-R20,37.750494,-122.4338905,37.75088167,-122.4276223
First column: index(serve as the ONLY identification throughout entire code-base for the STARTING STATION)  i.e Here: 0 is the station ID for SF-R19
Second column: start station name
Third column: end station name
Fourth column: start station latitude
Fifth column: start station longitude
Sixth column: end station latitude
Seventh column: end station longitude


## Data

In the data folder, nodes.csv's second column contains all the avaliable nodes/stations for the graph,and it will be used as a "blueprint" for all the stations. In this file, It's important to note that we only use the index (first column, "station ID" for start station) ,starting station(second column of data) and its corresponding latitude and longitude(fourth and fifth column respectively), all other data in this file will be ignored. final_graph_data contains all the valid trips that are used to construct the edges of the full graph. Lyft2.csv contains the repeated data that used for tracking number of visits. Nodes.csv and final_graph_data.csv will be able to construct a full graph for San Francisco lyft bike project. station_popularity.csv has the number of visits to each station. This is used to address the accesibility and the popularity of lyft bike stations. station_accessibility.csv has the accessibility ratio (15 bikes per station / number of visits at that station) information.



## Instruction on making customized data/full dataset for testing
FOR OBTAINING TESTING DATA SOLUTION:
First, make sure the file input of the get_nodes_data function in tests.cpp is "./tests/test_nodes_data.csv". Second,make sure the file input of the get_graph_data function in tests.cpp is "./tests/test_graph_data.csv"

FOR OBTAINING THE FULL SOLUTION:
First, make sure the file input of the get_nodes_data function in tests.cpp is "./data/nodes.csv". Second,make sure the file input of the get_graph_data function in tests.cpp is "./data/final_graph_data.csv". Then run the code in main.cpp/tests.cpp

IMPORTANT NOTE: THIS graph will not contain "invalid trip" (START AND END STATION IS THE SAME). DISCONNECTED GRAPH IS NOT ALLOWED IN THIS PROJECT'S CONTEXT!

### Making customized data:
1.Open tests folder->test_graph_data.csv\
2.Write your own trip data inside the test_graph_data.csv in the format of final_graph_data.csv. Create the trips in a way that ALL stations are CONNECTED, i.e you should have a SINGLE connected graph(you only need to manipulate start and end station name).You can write random numbers for index,longtitude and latitude in test_graph_data.csv since all the meaningful location data and identification data will be stored in test_nodes_data.csv and a location map(graphic output)\
3.Now you need to get your station information from nodes.csv(remember we only use column 1,2,4,5 of the data). Find both start & end stations from test_graph_data.csv in nodes.csv:
    search for start & end station names in nodes.csv in vscode, once highlighted/found, copy the line that has the highlighted station as starting station for that trip (ie, look for the line that has the station you searched in 2nd column)to test_nodes_data.csv\
4.If a node/station already exists in the 2nd column of test_nodes_data.csv, skip step 4\
5.Finally, after searching for all the targeted nodes, reset all the index(first column) in test_nodes_data.csv (with increasing index starting from 0) 0,1,2,3..


## Src
Graph.h/Graph.cpp : contains the implementation of the Graph for this project. Includes constructor, BFS, DFS ,Kruskal, and Dijkstra Algorithms.\
dsets.h/dsets.cpp: contains the disjoint set implementation that is used in the Kruskal algorithm to find minimum spanning tree\
Map.h/Map.cpp: contains the implementation of generating the graphic output of the graph (projecting graph data onto a map)\
main.cpp: Have detailed steps/code on how to construct the graph using function from /tests/tests.cpp to obtain the full heuristic solultion,the shortest path between a and b,and the popularity of each station. It also provides code for user to generate the graphic output of data\
tests.cpp: contains the actual construction of graph object of interest, all the maps needed for graphical output algorithm, station accessibility algorithm, and test suites for graph construction(correct number of nodes and adjacent pairs), bfs(correct order and number of visits), shortest path(correct order of visits and number of visits),findMST(correct number of nodes and adjacent pairs), and dfs(correct order and number of visits).


## How to make and run tests

### Make 
Make sure to comment out all the TEST_CASEs ONLY in the tests.cpp BEFORE you make (DON'T uncomment things above TEST CASEs, main.cpp needs those functions to build graph)

This will let you make and compile the codes in main.cpp which you can use to test your own data or play around with the full dataset

```bash
make
./graph
```

### Make test
When running test, make sure the data you put in the test csv files and the test you want to perform actually make sense
For example: you can't run BFS if your graph's disconnected (IN THIS PROJECT'S CONTEXT)
Make sure to uncomment out all the TEST_CASEs in the tests.cpp BEFORE you make test

```bash
make test
./test
```

