#pragma once

#include <array>
#include <cstdint>
#include <fstream>
#include <string>

class Reader {
private:
  std::array<uint8_t, 0x2000> _rom; // 8KB ROM
  std::fstream _file;

public:
  Reader(const std::string &filename) {
    _file.open(filename, std::ios::binary);
    _file.read(reinterpret_cast<char *>(_rom.data()), _rom.size());
    _file.close();
  }
  uint8_t read(uint16_t addr) const {
    return _rom[addr];
  }
};