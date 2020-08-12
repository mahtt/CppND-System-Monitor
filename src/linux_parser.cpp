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

vector<string> LinuxParser::CpuUtilization(int pid) { 
  string line, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, val1, val2, val3, val4;
  vector<string> times;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if(filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> x1 >> x2 >> x3 >> x4 >> x5 >> x6 >> x7 >> x8 >> x9 >> x10 >> x11 >> x12 >> x13 >> val1 >> val2 >> val3 >> val4;
    times.push_back(val1);
    times.push_back(val2);
    times.push_back(val3);
    times.push_back(val4);
  }
  return times; }

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

string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if(stream.is_open()){
    std::getline(stream,line);
    return line;
  }
  return "Error in LinuxParser::Command()"; }

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

long LinuxParser::UpTime(int pid) { 
 string line, value;
  long upTime;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if(filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for(int i = 0; i < 22; i++){
      linestream >> value;
    }
    upTime = std::stol(value) / sysconf(_SC_CLK_TCK);
    return UpTime() - upTime;
  }
  return -1;
  }