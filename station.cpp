#include "station.h"
#include "io_print_handler.h"

void set_nt_name(Event db[MAXSIZE], int size, Network_Code nt_name) {

    db[size].nt_name = nt_name;
}

Network_Code get_nt_name(Event db[MAXSIZE], int size, string nt_name) {

    return db[size].nt_name;
}

void set_st_name(Event db[MAXSIZE], int size, string st_name) {
    db[size].st_name = st_name;
}

string get_st_name(Event db[MAXSIZE], int size, string st_name) {
    return db[size].st_name;
}

void set_b_type(Event db[MAXSIZE], int size, Band_Type b_type) {
    db[size].b_type = b_type;
}

Band_Type get_band_type(Event db[MAXSIZE], int size, string b_type) {
    return db[size].b_type;
}

void set_Ins_type(Event db[MAXSIZE], int size, Instro_Type Ins_type) {

    db[size].Ins_type = Ins_type;
}

Instro_Type get_Ins_type(Event db[MAXSIZE], int size, string Ins_type) {
    return db[size].Ins_type;
}

void set_orientation(Event db[MAXSIZE], int size, string orientation) {

    db[size].orientation = orientation;
}

string get_orientation(Event db[MAXSIZE], int size, string orientation) {
    return db[size].orientation;
}
