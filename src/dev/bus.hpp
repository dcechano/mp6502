#pragma once
#include <array>
#include <cstdint>
#include <iostream>
#include <memory>

#include "./nes6502.hpp"

constexpr uint16_t                                     RAM_SIZE = 2048;
constexpr uint16_t                                     PPU_REG_SIZE = 8;
constexpr uint16_t                                     APU_IO_REG_SIZE = 24;
constexpr uint16_t                                     APU_TEST_REG_SIZE = 8;

typedef std::unique_ptr<std::array<uint8_t, RAM_SIZE>> InternalRAM;

/*
 *                                      NES6502 Memory Map
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
  NES6502                                _cpu; // NES6502
  APU                                    _apu; // Audio Processing Unit
  InternalRAM                            _iram; // 2KB internal RAM on heap
  std::array<uint8_t, PPU_REG_SIZE>      _ppu_rgstr; // PPU registers
  std::array<uint8_t, APU_IO_REG_SIZE>   _apu_io_rgstr; // APU I/O registers
  std::array<uint8_t, APU_TEST_REG_SIZE> _apu_test_rgstr; // APU test registers

public:
  Bus();
  ~Bus();
  uint8_t read(uint16_t addr);
  void    write(uint16_t addr, uint8_t data);
};