#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <string.h>

using namespace std;

struct Earthquake {

    double lat;
    double lon;
    double depth;
    string event_id;
    string event_date;
    string event_time;
    string time_zone;
    string event_name;
    string mag_t;
    float mag;

};

void set_lat(Earthquake er_info[1], ofstream&, double);
void set_lon(Earthquake er_info[1], ofstream&, double);
void set_depth(Earthquake er_info[1], ofstream&, double);
void set_event_id(Earthquake er_info[1], ofstream&, string);
void set_event_date(Earthquake er_info[1], ofstream&, string);
void set_event_time(Earthquake er_info[1], ofstream&, string);
void set_time_zone(Earthquake er_info[1], ofstream&, string);
void set_event_name(Earthquake er_info[1], ofstream&, string);
void set_mag_type(Earthquake er_info[1], ofstream&, string);
void set_mag(Earthquake er_info[1], ofstream&, float);

double get_lat(Earthquake er_info[1], string);
double get_lon(Earthquake er_info[1], string);
double get_depth(Earthquake er_info[1], string);

string get_event_id(Earthquake er_info[1], string);
string get_event_date(Earthquake er_info[1], string);
string get_event_time(Earthquake er_info[1], string);
string get_time_zone(Earthquake er_info[1], string);
string get_event_name(Earthquake er_info[1], string);
string get_mag_type(Earthquake er_info[1], string);
float get_mag(Earthquake er_info[1], string);
