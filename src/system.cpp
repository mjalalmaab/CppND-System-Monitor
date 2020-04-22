#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"
#include <iostream>

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { 
    //Processor cpu_;
    return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    vector <int> pids=LinuxParser::Pids();
    int pidSize = pids.size();
    //processes_.clear();
    for (int i=0; i<pidSize;i++){
        Process process;
        int pid = pids[i];
        process.pid =  (pid);
        process.command = LinuxParser::Command(pid);
        process.ram = LinuxParser::Ram(pid);
        processes_.push_back(process);
    }
    return processes_; }

// Return the system's kernel identifier (string)
std::string System::Kernel() { return kernel_; }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return os_; }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }