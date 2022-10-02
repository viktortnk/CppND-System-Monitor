#include "processor.h"

#include "cpu_ticks.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  CpuTicks new_value = LinuxParser::CpuTicks();
  CpuTicks diff = new_value.diff(prev_);
  prev_ = new_value;
  return diff.utilisation();
}
