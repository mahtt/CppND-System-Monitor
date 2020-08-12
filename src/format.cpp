#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    string secondsConverted = "";
    int hours, minutes = 0;
    hours = seconds / 3600;
    seconds = seconds % 3600;
    minutes = seconds / 60;
    seconds = seconds % 60;
    secondsConverted = std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds);
    char x[10];
    sprintf(x,"%02d:%02d:%02ld", hours, minutes, seconds);
    secondsConverted= x;
    return secondsConverted; }