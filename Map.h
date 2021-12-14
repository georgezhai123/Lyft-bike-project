#pragma once
#include "Graph.h"
#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"
#include "cs225/lodepng/lodepng.h"

using namespace std;
using namespace cs225;

struct Point {
    int x;
    int y;
};


class Map: public PNG {
    public:
    int mapWidth_;
    int mapHeight_;
    double mapLngLeft_;
    double mapLngRight_;
    double mapLatBottom_;

    Map(int mapWidth,int mapHeight, double mapLngLeft, double mapLngRight, double mapLatBottom);
    Point convertGeoToPixel(double latitude, double longitude,
                  int mapWidth, // in pixels
                  int mapHeight, // in pixels
                  double mapLngLeft, // in degrees. the longitude of the left side of the map (i.e. the longitude of whatever is depicted on the left-most part of the map image)
                  double mapLngRight, // in degrees. the longitude of the right side of the map
                  double mapLatBottom);
    Map* draw_stations(vector<vector<string>> stations);
    void Enlarge(double x, double y);
    Map* draw_edges(vector<vector<string>> trips, map<string, vector<string>> location_data);
    Map* draw_shortest_path(vector<unsigned int> shortest_path, map<string, vector<string>> location_data, map<unsigned int,string> nodemap);
    Map* draw_single_station(double latitude, double longitude);
    };


