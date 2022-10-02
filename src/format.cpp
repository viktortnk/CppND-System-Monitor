#include "format.h"

#include <string>

#include "stdio.h"

using std::string;

string Format::ElapsedTime(long seconds) {
  int total_minutes = seconds / 60;
  int hours = total_minutes / 60;
  int minutes = total_minutes % 60;
  int secs = seconds % 60;
  char res[20];
  sprintf(res, "%02d:%02d:%02d", hours, minutes, secs);
  return res;
}
