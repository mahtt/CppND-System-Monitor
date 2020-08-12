#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;


int Process::Pid() { return pid_; }

float Process::CpuUtilization() { 
    vector<string> times = LinuxParser::CpuUtilization(pid_); 
    int total_time = std::stoi(times[0]) + std::stoi(times[1]) + std::stoi(times[2]) + std::stoi(times[3]);
    const int seconds = LinuxParser::UpTime(pid_);
    float cpu_usage = (((float)total_time/(float)sysconf(_SC_CLK_TCK))/(float)seconds);
    return cpu_usage;}

string Process::Command() { 
    return LinuxParser::Command(pid_); }


string Process::Ram() { return LinuxParser::Ram(pid_); }

string Process::User() { 
    return LinuxParser::User(pid_); }

long int Process::UpTime() { 
    return LinuxParser::UpTime(pid_); }

bool Process::operator<(Process const& a) const { 
    return (LinuxParser::CpuUtilization(pid_) > LinuxParser::CpuUtilization(a.pid_) ? true : false); }