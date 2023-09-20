#ifndef OUTING_H
#define OUTING_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <cmath>

using namespace std;



long double toRadians(const long double & degree);

class Outing {
private:
    string m_metadata;
    string m_name;
    
    struct trkpt {
        long double lat; 
        long double lon;
    };
    vector<trkpt> m_track_segment;

    void add_trkpt(trkpt& tracking_point);
    bool parse_gpxfile(const string& filePath);

public:

    Outing(const string& file_path);
    void print_info();
    double compute_total_distance();
};

#endif // OUTING_H