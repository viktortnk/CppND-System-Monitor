#pragma once

struct CpuTicks {
  long int user, nice, system, idle, iowait, irq, softirq, steal, guest,
      guest_nice;

  CpuTicks diff(const CpuTicks& rhs) {
    CpuTicks c;
    c.user = user - rhs.user;
    c.nice = nice - rhs.nice;
    c.system = system - rhs.system;
    c.idle = idle - rhs.idle;
    c.iowait = iowait - rhs.iowait;
    c.irq = irq - rhs.irq;
    c.softirq = softirq - rhs.softirq;
    c.steal = steal - rhs.steal;
    c.guest = guest - rhs.guest;
    c.guest_nice = guest_nice - rhs.guest_nice;
    return c;
  }

  float utilisation() {
    auto total_cpu_time =
        user + nice + system + idle + iowait + irq + softirq + steal;
    auto total_idle_time = idle + iowait;
    auto total_usage_time = total_cpu_time - total_idle_time;

    return float((total_usage_time * 1.0) / total_cpu_time);
  }
};
