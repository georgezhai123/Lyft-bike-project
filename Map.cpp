#include "Map.h"
using namespace std;
#include <math.h>
#include <cmath>
#include <stdlib.h>  
#define pi 3.14159265358979323846

//map constructor
Map::Map(int mapWidth,int mapHeight, double mapLngLeft, double mapLngRight, double mapLatBottom) : mapWidth_(mapWidth), mapHeight_(mapHeight),
mapLngLeft_(mapLngLeft), mapLngRight_(mapLngRight),mapLatBottom_(mapLatBottom) {}


Point Map::convertGeoToPixel(double latitude, double longitude,
                  int mapWidth, // in pixels
                  int mapHeight, // in pixels
                  double mapLngLeft, // in degrees. the longitude of the left side of the map (i.e. the longitude of whatever is depicted on the left-most part of the map image)
                  double mapLngRight, // in degrees. the longitude of the right side of the map
                  double mapLatBottom) // in degrees.  the latitude of the bottom of the map
{
    double mapLatBottomRad = mapLatBottom * pi / 180;
    double latitudeRad = latitude * pi / 180;
    double mapLngDelta = (mapLngRight - mapLngLeft);

    double worldMapWidth = ((mapWidth / mapLngDelta) * 360) / (2 * pi);
    double mapOffsetY = (worldMapWidth / 2 * log((1 + sin(mapLatBottomRad)) / (1 - sin(mapLatBottomRad))));

    double x = (longitude - mapLngLeft) * (mapWidth / mapLngDelta);
    double y = mapHeight - ((worldMapWidth / 2 * log((1 + sin(latitudeRad)) / (1 - sin(latitudeRad)))) - mapOffsetY);
    Point point = {static_cast<int>(x), static_cast<int>(y)};
    return point;// the pixel x,y value of this point on the map image
}

//for visualization purposes: maginify size of the station plotted (from 1 pixel to 5*5)
void Map::Enlarge(double x, double y) {
    double startx = x - 2;
    double starty = y + 2;
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
        this->getPixel(startx + j, starty + i).h = 0;
        this->getPixel(startx + j, starty + i).s = 1;
        this->getPixel(startx + j, starty + i).l = 0.5;
        this->getPixel(startx + j, starty + i).a = 1;
        }
    }
}

//testing purpose
Map* Map::draw_single_station(double latitude, double longitude) {
    Point point = convertGeoToPixel(latitude,longitude,mapWidth_,mapHeight_,mapLngLeft_,mapLngRight_,mapLatBottom_);
    this->Enlarge(point.x, point.y);
    return this;
}

Map* Map::draw_stations(vector<vector<string>> stations) {
    //input: <<startid, start,end,startlat,startlong, endlati,endlong>> from nodes.csv that has unique nodes info
    // Point point = convertGeoToPixel(37.81013,-122.46177,1792,1418,-122.523815,-122.348528,37.701743);
    // this->Enlarge(point.x,point.y);
    for (vector<string> station : stations) {
        double station_lati = stod(station[3]);
        double station_long = stod(station[4]);
        Point point = convertGeoToPixel(station_lati,station_long,mapWidth_,mapHeight_,mapLngLeft_,mapLngRight_,mapLatBottom_);
        this->Enlarge(point.x, point.y);
    }
    return this;
}

//draw edges for the graph (full dataset or chosen dataset)
Map* Map::draw_edges(vector<vector<string>> trips, map<string, vector<string>>location_data) {
    //input: <<startid, start,end,startlat,startlong, endlati,endlong>> from final_graph_data.csv that has all the trip data
    for(vector<string> trip : trips) {
        // double start_lati = stod(trip[3]);
        // double start_long = stod(trip[4]);
        // double end_lati = stod(trip[5]);
        // double end_long = stod(trip[6]);
        string start_station = trip[1];
        string end_station = trip[2];
        double start_lati;
        double start_long;
        double end_lati;
        double end_long;
        // look for the location of starting and ending station in the map location_data
        map<string,vector<string>>::iterator lookup1 = location_data.find(start_station);
        map<string,vector<string>>::iterator lookup2 = location_data.find(end_station);
        if (lookup1 != location_data.end()) {
            vector<string> loc_data = lookup1->second;
            start_lati = stod(loc_data[0]);
            start_long = stod(loc_data[1]);
        } else {
            continue;
        }
        if (lookup2 != location_data.end()) {
            vector<string> loc_data = lookup2->second;
            end_lati = stod(loc_data[0]);
            end_long = stod(loc_data[1]);
        } else {
            continue;
        }
        Point start = convertGeoToPixel(start_lati,start_long,mapWidth_,mapHeight_,mapLngLeft_,mapLngRight_,mapLatBottom_);
        Point end = convertGeoToPixel(end_lati,end_long,mapWidth_,mapHeight_,mapLngLeft_,mapLngRight_,mapLatBottom_);
        double slope;
        if ((double)end.x - (double)start.x == 0) {
            slope = 0;
        } else {
            slope = ((double)end.y - (double)start.y)/((double)end.x - (double)start.x);
        }
        int deltax = abs(end.x - start.x);
        int deltay = abs(end.y - start.y);
        //from lower left  to upper right or from upper left to lower right or from left to right
        if ((start.x < end.x) || (start.y == end.y && start.x < end.x)) {
            for(int i = 1; i < deltax; i++) {
                int colory = floor(slope * i + start.y);
                this->getPixel(start.x + i, colory).h = 0;
                this->getPixel(start.x + i, colory).s = 0;
                this->getPixel(start.x + i, colory).l = 0;
                this->getPixel(start.x + i, colory).a = 1;
                }
        // else if ((start.x < end.x && start.y < end.y)) {
        //         for(int i = 1; i < deltax; i++) {
        //         int colory = ceil(slope * i + start.y);
        //         this->getPixel(start.x + i, colory).h = 0;
        //         this->getPixel(start.x + i, colory).s = 0;
        //         this->getPixel(start.x + i, colory).l = 0;
        //         this->getPixel(start.x + i, colory).a = 1;
        //         }
        // }
        //from upper right to lower left or from upper left to lower right or from right to left
        } else if ((start.x > end.x) || (start.y == end.y && start.x > end.x)) {
            for(int i = 1; i < deltax; i++) {
                int colory = floor(slope * -1 * i + start.y);
                this->getPixel(start.x - i, colory).h = 0;
                this->getPixel(start.x - i, colory).s = 0;
                this->getPixel(start.x - i, colory).l = 0;
                this->getPixel(start.x - i, colory).a = 1; 
            }
        //start.x = end.x , from top to bottom
        } else if (start.x == end.x && start.y < end.y) {
                for(int i = 1; i < deltay; i++) {
                    int colory = floor(slope * i + start.y);
                    this->getPixel(start.x, colory + i).h = 0;
                    this->getPixel(start.x, colory + i).s = 0;
                    this->getPixel(start.x, colory + i).l = 0;
                    this->getPixel(start.x, colory + i).a = 1; 
                } 
            // from bottom to top
        } else if(start.x == end.x && start.y > end.y) {
                for(int i = 1; i < deltay; i++) {
                    int colory = floor(slope * i + start.y);
                    this->getPixel(start.x, colory - i).h = 0;
                    this->getPixel(start.x, colory - i).s = 0;
                    this->getPixel(start.x, colory - i).l = 0;
                    this->getPixel(start.x, colory - i).a = 1; 
                    }
                }
            }
            return this;
}


Map* Map::draw_shortest_path(vector<unsigned int> shortest_path, map<string, vector<string>> location_data, map<unsigned int,string> nodemap) {
    for(unsigned int i = 0; i < shortest_path.size() - 1; i++) {
        unsigned int starts = shortest_path[i];
        string start_station = nodemap[starts];
        unsigned int ends = shortest_path[i + 1];
        string end_station = nodemap[ends];
        double start_lati;
        double start_long;
        double end_lati;
        double end_long;
        // look for the location of starting and ending station in the map location_data
        map<string,vector<string>>::iterator lookup1 = location_data.find(start_station);
        map<string,vector<string>>::iterator lookup2 = location_data.find(end_station);
        if (lookup1 != location_data.end()) {
            vector<string> loc_data = lookup1->second;
            start_lati = stod(loc_data[0]);
            start_long = stod(loc_data[1]);
        } else {
            continue;
        }
        if (lookup2 != location_data.end()) {
            vector<string> loc_data = lookup2->second;
            end_lati = stod(loc_data[0]);
            end_long = stod(loc_data[1]);
        } else {
            continue;
        }
        Point start = convertGeoToPixel(start_lati,start_long,mapWidth_,mapHeight_,mapLngLeft_,mapLngRight_,mapLatBottom_);
        Point end = convertGeoToPixel(end_lati,end_long,mapWidth_,mapHeight_,mapLngLeft_,mapLngRight_,mapLatBottom_);
        double slope;
        if ((double)end.x - (double)start.x == 0) {
            slope = 0;
        } else {
            slope = ((double)end.y - (double)start.y)/((double)end.x - (double)start.x);
        }
        int deltax = abs(end.x - start.x);
        int deltay = abs(end.y - start.y);
        //from lower left  to upper right or from upper left to lower right or from left to right
        if ((start.x < end.x) || (start.y == end.y && start.x < end.x)) {
            for(int i = 1; i < deltax; i++) {
                int colory = floor(slope * i + start.y);
                this->getPixel(start.x + i, colory).h = 240;
                this->getPixel(start.x + i, colory).s = 1;
                this->getPixel(start.x + i, colory).l = 0.5;
                this->getPixel(start.x + i, colory).a = 0.7;
                }
        // else if ((start.x < end.x && start.y < end.y)) {
        //         for(int i = 1; i < deltax; i++) {
        //         int colory = ceil(slope * i + start.y);
        //         this->getPixel(start.x + i, colory).h = 0;
        //         this->getPixel(start.x + i, colory).s = 0;
        //         this->getPixel(start.x + i, colory).l = 0;
        //         this->getPixel(start.x + i, colory).a = 1;
        //         }
        // }
        //from upper right to lower left or from upper left to lower right or from right to left
        } else if ((start.x > end.x) || (start.y == end.y && start.x > end.x)) {
            for(int i = 1; i < deltax; i++) {
                int colory = floor(slope * -1 * i + start.y);
                this->getPixel(start.x - i, colory).h = 240;
                this->getPixel(start.x - i, colory).s = 1;
                this->getPixel(start.x - i, colory).l = 0.5;
                this->getPixel(start.x - i, colory).a = 0.7; 
            }
        //start.x = end.x , from top to bottom
        } else if (start.x == end.x && start.y < end.y) {
                for(int i = 1; i < deltay; i++) {
                    int colory = floor(slope * i + start.y);
                    this->getPixel(start.x, colory + i).h = 240;
                    this->getPixel(start.x, colory + i).s = 1;
                    this->getPixel(start.x, colory + i).l = 0.5;
                    this->getPixel(start.x, colory + i).a = 0.7; 
                } 
            // from bottom to top
        } else if(start.x == end.x && start.y > end.y) {
                for(int i = 1; i < deltay; i++) {
                    int colory = floor(slope * i + start.y);
                    this->getPixel(start.x, colory - i).h = 240;
                    this->getPixel(start.x, colory - i).s = 1;
                    this->getPixel(start.x, colory - i).l = 0.5;
                    this->getPixel(start.x, colory - i).a = 0.7; 
                    }
                }
            }
            return this;
    }
