#include "system.h"

#include <unistd.h>

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric
criteria "The student will be able to extract and display basic data about the
system."

You need to properly format the uptime. Refer to the comments mentioned in
format. cpp for formatting the uptime.*/

Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
Process pid_to_process(int pid, long uptime) {
  Process p;
  p.Pid(pid);
  p.Command(LinuxParser::Command(pid));
  p.UpTime(LinuxParser::UpTime(pid));
  p.User(LinuxParser::User(pid));
  p.Ram(LinuxParser::Ram(pid));
  auto cpu_stats = LinuxParser::CpuStats(pid);
  cpu_stats.total_uptime = uptime;
  p.CpuStats(cpu_stats);
  return p;
}

vector<Process>& System::Processes() {
  vector<int> pids = LinuxParser::Pids();
  vector<Process> processes;
  long uptime = LinuxParser::UpTime();
  std::transform(pids.begin(), pids.end(), std::back_inserter(processes),
                 [uptime](auto& pid) { return pid_to_process(pid, uptime); });
  std::sort(processes.begin(), processes.end());
  processes_ = processes;
  return processes_;
}

std::string System::Kernel() { return kernel_; }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return os_; }

int System::RunningProcesses() { return 1; }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }

void System::LoadSystemInfo() {
  os_ = LinuxParser::OperatingSystem();
  kernel_ = LinuxParser::Kernel();
}
