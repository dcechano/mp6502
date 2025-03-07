#pragma once
#include <array>
#include <cstdint>
#include <iostream>

#include "cpu.hpp"

/*
 *                                      CPU Memory Map
 * +-----------------------------------------------------------------------------------------------+
 * | Address Range  |   Size   |                  Description                  |     Catagory      |
 * |===============================================================================================|
 * | $0000-$07FF    | $0800    | 2KB internal RAM                              |                   |
 * |----------------|----------|-----------------------------------------------|                   |
 * | $0800-$0FFF    | $0800    |                                               |                   |
 * |----------------|----------|                                               |    System RAM     | 
 * | $1000-$17FF    | $0800    | Mirror of $0000-$07FF                         |                   |
 * |----------------|----------|                                               |                   |
 * | $1800-$1FFF    | $0800    |                                               |                   |
 * |----------------|----------|-----------------------------------------------|-------------------|
 * | $2000-$2007    | $0008    | PPU registers                                 |                   |
 * |----------------|----------|-----------------------------------------------|                   |
 * | $2008-$3FFF    | $1FF8    | Mirror of $2000-$2007 (repeats every 8 bytes) |                   |
 * |----------------|----------|-----------------------------------------------|   I/O Registers   |
 * | $4000-$4017    | $0018    | APU and I/O registers                         |                   |
 * |----------------|----------|-----------------------------------------------|                   |
 * | $4018-$401F    | $0008    | APU test mode registers                       |                   |
 * |----------------|----------|-----------------------------------------------|-------------------|
 * | $4020-$FFFF    | $BFE0    | PRG ROM, PRG RAM, and mapper registers        |  Cartridge Space  |
 * +-----------------------------------------------------------------------------------------------+
 */
class Bus {
private:
  CPU cpu;
  std::array<uint8_t, 2048> ram;
  std::array<uint8_t, 8> ppu_registers;
  std::array<uint8_t, 24> apu_io_registers;
  std::array<uint8_t, 8> apu_test_registers;

public:
  Bus() {
    std::cout << "Bus initialized" << std::endl;
    ram.fill(0);
    ppu_registers.fill(0);
    apu_io_registers.fill(0);
    apu_test_registers.fill(0);
  }

  uint8_t read(uint16_t addr) {
    if (addr < 0x2000) {
      return ram[addr & 0x07FF];
    } else if (addr < 0x4000) {
      return ppu_registers[addr & 0x0007];
    } else if (addr < 0x4018) {
      return apu_io_registers[addr - 0x4000];
    } else if (addr < 0x4020) {
      return apu_test_registers[addr - 0x4018];
    }
    return 0;
  }

  void write(uint16_t addr, uint8_t data) {
    if (addr < 0x2000) {
      ram[addr & 0x07FF] = data;
    } else if (addr < 0x4000) {
      ppu_registers[addr & 0x0007] = data;
    } else if (addr < 0x4018) {
      apu_io_registers[addr - 0x4000] = data;
    } else if (addr < 0x4020) {
      apu_test_registers[addr - 0x4018] = data;
    }
  }
};