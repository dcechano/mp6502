#pragma once
#include <iostream>

#include "./apu.hpp"
#include <bitset>
#include <cstdint>

class CPU {
public:
  CPU() {
    _pc = 0x0000;
    _stp = 0xFD;
    _acc = 0;
    _irx = 0;
    _iry = 0;
    _proc_stat = 0;
    _pstat_r.reset();
    std::cout << "CPU initialized" << std::endl;
  }

private:
  uint16_t _pc; // Program Counter
  uint8_t _stp; // Stack Pointer
  uint8_t _acc; // Accumulator
  uint8_t _irx; // Index Register X
  uint8_t _iry; // Index Register Y
  uint8_t _proc_stat; // Processor Status
  /*
   * 7  bit  0
   * ---- ----
   * NVss DIZC
   * |||| ||||
   * |||| |||+- Carry
   * |||| |||`- Zero
   * |||| ||`- Interrupt Disable
   * |||| |`- Decimal Mode
   * |||| `- Break
   * |||` `- Unused
   * ||` `- Overflow
   */
  std::bitset<8> _pstat_r; // Processor Status Register
};
