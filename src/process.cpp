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

Process::Process(int pid){
    this->pID = pid;
    this->ram = std::stof(this->Ram());    
}

// TODO: Return this process's ID
int Process::Pid() { return pID; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    float cpuTime{0}, cpuUsage, seconds, totalTime;
    
    vector<string> cpuData = LinuxParser::CpuUtilization(pID); 
    cpuTime = std::stof(cpuData[13]) + std::stof(cpuData[14]);
    totalTime = cpuTime + std::stof(cpuData[15]) + std::stof(cpuData[16]);
    seconds = LinuxParser::UpTime() - (std::stof(cpuData[21]) / sysconf(_SC_CLK_TCK));
    cpuUsage = ((totalTime/sysconf(_SC_CLK_TCK)) / seconds);
    
    return cpuUsage;
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pID); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pID); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pID); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pID); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { return this->ram < a.ram; }