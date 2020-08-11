#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <cassert>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  int linesWithRelevantInfos = 4;
	int lineCount = 0;
	float totalMemory, freeMemory, availableMemory, buffers;
	string line;
	std::ifstream stream(kProcDirectory + kMeminfoFilename);
	while(stream.is_open() && lineCount < linesWithRelevantInfos){
		std::getline(stream,line);
		string category;
		float value; 
		std::istringstream linestream(line);
		linestream >> category >> value;

		if(category == "MemTotal:") totalMemory = value;
		else if(category == "MemFree:") freeMemory = value;
		else if(category == "MemAvailable:") availableMemory = value;
    else if(category == "Buffers:") buffers = value;
    else{
      std::cout << "Error in MemoryUtilization() ! \n" << category << std::endl; //TODO: throw exception.
    }
		lineCount++;
	}
  return (totalMemory - freeMemory) / totalMemory; }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
	int upTime;
	std::ifstream stream("/proc/uptime");
	if(stream.is_open()){
		std::getline(stream,line);
		std::istringstream linestream(line);
		linestream >> upTime;
	}
  return upTime; }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
float LinuxParser::CpuUtilization(int pid) { 
  string line, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q;
  vector<int> times;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);

  if(filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> a >> b >> c >> d >> e >> f >> g >> h >> i >> j >> k >> l >> m >> n >> o >> p >> q;
    times.push_back(std::stoi(n));
    times.push_back(std::stoi(o));
    times.push_back(std::stoi(p));
    times.push_back(std::stoi(q));
  }
  int total_time = times[0] + times[1] + times[2] + times[3];
  int seconds = LinuxParser::UpTime() - (LinuxParser::UpTime(pid) / sysconf(_SC_CLK_TCK));
  float cpu_usage = (((float)total_time/(float)sysconf(_SC_CLK_TCK))/(float)seconds);
  return cpu_usage; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, category;
	int totalProcesses;
	std::ifstream stream(kProcDirectory + kStatFilename);
	while(stream.is_open()){
		std::getline(stream,line);
		std::istringstream linestream(line);
		linestream >> category >> totalProcesses;
		if(category == "processes") break;
	} 
  return totalProcesses;
  } 

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line, category;
	int procs_running;
	std::ifstream stream(kProcDirectory + kStatFilename);
	while(stream.is_open()){
		std::getline(stream,line);
		std::istringstream linestream(line);
		linestream >> category >> procs_running;
		if(category == "procs_running") break;
	}
  return procs_running; } 

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if(stream.is_open()){
    std::getline(stream,line);
    return line;
  }
  return "Error in LinuxParser::Command()"; }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line, key, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);  
      linestream >> key >> value;
      if(key == "VmSize:"){
        int mb = std::stoi(value) / 1024;
        return std::to_string(mb);
      }
    }
  }
  return "Error LinuxParser::Ram()"; }

// TODO: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line, key, uid;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  while(stream.is_open()){
    std::getline(stream,line);
    std::istringstream linestream(line);
    linestream >> key >> uid; 
    if(key == "Uid:") break;
  }
  return uid; }

// TODO: Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string line, user, dump, userid; 
  string uid = LinuxParser::Uid(pid);
  std::ifstream stream(kPasswordPath);
  while(stream.is_open()){
    std::getline(stream,line);
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    linestream >> user >> dump >> userid;
    if(userid == uid) return user;
  }

  return "ErrorGettingUser"; }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line;
  long clk, upTime;
  clk = sysconf(_SC_CLK_TCK);
  int posOfUpTimeValue = 22;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if(stream.is_open()){
	  std::getline(stream,line);
	  std::istringstream linestream(line);
	  for(int i = 0; i < posOfUpTimeValue; i++) {
		  linestream >> upTime;
	}
}
	return upTime / clk; 
  }