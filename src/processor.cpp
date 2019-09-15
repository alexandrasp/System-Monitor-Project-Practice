#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    //get aggreaget data
    std::vector<std::string> cpuStates = LinuxParser::CpuUtilization();

    //handle cpu data
    int user = std::stoi(cpuStates[LinuxParser::CPUStates::kUser_]),
        nice = std::stoi(cpuStates[LinuxParser::CPUStates::kNice_]),
        system = std::stoi(cpuStates[LinuxParser::CPUStates::kSystem_]),
        irq = std::stoi(cpuStates[LinuxParser::CPUStates::kIRQ_]),
        softirq = std::stoi(cpuStates[LinuxParser::CPUStates::kSoftIRQ_]),
        steal = std::stoi(cpuStates[LinuxParser::CPUStates::kSteal_]),
        idle = std::stoi(cpuStates[LinuxParser::CPUStates::kIdle_]),
        iowait = std::stoi(cpuStates[LinuxParser::CPUStates::kIOwait_]);

    int cpuNow = user + nice + system + idle + iowait + irq + softirq + steal;
    int cpuIdle = idle + iowait;

    int diffCpuTime = cpuNow - totalCpuTime;
    int diffIdleTime = cpuIdle - totalIdleTime;
    float cpuUsageTime = (diffCpuTime - diffIdleTime) / (float)diffCpuTime;

    this->totalCpuTime = cpuNow;
    this->totalIdleTime = cpuIdle;

    return cpuUsageTime;
}