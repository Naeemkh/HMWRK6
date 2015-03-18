/*
 * Prog Tools for Scits Engrs - CIVL 8903/7903 001
 * Prof. Ricardo Taborda
 * Homework 6
 * Written by : Naeem Khoshnevis (nkhshnvs@memphis.edu)
 * https://github.com/Naeemkh/HMW5.git
 * Feb 21 , 2015
 */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <string.h>

#include "earthquake.h"
#include "station.h"

using namespace std;

enum Magnitude_Type {
	ML, Ms, Mb, Mw
};

enum months {
	January = 1,
	February,
	March,
	April,
	May,
	June,
	July,
	August,
	September,
	October,
	November,
	December
};

const int MAXSIZE = 300;

/***************************** function prototypes *************************************/

bool is_date_valid(string, int&, int&, int&);
bool is_time_valid(string &);
bool is_tzone_valid(string);
bool is_mag_valid(string);
bool is_mag_inrange(float);
bool is_net_valid(string);
bool is_station_valid(string);
bool is_band_valid(string);
bool is_instrument_valid(string);
bool is_or_valid(string);
bool read_input(ifstream&, ofstream&, Event db[MAXSIZE], int &, int &, int &,
		int &, int &);

void open_input(ifstream&, ofstream&);
void open_output(ofstream&, ofstream&, string);
void open_output(ofstream&, string);
void print_output(ofstream&, ostream&, string);
void print_output(ofstream&, ostream&, int);
void check_validate(Earthquake er_info[1], ofstream&, int &, int &, int &);
void print_header(int, int, int, Earthquake er_info[1], ofstream&);
void generate_recorded_list(Earthquake er_info[1],ofstream&, Event db[MAXSIZE], int &, int &);

string Instro_Type_to_string(Instro_Type c);
string Band_Type_to_string(Band_Type c);
string Network_Code_to_string(Network_Code c);
string uppercase(string &);
string Magnitude_Type_to_string(Magnitude_Type);

months int_to_months(int);
Network_Code string_to_Network_Code(string);
Band_Type string_to_Band_Type(string);
Instro_Type string_to_instro_Type(string);
Magnitude_Type string_to_Magnitude_Type(string);



void set_lat(Earthquake er_info[1],ofstream&, double);
void set_lon(Earthquake er_info[1],ofstream&, double);
void set_depth(Earthquake er_info[1],ofstream&, double);

void set_event_id(Earthquake er_info[1], ofstream&, string);
void set_event_date(Earthquake er_info[1], ofstream&, string);
void set_event_time(Earthquake er_info[1], ofstream&, string);
void set_time_zone(Earthquake er_info[1], ofstream&, string);
void set_event_name(Earthquake er_info[1], ofstream&, string);
void set_mag_type(Earthquake er_info[1], ofstream&, string);
void set_mag(Earthquake er_info[1], ofstream&,float);

double get_lat(Earthquake er_info[1],string);
double get_lon(Earthquake er_info[1],string);
double get_depth(Earthquake er_info[1],string);

/********************************* main function ****************************************/

int main() {

	ifstream inputfile;
	ofstream outputfile;
	ofstream errorfile;

	//string  event_date, event_time, time_zone, event_name,mag_t;

    int month = 0, day = 0, year = 0;
    int total_entry = 0, invalid_counter = 0, valid_counter = 0, total_co = 0;
    double t_lat, t_lon, t_depth;
    string t_event_id,t_event_name, t_event_date, t_event_time, t_time_zone,t_mag_t;
    float t_mag;
    
    
	Event db[MAXSIZE];
	Earthquake er_info[1];
	int number_of_events;

	open_input(inputfile, errorfile);
	print_output(errorfile, cout, "Processing input ... \n");

	inputfile >> t_event_id;
	inputfile >> t_event_date;
	inputfile >> t_event_time;
	inputfile >> t_time_zone;

	inputfile.ignore();
	getline(inputfile, t_event_name);

	inputfile >> t_lon;
	inputfile >> t_lat;
    inputfile >> t_depth;
    
    inputfile >> t_mag_t;
    inputfile >> t_mag;
    
    set_lon(er_info,errorfile,t_lon);
    set_lat(er_info,errorfile,t_lat);
    set_depth(er_info,errorfile,t_depth);
    
    set_event_id(er_info,errorfile,t_event_id);
    set_event_date(er_info,errorfile,t_event_date);
    set_event_time(er_info,errorfile,t_event_time);
    set_time_zone(er_info,errorfile,t_time_zone);
    set_event_name(er_info,errorfile,t_event_name);
    set_mag_type(er_info,errorfile,t_mag_t);
    set_mag(er_info,errorfile,t_mag);



	check_validate(er_info, errorfile, month, day, year);

	print_output(errorfile, cout, "Header read correctly! \n");
	open_output(outputfile, errorfile, "naeem.out");
	print_header(month, day, year, er_info, outputfile);

	read_input(inputfile, errorfile, db, number_of_events, total_co,
			invalid_counter, total_entry, valid_counter);

	print_output(errorfile, cout, "Total invalid entries ignored: ");
	print_output(errorfile, cout, invalid_counter);
	print_output(errorfile, cout, "\n");

	print_output(errorfile, cout, "Total valid entries read: ");
	print_output(errorfile, cout, valid_counter);
	print_output(errorfile, cout, "\n");

	print_output(errorfile, cout, "Total signal names produced: ");
	print_output(errorfile, cout, total_co);
	print_output(errorfile, cout, "\n");
	print_output(errorfile, cout, "Finished !");
	print_output(errorfile, cout, "\n");

	generate_recorded_list(er_info, outputfile, db, number_of_events, total_co);

	outputfile.close();
	return 0;
}

/********************************* function definition **********************************/

void open_input(ifstream & inputfile, ofstream & errorfile) {

	string inputfilename;
	cout << "Enter input filename: ";
	cin >> inputfilename;

	inputfile.open(inputfilename.c_str());

	if (!inputfile.is_open()) {

		open_output(errorfile, "naeem.log");
		print_output(errorfile, cout, "Cannot open input file: ");
		print_output(errorfile, cout, inputfilename);
		print_output(errorfile, cout, "\n");
		errorfile.close();
		exit(1);
	} else {
		open_output(errorfile, "naeem.log");
		print_output(errorfile, cout, "Opening file: ");
		print_output(errorfile, cout, inputfilename);
		print_output(errorfile, cout, "\n");
	}
}

void open_output(ofstream& outputfile, ofstream& errorfile,
		string outputfilename) {

	outputfile.open(outputfilename.c_str());
	if (!outputfile.is_open()) {

		open_output(errorfile, "naeem.err");
		print_output(errorfile, cout, "Cannot open input file: ");
		print_output(errorfile, cout, outputfilename);
		print_output(errorfile, cout, "\n");
		errorfile.close();
		exit(1);
	}
}

void open_output(ofstream& errorfile, string errorfilename) {

	errorfile.open(errorfilename.c_str());
	if (!errorfile.is_open()) {

		cout << "Cannot open error file: " << errorfilename;

	}
}

void print_output(ofstream & output1, ostream & output2, string string1) {

	output1 << string1;
	output2 << string1;
}

void print_output(ofstream & output1, ostream & output2, int string1) {

	output1 << string1;
	output2 << string1;
}

bool is_date_valid(string event_date, int & month, int & day, int & year) {

	int s = event_date.size();
	int k = 0, delim;
	int pos[2] = { 0 };
	bool date_flag = 0;

	int p1 = event_date.find("/");
	int p2 = event_date.find("-");

	if (p1 < 0 && (p2 < 0)) {
		date_flag = 1;
	} else if (p1 > 0 && p2 > 0) {
		date_flag = 1;
	} else if (p1 > 0 && p2 < 0) {
		delim = 1;
	} else if (p1 < 0 && p2 > 0) {
		delim = 2;
	}

	if (date_flag == 0) {

		for (int i = 0; i < s; i++) {
			switch (delim) {
			case 1:
				if (strncmp(&event_date[i], "/", 1) == 0) {
					pos[k] = i;
					k = k + 1;
				}
				break;
			case 2:
				if (strncmp(&event_date[i], "-", 1) == 0) {
					pos[k] = i;
					k = k + 1;
				}
				break;
			}
		}
	}

	if (k != 2) {
		date_flag = 1;
	} else {
		stringstream m, d, y;

		m << event_date.substr(0, pos[0]);
		d << event_date.substr(pos[0] + 1, pos[1] - pos[0] - 1);
		y << event_date.substr(pos[1] + 1, s - pos[1]);

		m >> month;
		d >> day;
		y >> year;

		if (month < 0 || month > 12) {
			date_flag = 1;
		}
		if (day < 0 || day > 31) {
			date_flag = 1;
		}
		if (year < 1900 || year > 2015) {
			date_flag = 1;
		}
	}
	return date_flag;
}

bool is_time_valid(string & event_time) {

	int s = event_time.size();
	int k = 0;
	int pos[2] = { 0 };
	bool time_flag = 0;

	int p1 = event_time.find(":");
	int p2 = event_time.find(".");

	if (p1 < 0 && (p2 < 0)) {
		time_flag = 1;
	}
	if (time_flag == 0) {

		for (int i = 0; i < s; i++) {
			if (strncmp(&event_time[i], ":", 1) == 0) {
				pos[k] = i;
				k = k + 1;
			}
		}
	}

	if (k != 2 || pos[1] > p2) {
		time_flag = 1;
	} else {

		stringstream hh;
		stringstream mm;
		stringstream ss;

		int hour, min;
		double second;

		hh << event_time.substr(0, pos[0]);
		mm << event_time.substr(pos[0] + 1, pos[1] - pos[0] - 1);
		ss << event_time.substr(pos[1] + 1, s - pos[1]);

		hh >> hour;
		mm >> min;
		ss >> second;

		if (hour < 0 || hour > 24) {
			time_flag = 1;
		}
		if (min < 0 || min > 59) {
			time_flag = 1;
		}
		if (second < 0 || second >= 60) {
			time_flag = 1;
		}
	}
	return time_flag;
}

bool is_tzone_valid(string time_zone) {

	int s = time_zone.size();
	stringstream os1;
	bool tzone_flag = 0;

	if (s != 3) {
		tzone_flag = 1;
	}

	return tzone_flag;
}

bool is_mag_valid(string mag_t) {
	string ss = uppercase(mag_t);
	return ((ss == "ML") || (ss == "MS") || (ss == "MB") || (ss == "MW"));
}

bool is_mag_inrange(float mag) {
	return (mag >= 0);
}

string Magnitude_Type_to_string(Magnitude_Type c) {
	switch (c) {
	case ML:
		return "ML";
	case Ms:
		return "Ms";
	case Mb:
		return "Mb";
	case Mw:
		return "Mw";
	}
	// It should never get here!!
	exit (EXIT_FAILURE);
}

Magnitude_Type string_to_Magnitude_Type(string s) {

	string ss = uppercase(s);

	if (ss == "ML")
		return ML;
	if (ss == "MS")
		return Ms;
	if (ss == "MB")
		return Mb;
	if (ss == "MW")
		return Mw;

	// It should never get here!!
	exit (EXIT_FAILURE);
}

months int_to_months(int a) {

	if (a == 1)
		return January;
	if (a == 2)
		return February;
	if (a == 3)
		return March;
	if (a == 4)
		return April;
	if (a == 5)
		return May;
	if (a == 6)
		return June;
	if (a == 7)
		return July;
	if (a == 8)
		return August;
	if (a == 9)
		return September;
	if (a == 10)
		return October;
	if (a == 11)
		return November;
	if (a == 12)
		return December;

	// It should never get here!!
	exit (EXIT_FAILURE);
}

string month_to_string(months c) {
	switch (c) {
	case January:
		return "January";
	case February:
		return "February";
	case March:
		return "March";
	case April:
		return "April";
	case May:
		return "May";
	case June:
		return "June";
	case July:
		return "July";
	case August:
		return "August";
	case September:
		return "September";
	case October:
		return "October";
	case November:
		return "November";
	case December:
		return "December";
	}
	// It should never get here!!
	exit (EXIT_FAILURE);
}

string uppercase(string & s) {
	string result = s;
	for (int i = 0; i < (int) result.size(); i++)
		result[i] = toupper(result[i]);
	return result;
}

void check_validate(Earthquake er_info[1], ofstream & errorfile, int & month,
		int & day, int & year) {

	string event_date = er_info[0].event_date;
	string event_time = er_info[0].event_time;
	float mag = er_info[0].mag;
	string mag_t = er_info[0].mag_t;
	string time_zone = er_info[0].time_zone;

	if (is_date_valid(event_date, month, day, year) != 0) {
		print_output(errorfile, cout, "Date format is not valid. \n");
		errorfile.close();
		exit (EXIT_FAILURE);
	} else if (is_time_valid(event_time) != 0) {
		print_output(errorfile, cout, "Time format is not valid. \n");
		errorfile.close();
		exit (EXIT_FAILURE);
	} else if (is_tzone_valid(time_zone) != 0) {
		print_output(errorfile, cout, "Time zone format is not valid. \n");
		errorfile.close();
		exit (EXIT_FAILURE);
	} else if (is_mag_valid(mag_t) != 1) {
		print_output(errorfile, cout, "Magnitude type is not valid. \n");
		errorfile.close();
		exit (EXIT_FAILURE);
	} else if (is_mag_inrange(mag) != 1) {
		print_output(errorfile, cout, "Magnitude is not in valid range. \n");
		errorfile.close();
		exit (EXIT_FAILURE);
	}

}

void print_header(int month, int day, int year, Earthquake er_info[1],
		ofstream& outputfile) {
    
    string lat,lon,depth;
    
	outputfile << "# " << day << " " << month_to_string(int_to_months(month))
			<< " " << year << " " << er_info[0].event_time << " "
			<< er_info[0].time_zone << " "
			<< Magnitude_Type_to_string(
					string_to_Magnitude_Type(er_info[0].mag_t)) << " "
			<< er_info[0].mag << " " << er_info[0].event_name << " " << "["
			<< er_info[0].event_id << "]" << "(" << get_lon(er_info,lon) << ", "
			<< get_lat(er_info,lat) << ", " << get_depth(er_info,depth) << ")" << "\n";

}

bool read_input(ifstream& inputfile, ofstream& errorfile, Event db[MAXSIZE],
		int & size, int & total_co, int & invalid_counter, int & total_entry,
		int & valid_counter) {

	string nt_name, st_name, b_type, Ins_type, orientation;
	stringstream os1;
	size = 0;
	total_entry = 1;

	while (size < MAXSIZE) {

		bool save_record_flag = 1;
		if (!(inputfile >> nt_name >> st_name >> b_type >> Ins_type
				>> orientation))
			break;

		if (is_net_valid(nt_name) == 0) {

			stringstream ks;
			string kk;
			ks << total_entry;
			ks >> kk;
			print_output(errorfile, cout, "Entry # ");
			print_output(errorfile, cout, kk);
			print_output(errorfile, cout, " Ignored. ");
			print_output(errorfile, cout, "Invalid Network. \n");
			save_record_flag = 0;
		}

		if (is_station_valid(st_name) == 0) {

			stringstream ks;
			string kk;
			ks << total_entry;
			ks >> kk;

			print_output(errorfile, cout, "Entry # ");
			print_output(errorfile, cout, kk);
			print_output(errorfile, cout, " Ignored. ");
			print_output(errorfile, cout, "Invalid station code. \n");
			save_record_flag = 0;
		}

		if (is_band_valid(b_type) == 0) {

			stringstream ks;
			string kk;
			ks << total_entry;
			ks >> kk;

			print_output(errorfile, cout, "Entry # ");
			print_output(errorfile, cout, kk);
			print_output(errorfile, cout, " Ignored. ");
			print_output(errorfile, cout, "Invalid band type. \n");
			save_record_flag = 0;
		}

		if (is_instrument_valid(Ins_type) == 0) {

			stringstream ks;
			string kk;
			ks << total_entry;
			ks >> kk;
			print_output(errorfile, cout, "Entry # ");
			print_output(errorfile, cout, kk);
			print_output(errorfile, cout, " Ignored. ");
			print_output(errorfile, cout, "Invalid instrument type. \n");
			save_record_flag = 0;
		}

		if (is_or_valid(orientation) == 0) {

			stringstream ks;
			string kk;
			ks << total_entry;
			ks >> kk;
			print_output(errorfile, cout, "Entry # ");
			print_output(errorfile, cout, kk);
			print_output(errorfile, cout, " Ignored. ");
			print_output(errorfile, cout, "Invalid orientation. \n");
			save_record_flag = 0;
		}

		if (save_record_flag == 1) {

			db[size].nt_name = string_to_Network_Code(nt_name);
			db[size].st_name = st_name;
			db[size].b_type = string_to_Band_Type(b_type);
			db[size].Ins_type = string_to_instro_Type(Ins_type);
			db[size].orientation = orientation;

			total_co = total_co + orientation.size();

			size++;

		} else {
			invalid_counter = invalid_counter + 1;
		}
		total_entry++;
	}
	valid_counter = total_entry - invalid_counter;
	return true;
}

bool is_net_valid(string nt_name) {
	bool net_flag = 0;
	string ss = uppercase(nt_name);
	if (((ss == "CE") || (ss == "CI") || (ss == "FA") || (ss == "NP")
			|| (ss == "WR")) && (ss == nt_name)) {
		net_flag = 1;
	}
	return net_flag;
}

bool is_band_valid(string b_type) {
	bool band_flag = 0;
	string ss = uppercase(b_type);
	if ((ss == "LONG-PERIOD") || (ss == "SHORT-PERIOD")
			|| (ss == "BROADBAND")) {
		band_flag = 1;
	}
	return band_flag;
}

bool is_instrument_valid(string Ins_type) {
	bool ins_flag = 0;
	string ss = uppercase(Ins_type);
	if ((ss == "HIGH-GAIN") || (ss == "LOW-GAIN") || (ss == "ACCELEROMETER")) {
		ins_flag = 1;
	}
	return ins_flag;
}

bool is_station_valid(string st_name) {
	bool st_flag = 0;
	string ss = uppercase(st_name);
	int sn = st_name.size();

	if (sn == 3 && ss == st_name) {
		st_flag = 1;
	}
	if (sn == 5) {
		st_flag = 1;
		for (int i = 0; i == 4; i++) {
			if (isdigit(st_name[i] == 0)) {
				st_flag = 0;
			}
		}
	}
	return st_flag;
}

bool is_or_valid(string orientation) {

	bool or_flag = 0;
	int kk = 1;
	int ors = orientation.size();

	if (ors < 4 && ors > 0) {
		if (isdigit(orientation[0])) {
			for (int i = 0; i < ors; i++) {
				if (strncmp(&orientation[i], "1", 1) == 0
						|| strncmp(&orientation[i], "2", 1) == 0
						|| strncmp(&orientation[i], "3", 1) == 0) {
					kk = kk + 1;
				}
			}
		} else {

			for (int i = 0; i < ors; i++) {
				if (strncmp(&orientation[i], "N", 1) == 0
						|| strncmp(&orientation[i], "E", 1) == 0
						|| strncmp(&orientation[i], "Z", 1) == 0) {
					kk = kk + 1;
				}
			}
		}
	}

	if (kk - 1 == ors) {
		or_flag = 1;
	}
	if (ors == 2 && orientation[0] == orientation[1]) {
		or_flag = 0;
	}
	if (ors == 2
			&& (orientation[0] == orientation[1]
					|| orientation[0] == orientation[2]
					|| orientation[1] == orientation[2])) {
		or_flag = 0;
	}

	return or_flag;
}

Network_Code string_to_Network_Code(string s) {

	string ss = uppercase(s);

	if (ss == "CE")
		return CE;
	if (ss == "CI")
		return CI;
	if (ss == "FA")
		return FA;
	if (ss == "NP")
		return NP;
	if (ss == "WR")
		return WR;

	// It should never get here!!
	exit (EXIT_FAILURE);

}

string Network_Code_to_string(Network_Code c) {
	switch (c) {
	case CE:
		return "CE";
	case CI:
		return "CI";
	case FA:
		return "FA";
	case NP:
		return "NP";
	case WR:
		return "WR";
	}
	// It should never get here!!
	exit (EXIT_FAILURE);
}

Band_Type string_to_Band_Type(string s) {

	string ss = uppercase(s);

	if (ss == "LONG-PERIOD")
		return Longperiod;
	if (ss == "SHORT-PERIOD")
		return Shortperiod;
	if (ss == "BROADBAND")
		return Broadband;

	// It should never get here!!
	exit (EXIT_FAILURE);

}

string Band_Type_to_string(Band_Type c) {
	switch (c) {
	case Longperiod:
		return "L";
	case Shortperiod:
		return "B";
	case Broadband:
		return "H";

	}

	// It should never get here!!
	exit (EXIT_FAILURE);
}

Instro_Type string_to_instro_Type(string s) {

	string ss = uppercase(s);

	if (ss == "HIGH-GAIN")
		return HighGain;
	if (ss == "LOW-GAIN")
		return LowGain;
	if (ss == "ACCELEROMETER")
		return Accelerometer;

	// It should never get here!!
	exit (EXIT_FAILURE);

}

string Instro_Type_to_string(Instro_Type c) {
	switch (c) {
	case HighGain:
		return "H";
	case LowGain:
		return "L";
	case Accelerometer:
		return "N";

	}
	// It should never get here!!
	exit (EXIT_FAILURE);
}

void generate_recorded_list(Earthquake er_info[1], ofstream& outputfile, Event db[MAXSIZE], int & size,
		int & total_co) {

	outputfile << total_co << "\n";

	for (int i = 0; i < size; i++) {
		int sc = db[i].orientation.size();

		for (int j = 0; j < sc; j++) {
			outputfile << er_info[0].event_id << Network_Code_to_string(db[i].nt_name) << "."
					<< db[i].st_name << "." << Band_Type_to_string(db[i].b_type)
					<< Instro_Type_to_string(db[i].Ins_type)
					<< db[i].orientation[j] << "\n";

		}

	}
}

void set_lat(Earthquake er_info[1], ofstream& errorfile, double lat){
    if ( -60 <= lat && lat <= 60){
        er_info[0].lat = lat;
    } else {
        print_output(errorfile, cout, "Latitude is not valid. \n");
        errorfile.close();
        exit(1);
    }
}

double get_lat(Earthquake er_info[1],string lat){
    return er_info[0].lat;
}

void set_lon(Earthquake er_info[1], ofstream& errorfile, double lon){
    if ( -180 <= lon && lon <= 180){
        er_info[0].lon = lon;
    } else {
        print_output(errorfile, cout, "Longitude is not valid. \n");
        errorfile.close();
        exit(1);
    }
}

double get_lon(Earthquake er_info[1],string lon){
    return er_info[0].lon;
}

void set_depth(Earthquake er_info[1], ofstream& errorfile, double depth){
    if ( 0 <= depth ){
        er_info[0].depth = depth;
    } else {
        print_output(errorfile, cout, "Depth of earthquake is not valid. \n");
        errorfile.close();
        exit(1);
    }
}

double get_depth(Earthquake er_info[1],string depth){
    return er_info[0].depth;
}

void set_event_id(Earthquake er_info[1], ofstream& errorfile, string event_id){
    
        er_info[0].event_id = event_id;

}


void set_event_date(Earthquake er_info[1], ofstream& errorfile, string event_date){
    
    er_info[0].event_date = event_date;
    
}

void set_event_time(Earthquake er_info[1], ofstream& errorfile, string event_time){
    
    er_info[0].event_time = event_time;
    
}

void set_time_zone(Earthquake er_info[1], ofstream& errorfile, string time_zone){
    
    er_info[0].time_zone = time_zone;
    
}

void set_event_name(Earthquake er_info[1], ofstream& errorfile, string event_name){
    
    er_info[0].event_name = event_name;
    
}

void set_mag_type(Earthquake er_info[1], ofstream& errorfile, string mag_t){
    
    er_info[0].mag_t = mag_t;
    
}


void set_mag(Earthquake er_info[1], ofstream& errorfile, float  mag){
    
    er_info[0].mag = mag;
    
}


