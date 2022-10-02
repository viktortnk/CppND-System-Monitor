#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return cpu_stats_.utilisation(); }

string Process::Command() { return command_; }

string Process::Ram() { return ram_; }

string Process::User() { return user_; }

long int Process::UpTime() { return uptime_; }

bool Process::operator<(Process const& a) const { return ram_ > a.ram_; }

void Process::Pid(int pid) { pid_ = pid; };
void Process::User(std::string user) { user_ = user; };
void Process::Command(std::string command) { command_ = command; };
void Process::UpTime(long int uptime) { uptime_ = uptime; };
void Process::Ram(std::string ram) { ram_ = ram; }
void Process::CpuStats(ProcessCpuStats s) { cpu_stats_ = s; }
