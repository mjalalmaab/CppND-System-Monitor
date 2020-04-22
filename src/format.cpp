#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int min = seconds/60;
    int sec = seconds % 60;
    int hr  = min/60;
    min = min % 60;

    string sec_st = std::to_string(sec);
    string min_st = std::to_string(min);
    string hr_st  = std::to_string(hr);

    string sec_string = hr_st + ":" + min_st + ":" + sec_st;
    return sec_string; }