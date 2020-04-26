#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

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

// Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    vector <int> pids=LinuxParser::Pids();
    int pidSize = pids.size();
    //processes_.clear();
    //std::cout << "\n***pidSize**" <<pidSize <<"*******\n";
    //std::cout << "\n**processes_.size()***" <<processes_.size() <<"*******\n";

    if (int(processes_.size())!=int(pidSize)){
        processes_.resize(pidSize);
    }
    //std::cout << "\n**processes_.size()***" <<processes_.size() <<"*******\n";

    for (int i=0; i<pidSize;i++){
        int pid = pids[i];
        processes_.at(i).pid =  (pid);
        processes_.at(i).command = LinuxParser::Command(pid);
        processes_.at(i).ram = LinuxParser::Ram(pid);
        processes_.at(i).user = LinuxParser::Uid(pid);
        processes_.at(i).cpuUtil = float(LinuxParser::ActiveJiffies(pid))/float(cpu_.totaltime);
        processes_.at(i).uptime  = LinuxParser::UpTime(pid);
    }
    std::sort(processes_.begin(),processes_.end());
    std::reverse(processes_.begin(),processes_.end());
    //std::cout << processes_.size() <<"*******\n";
    return processes_;
     }  

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