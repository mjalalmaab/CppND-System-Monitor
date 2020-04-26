#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>


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
  string os, kernel, version;
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




// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string memTotal_s;
  string memAvail_s;
  string line;
  string terms;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream>>terms >> memTotal_s;
    std::getline(stream, line);
    std::getline(stream, line);
    std::istringstream linestream2(line);
    linestream2>>terms >> memAvail_s;
  }
  int memUsed = std::stoi(memTotal_s)-std::stoi(memAvail_s);
  float memUtil = float(memUsed)/float(std::stoi(memTotal_s));
  return memUtil;
 }




//Read and return the system uptime
long LinuxParser::UpTime() { 
  long uptime;
  string line;
  string uptime_s;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream>>uptime_s;
  }
  uptime = std::stoi(uptime_s);
  return uptime;}





// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }




// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  long int procActiveJiff;
  string line;
  vector <string> terms;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
      std::getline(stream, line);
    std::istringstream linestream(line);
    string buffer;
    while(linestream>>buffer){
      terms.push_back(buffer);
    }; 
    procActiveJiff = std::stoi(terms[13])+std::stoi(terms[14])+std::stoi(terms[15])+std::stoi(terms[16]);
  }
  
  return procActiveJiff; }



// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }



// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }




// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpuUtil;
  string line;
  string terms;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream>>terms){
      cpuUtil.push_back(terms);
    }
  } 
  return cpuUtil; 
  }



// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int totalProc;
  string totalProc_s;
  string line;
  string terms;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    for(int i=1; i<10; i++){
    std::getline(stream, line);
    }
    std::istringstream linestream(line);
    linestream>>terms>>totalProc_s; 
  } 
  totalProc = std::stoi(totalProc_s);
  return totalProc;}



// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  int runproc;
  string runproc_s;
  string line;
  string terms;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    for(int i=1; i<10; i++){
    std::getline(stream, line);
    }
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream>>terms>>runproc_s; 
  } 
  runproc = std::stoi(runproc_s);
  return runproc; }

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string commandLine;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> commandLine;
  }
  return commandLine; }

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string ram;
  string line;
  string terms;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    for(int i=1; i<19; i++){
    std::getline(stream, line);
    }
    std::istringstream linestream(line);
    linestream>>terms>>ram; 
  } 
  std::stringstream stream2;
  float ram_mb_f = std::stof(ram)/1024;
  stream2 << std::fixed << std::setprecision(2) << ram_mb_f;
  std::string ram_mb = stream2.str();
  return ram_mb; }

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string uid;
  string line;
  string terms;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    for(int i=1; i<10; i++){
    std::getline(stream, line);
    }
    std::istringstream linestream(line);
    linestream>>terms>>uid; 
    //std::cout << uid << "*****\n";
  }
  return uid; }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  long int uptime;
  string line;
  vector <string> terms;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
      std::getline(stream, line);
    std::istringstream linestream(line);
    string buffer;
    while(linestream>>buffer){
      terms.push_back(buffer);
    }
  }
  uptime = std::stoi(terms[21]);
  float uptime_f = float(uptime)/sysconf(_SC_CLK_TCK);
  uptime = long(uptime_f); 
  return uptime; 
  }