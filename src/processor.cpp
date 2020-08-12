#include "processor.h"
#include <string>
#include <fstream>
#include <iostream>
#include <regex>

using std::string;

float Processor::Utilization() { 
    int user, nice, system, idle, iowait,irq, softirq, steal, guest, guest_nice, nonidle,total,totald,idled;
	string line, cpu;
	std::ifstream stream("/proc/stat");
	if(stream.is_open()){
		std::getline(stream,line);
		std::istringstream linestream(line);
		linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
	}
    previdle_ = previdle_ + previowait_;
    idle = idle + iowait;

    prevnonidle_ = prevuser_ + prevnice_ + prevsystem_ + previrq_ + prevsoftirq_ + prevsteal_;
    nonidle = user + nice + system + irq + softirq + steal;

    prevtotal_ = previdle_ + prevnonidle_;
    total = idle + nonidle;

    totald = total - prevtotal_;
    idled = idle - previdle_;
    return (float) (totald - idled) / totald; }