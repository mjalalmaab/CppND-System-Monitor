#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>
#include <iostream>

using std::string;
using std::vector;

// Done: Return the aggregate CPU utilization
float Processor::Utilization() { 
   vector<string> Cputerms =   LinuxParser::CpuUtilization();
   int user = std::stoi(Cputerms[1]); // normal processes executing in user mode
   int nice = std::stoi(Cputerms[2]); //niced processes executing in user mode
   int system = std::stoi(Cputerms[3]);     //system: processes executing in kernel mode
   int idle   = std::stoi(Cputerms[4]);     //idle: twiddling thumbs
   int iowait = std::stoi(Cputerms[5]);     //iowait: In a word, iowait stands for waiting for I/O to complete. 
   int irq     = std::stoi(Cputerms[6]);    //irq: servicing interrupts
   int softirq = std::stoi(Cputerms[7]);    //softirq: servicing softirqs
   int steal = std::stoi(Cputerms[8]);      //steal: involuntary wait
   int guest = std::stoi(Cputerms[9]);      //guest: running a normal guest
   int guest_nice = std::stoi(Cputerms[10]); //guest_nice: running a niced guest



    // Guest time is already accounted in usertime
    int usertime = user - guest;                     //# As you see here, it subtracts guest from user time
    //std::cout << "usertime" << usertime <<"\n";  
    int nicetime = nice - guest_nice;                // # and guest_nice from nice time
    // Fields existing on kernels >= 2.6
    // (and RHEL's patched kernel 2.4...)
    int idlealltime = idle + iowait;                 //# ioWait is added in the idleTime
    //std::cout << "idlealltime" << idlealltime <<"\n";  
    int systemalltime = system + irq + softirq;
    int virtalltime = guest + guest_nice;
    int totaltime = usertime + nicetime + systemalltime + idlealltime + steal + virtalltime;
    float cpuPercetn = float(totaltime-idlealltime)/float(totaltime);
    return cpuPercetn;
 }