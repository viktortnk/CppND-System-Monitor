#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "cpu_ticks.h"

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

 private:
  CpuTicks prev_;
};

#endif
