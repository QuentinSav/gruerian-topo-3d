#include "outing.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <cmath>

using namespace std;

long double toRadians(const long double & degree)
{
    long double one_deg = (M_PI) / 180;
    return (one_deg * degree);
}

Outing::Outing(const string& file_path) {
    parse_gpxfile(file_path);
}

void Outing::add_trkpt(trkpt& tracking_point) 
{
    m_track_segment.push_back(tracking_point);
}

 
bool Outing::parse_gpxfile(const string& filePath) {

    ifstream file(filePath);
    trkpt next_tracking_point;

    if (!file.is_open()) {
        cout << "Failed to open GPX file." << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (line.find("<name>") != string::npos) {
            size_t start = line.find(">") + 1;
            size_t end = line.rfind("<");
            m_name = line.substr(start, end - start);
        }
        if (line.find("<trkpt") != string::npos) {
            size_t start_lat = line.find("lat=\"") + 5;
            size_t end_lat = line.rfind("\" lon=");
            size_t start_lon = line.find("lon=\"") + 5;
            size_t end_lon = line.rfind("\">");
            next_tracking_point.lat = stof(line.substr(start_lat, end_lat - start_lat));
            next_tracking_point.lon = stof(line.substr(start_lon, end_lon - start_lon));
            
            add_trkpt(next_tracking_point);
        }
    }

    file.close();
    return true;
};

void Outing::print_info() {
    cout << m_name << endl;
    for (auto track_point : m_track_segment) {
        cout << setprecision(6) << fixed << "lat = " << track_point.lat << ", lon = " << track_point.lon << endl;
    }
}

double Outing::compute_total_distance(){
    long double dist = 0;

    for (int i=0; i<m_track_segment.size()-1; ++i) {
        dist += acos(sin(toRadians(m_track_segment[i].lat))*sin(toRadians(m_track_segment[i+1].lat))+cos(toRadians(m_track_segment[i].lat))*cos(toRadians(m_track_segment[i+1].lat))*cos(toRadians(m_track_segment[i+1].lon)-toRadians(m_track_segment[i].lon)))*6371;
    }

    cout << "Total distance = " << dist << endl;
    return dist;
}