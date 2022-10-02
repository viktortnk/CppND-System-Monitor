#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "cpu_ticks.h"
#include "process.h"
#include "process_cpu_stats.h"

using std::stof;
using std::stoi;
using std::string;
using std::to_string;
using std::vector;

using StringMap = std::unordered_map<string, string>;

namespace {

// trim from start (in place)
static inline void ltrim(std::string& s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
          }));
}

static inline std::pair<string, string> split2(const string& s,
                                               const char& delimiter) {
  size_t pos = s.find(delimiter);
  if (pos != string::npos) {
    return {s.substr(0, pos), s.substr(pos + 1)};
  } else {
    return {s.substr(), ""};
  }
}

StringMap FileToMap(const string& path, const char& delimiter) {
  StringMap map;
  std::ifstream filestream{path};

  if (filestream.is_open()) {
    string line;
    while (std::getline(filestream, line)) {
      auto kv = split2(line, delimiter);
      auto key = kv.first;
      auto value = kv.second;
      ltrim(value);
      map[key] = value;
    }
  }

  return map;
}

string FileContent(const string& path) {
  std::ifstream f(path);
  if (f.is_open()) {
    std::stringstream buffer;
    buffer << f.rdbuf();
    return buffer.str();
  } else {
    return string{};
  }
}

CpuTicks ParseCpuTicks(const string& s) {
  CpuTicks c;
  if (s.rfind("cpu ", 0) != string::npos) {
    string xs = s.substr(4);
    ltrim(xs);
    std::stringstream ss{xs};
    while (ss >> c.user >> c.nice >> c.system >> c.idle >> c.iowait >> c.irq >>
           c.softirq >> c.steal >> c.guest >> c.guest_nice) {
    }
  }
  return c;
}

CpuTicks LoadCpuTicks(const string& path) {
  CpuTicks c;
  std::ifstream f(path);
  if (f.is_open()) {
    string s;
    std::getline(f, s);
    c = ParseCpuTicks(s);
  }
  return c;
}

string proc_pid_path(int pid, const string& path) {
  return LinuxParser::kProcDirectory + to_string(pid) + path;
}

vector<string> tokenize(const string& s, const char& delimiter = ' ') {
  std::stringstream linestream(s);
  vector<string> tokens;
  string temp;
  while (std::getline(linestream, temp, delimiter)) {
    tokens.push_back(temp);
  }
  return tokens;
}

}  // namespace

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

float LinuxParser::MemoryUtilization() {
  auto map = FileToMap(kProcDirectory + kMeminfoFilename, ':');
  string mem_total_str = map["MemTotal"];
  string mem_free_str = map["MemFree"];
  auto mem_total = stof(split2(mem_total_str, ' ').first);
  auto mem_free = stof(split2(mem_free_str, ' ').first);

  return (mem_total - mem_free) / mem_total;
}

long LinuxParser::UpTime() {
  string uptime_str = FileContent(kProcDirectory + kUptimeFilename);
  std::stringstream ss{uptime_str};
  double uptime;
  ss >> uptime;
  return long(uptime);
}

CpuTicks LinuxParser::CpuTicks() {
  return LoadCpuTicks(kProcDirectory + kStatFilename);
}

int LinuxParser::TotalProcesses() {
  auto map = FileToMap(kProcDirectory + kStatFilename, ' ');
  return stoi(map["processes"]);
}

int LinuxParser::RunningProcesses() {
  auto map = FileToMap(kProcDirectory + kStatFilename, ' ');
  return stoi(map["procs_running"]);
}

ProcessCpuStats LinuxParser::CpuStats(int pid) {
  ProcessCpuStats s;
  auto content = FileContent(proc_pid_path(pid, LinuxParser::kStatFilename));
  auto tokens = tokenize(content);
  if (tokens.size() > 21) {
    s.utime = stol(tokens[13]);
    s.stime = stol(tokens[14]);
    s.cutime = stol(tokens[15]);
    s.cstime = stol(tokens[16]);
    s.starttime = stol(tokens[21]);
  }
  return s;
}

string LinuxParser::Command(int pid) {
  return FileContent(proc_pid_path(pid, kCmdlineFilename));
}

// NOTE: using 'VmRSS' (not virtual memory size 'VmSize')
string LinuxParser::Ram(int pid) {
  auto map = FileToMap(proc_pid_path(pid, kStatusFilename), ':');
  std::stringstream ss(map["VmRSS"]);
  int ram_kb;
  if (ss >> ram_kb) {
    return to_string(ram_kb / 1024);
  } else {
    return "";
  }
}

string LinuxParser::Uid(int pid) {
  auto map = FileToMap(proc_pid_path(pid, kStatusFilename), ':');
  auto uid_str = map["Uid"];
  std::istringstream ss{uid_str};
  string uid;
  ss >> uid;
  return uid;
}

string LinuxParser::User(int pid) {
  string uid = Uid(pid);

  string line;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      vector<string> tokens = tokenize(line, ':');
      if (tokens.size() > 2 && tokens[2] == uid) {
        return tokens[0];
      }
    }
  }

  return "uid:" + uid;
}

long LinuxParser::UpTime(int pid) {
  auto uptime_str = FileContent(proc_pid_path(pid, kStatFilename));
  std::stringstream ss{uptime_str};
  string value;
  int i = 0;
  while (ss >> value && i < 22) {
    i++;
  }
  return std::stol(value);
}
