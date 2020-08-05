#include "ncurses_display.h"
#include "system.h"
#include "../test/test_system_monitor.cpp"

int main() {
  Tests();
  System system;
  NCursesDisplay::Display(system);
}