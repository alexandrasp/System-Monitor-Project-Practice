#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string header;

  float freeMemory, totalMemory;

  std::ifstream memFile(kProcDirectory + kMeminfoFilename);
  if (memFile.is_open()) {
    while (std::getline(memFile, line)) {
      std::istringstream lineStream(line);
      lineStream >> header;

      if (header == "MemTotal:") {
        lineStream >> totalMemory;
      }
      if (header == "MemFree:") {
        lineStream >> freeMemory;
      }
    }
  }
  return (totalMemory - freeMemory) / totalMemory;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime = 0;

  std::ifstream uptimeFile(kProcDirectory + kUptimeFilename);

  if (uptimeFile.is_open()) {
    std::string line;
    if (std::getline(uptimeFile, line)) {
      std::istringstream lineStream(line);
      lineStream >> uptime;
    }
  }
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpuInfo{};
  string line;
  string header;

  std::ifstream cpuFile(kProcDirectory + kStatFilename);
  if (cpuFile.is_open()) {
    while (std::getline(cpuFile, line)) {
      std::istringstream lineStream(line);
      lineStream >> header;
      if (header == "cpu")  // Match CPU line
      {
        string value;
        while (!lineStream.eof()) {
          lineStream >> value;
          cpuInfo.push_back(value);
        }
        break;
      }
    }
  }

  return cpuInfo;
}

vector<string> LinuxParser::CpuUtilization(int pid) {
  vector<string> procData{};

  std::ifstream statFile(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (statFile.is_open()) {
    std::string line;
    std::getline(statFile, line);

    std::string value;
    std::istringstream lineStream(line);
    while (!lineStream.eof()) {
      lineStream >> value;
      procData.push_back(value);
    }
  }
  return procData;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int totalProcess;
  string line;
  string header;

  std::ifstream memInfoFile(kProcDirectory + kStatFilename);
  if (memInfoFile.is_open()) {
    while (std::getline(memInfoFile, line)) {
      std::istringstream lineStream(line);
      lineStream >> header;

      if(header == "processes") {
        lineStream >> totalProcess;
        return totalProcess;
      }
    }  
  }
  return -1;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  int processRun;
  string line;
  string header;

  std::ifstream memInfoFile(kProcDirectory + kStatFilename);
  if (memInfoFile.is_open()) {
    while (std::getline(memInfoFile, line)) {
      std::istringstream lineStream(line);
      lineStream >> header;

      if(header == "procs_running") {
        lineStream >> processRun;
        return processRun;
      }
    }  
  }
  return -1;
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string command;
  std::ifstream commandLine(kProcDirectory + std::to_string(pid) + kCmdlineFilename);

  if (commandLine.is_open()) {
    std::string line;
    if (std::getline(commandLine, line)) {
      std::istringstream lineStream(line);
      lineStream >> command;
    }
  }
  return command;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string ram;
  string line, header;

  std::ifstream statusFile(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (statusFile.is_open()) {
    while (std::getline(statusFile, line)) {
      std::istringstream lineStream(line);
      lineStream >> header;
      if (header == "VmSize:")  // Match Ram line
      {
        lineStream >> ram;
        ram = std::to_string(std::stol(ram)/1000);
        return ram;
      }
    }
  }
  return "0";
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string uid;
  string line, header;

  std::ifstream statusFile(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (statusFile.is_open()) {
    while (std::getline(statusFile, line)) {
      std::istringstream lineStream(line);
      lineStream >> header;
      if (header == "Uid:")  // Match Uid line
      {
        lineStream >> uid;
        return uid;
      }
    }
  }
  return "0";
 }

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string user;
  string line, header, password, uidUser;

  string uid = LinuxParser::Uid(pid);

  std::ifstream passwordFile(kPasswordPath);
  if (passwordFile.is_open()) {
    while (std::getline(passwordFile, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream lineStream(line);
      
      lineStream >> user;
      lineStream >> password;
      lineStream >> uidUser;
      
      if (uidUser == uid){
        return user;
      } 
    }
  }
  return "None";
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  vector<string> procData = LinuxParser::CpuUtilization(pid);
  long startTime = std::stol(procData[ProcessStates::kStartTime]);

  return LinuxParser::UpTime() - (startTime / sysconf(_SC_CLK_TCK));
}
