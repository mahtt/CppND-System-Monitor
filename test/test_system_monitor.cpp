#include <cassert>

#include "../include/format.h"

void Tests() {
    //ElapsedTime() tests
    assert(Format::ElapsedTime(60) == "0:1:0");
    assert(Format::ElapsedTime(1783714) == "495:28:34");
}