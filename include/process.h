#ifndef PROCESS_H
#define PROCESS_H

#include <string>

#include "process_cpu_stats.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  void Pid(int pid);
  void User(std::string user);
  void Command(std::string command);
  void UpTime(long int uptime);
  void Ram(std::string ram);
  void CpuStats(ProcessCpuStats s);

  // TODO: Declare any necessary private members
 private:
  int pid_;
  std::string user_;
  std::string command_;
  long int uptime_;
  std::string ram_;
  ProcessCpuStats cpu_stats_;
};

#endif
