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
	assert(totalMemory == 8032340); //specific to my machine
  

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
vector<string> LinuxParser::CpuUtilization() { return {}; }

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
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { 
  
  return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }