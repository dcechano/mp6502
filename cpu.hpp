#pragma once
#include <iostream>

#include "apu.hpp"

class CPU {
public:
  APU apu;
  CPU() { std::cout << "CPU initialized" << std::endl; };
};
