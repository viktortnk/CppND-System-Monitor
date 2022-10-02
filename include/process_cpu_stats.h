#pragma once

#include <unistd.h>

#include <iostream>
struct ProcessCpuStats {
  long int utime, stime, cutime, cstime, starttime, total_uptime;

  ProcessCpuStats diff(const ProcessCpuStats& rhs) {
    ProcessCpuStats c;
    c.utime = utime - rhs.utime;
    c.stime = stime - rhs.stime;
    c.cutime = cutime - rhs.cutime;
    c.cstime = cstime - rhs.cstime;
    c.starttime = starttime - rhs.starttime;
    c.total_uptime = total_uptime - c.total_uptime;

    return c;
  }

  float utilisation() {
    auto total_time = utime + stime + cutime + cstime;
    float hertz = sysconf(_SC_CLK_TCK);
    // std::cout << total << std::endl;
    auto seconds = total_uptime - (starttime / hertz);
    if (seconds > 0) {
      auto cpu_usage = ((total_time / hertz) / seconds);
      return cpu_usage;
    } else {
      return 0.0;
    }
  }
};
