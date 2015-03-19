#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <string.h>

using namespace std;

void print_output(ofstream&, ostream&, string);
void print_output(ofstream&, ostream&, int);
void open_input(ifstream&, ofstream&);
void open_output(ofstream&, ofstream&, string);
void open_output(ofstream&, string);

