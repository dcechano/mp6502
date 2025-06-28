#include <array>
#include <cstdint>
#include <iostream>

#include "./bus.hpp"
#include "./nes6502.hpp"

Bus::Bus() {
  std::cout << "Bus initialized" << std::endl;
  std::cout << "Bus size: " << sizeof(Bus) << std::endl;
  _iram = std::make_unique<std::array<uint8_t, RAM_SIZE>>();
  _iram->fill(0);
  _ppu_rgstr.fill(0);
  _apu_io_rgstr.fill(0);
  _apu_test_rgstr.fill(0);
  _cpu.set_bus(*this);
}

Bus::~Bus() {}

uint8_t Bus::read(uint16_t addr) {
  if (addr < 0x2000) {
    return (*_iram)[addr & 0x07FF];
  } else if (addr < 0x4000) {
    return _ppu_rgstr[addr & 0x0007];
  } else if (addr < 0x4018) {
    return _apu_io_rgstr[addr - 0x4000];
  } else if (addr < 0x4020) {
    return _apu_test_rgstr[addr - 0x4018];
  }
  return 0;
}

void Bus::write(uint16_t addr, uint8_t data) {
  if (addr < 0x2000) {
    (*_iram)[addr & 0x07FF] = data;
  } else if (addr < 0x4000) {
    _ppu_rgstr[addr & 0x0007] = data;
  } else if (addr < 0x4018) {
    _apu_io_rgstr[addr - 0x4000] = data;
  } else if (addr < 0x4020) {
    _apu_test_rgstr[addr - 0x4018] = data;
  }
}
