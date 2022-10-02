#include "ncurses_display.h"
#include "system.h"

int main() {
  System system;
  system.LoadSystemInfo();
  NCursesDisplay::Display(system);
}
