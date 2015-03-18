using namespace std;

enum Network_Code {
	CE, CI, FA, NP, WR
};

enum Band_Type {
	Longperiod, Shortperiod, Broadband
};

enum Instro_Type {
	HighGain, LowGain, Accelerometer
};


struct Event {
	Network_Code nt_name;
	string st_name;
	Band_Type b_type;
	Instro_Type Ins_type;
	string orientation;
};

