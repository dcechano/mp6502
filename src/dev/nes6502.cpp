#include "./nes6502.hpp"
#include "./bus.hpp"
#include <cstdint>
#include <iostream>
#include <stdckdint.h>
#include <vector>

NES6502::NES6502() {
  _pc = 0x0000;
  _stp = 0xFF;
  _acc = 0;
  _irx = 0;
  _iry = 0;
  _pstat_r.reset();
  _instr = {
      {0x00,  &NES6502::IMM,     &NES6502::BRK, 7},
      {0x01, &NES6502::INDX,     &NES6502::ORA, 6},
      {0x02,  &NES6502::IMP, &NES6502::INVALID, 2},
      {0x03, &NES6502::INDX, &NES6502::INVALID, 8},
      {0x04,  &NES6502::ZP0, &NES6502::INVALID, 3},
      {0x05,  &NES6502::ZP0,     &NES6502::ORA, 3},
      {0x06,  &NES6502::ZP0,     &NES6502::ASL, 5},
      {0x07,  &NES6502::ZP0, &NES6502::INVALID, 5},
      {0x08,  &NES6502::IMP,     &NES6502::PHP, 3},
      {0x09,  &NES6502::IMM,     &NES6502::ORA, 2},
      {0x0A,  &NES6502::ACC,     &NES6502::ASL, 2},
      {0x0B,  &NES6502::IMM, &NES6502::INVALID, 2},
      {0x0C,  &NES6502::ABS, &NES6502::INVALID, 4},
      {0x0D,  &NES6502::ABS,     &NES6502::ORA, 4},
      {0x0E,  &NES6502::ABS,     &NES6502::ASL, 6},
      {0x0F,  &NES6502::ABS, &NES6502::INVALID, 6},
      {0x10,  &NES6502::REL,     &NES6502::BPL, 2},
      {0x11, &NES6502::INDY,     &NES6502::ORA, 5},
      {0x12,  &NES6502::IMP, &NES6502::INVALID, 2},
      {0x13, &NES6502::INDY, &NES6502::INVALID, 8},
      {0x14,  &NES6502::ZPX, &NES6502::INVALID, 4},
      {0x15,  &NES6502::ZPX,     &NES6502::ORA, 4},
      {0x16,  &NES6502::ZPX,     &NES6502::ASL, 6},
      {0x17,  &NES6502::ZPX, &NES6502::INVALID, 6},
      {0x18,  &NES6502::IMP,     &NES6502::CLC, 2},
      {0x19, &NES6502::ABSY,     &NES6502::ORA, 4},
      {0x1A,  &NES6502::IMP, &NES6502::INVALID, 2},
      {0x1B, &NES6502::ABSY, &NES6502::INVALID, 7},
      {0x1C, &NES6502::ABSX, &NES6502::INVALID, 4},
      {0x1D, &NES6502::ABSX,     &NES6502::ORA, 4},
      {0x1E, &NES6502::ABSX,     &NES6502::ASL, 7},
      {0x1F, &NES6502::ABSX, &NES6502::INVALID, 7},
      {0x20,  &NES6502::ABS,     &NES6502::JSR, 6},
      {0x21, &NES6502::INDX,     &NES6502::AND, 6},
      {0x22,  &NES6502::IMP, &NES6502::INVALID, 2},
      {0x23, &NES6502::INDX, &NES6502::INVALID, 8},
      {0x24,  &NES6502::ZP0,     &NES6502::BIT, 3},
      {0x25,  &NES6502::ZP0,     &NES6502::AND, 3},
      {0x26,  &NES6502::ZP0,     &NES6502::ROL, 5},
      {0x27,  &NES6502::ZP0, &NES6502::INVALID, 5},
      {0x28,  &NES6502::IMP,     &NES6502::PLP, 4},
      {0x29,  &NES6502::IMM,     &NES6502::AND, 2},
      {0x2A,  &NES6502::ACC,     &NES6502::ROL, 2},
      {0x2B,  &NES6502::IMM, &NES6502::INVALID, 2},
      {0x2C,  &NES6502::ABS,     &NES6502::BIT, 4},
      {0x2D,  &NES6502::ABS,     &NES6502::AND, 4},
      {0x2E,  &NES6502::ABS,     &NES6502::ROL, 6},
      {0x2F,  &NES6502::ABS, &NES6502::INVALID, 6},
      {0x30,  &NES6502::REL,     &NES6502::BMI, 2},
      {0x31, &NES6502::INDY,     &NES6502::AND, 5},
      {0x32,  &NES6502::IMP, &NES6502::INVALID, 2},
      {0x33, &NES6502::INDY, &NES6502::INVALID, 8},
      {0x34,  &NES6502::ZPX, &NES6502::INVALID, 4},
      {0x35,  &NES6502::ZPX,     &NES6502::AND, 4},
      {0x36,  &NES6502::ZPX,     &NES6502::ROL, 6},
      {0x37,  &NES6502::ZPX, &NES6502::INVALID, 6},
      {0x38,  &NES6502::IMP,     &NES6502::SEC, 2},
      {0x39, &NES6502::ABSY,     &NES6502::AND, 4},
      {0x3A,  &NES6502::IMP, &NES6502::INVALID, 2},
      {0x3B, &NES6502::ABSY, &NES6502::INVALID, 7},
      {0x3C, &NES6502::ABSX, &NES6502::INVALID, 4},
      {0x3D, &NES6502::ABSX,     &NES6502::AND, 4},
      {0x3E, &NES6502::ABSX,     &NES6502::ROL, 7},
      {0x3F, &NES6502::ABSX, &NES6502::INVALID, 7},
      {0x40,  &NES6502::IMP,     &NES6502::RTI, 6},
      {0x41, &NES6502::INDX,     &NES6502::EOR, 6},
      {0x42,  &NES6502::IMP, &NES6502::INVALID, 2},
      {0x43, &NES6502::INDX, &NES6502::INVALID, 8},
      {0x44,  &NES6502::ZP0, &NES6502::INVALID, 3},
      {0x45,  &NES6502::ZP0,     &NES6502::EOR, 3},
      {0x46,  &NES6502::ZP0,     &NES6502::LSR, 5},
      {0x47,  &NES6502::ZP0, &NES6502::INVALID, 5},
      {0x48,  &NES6502::IMP,     &NES6502::PHA, 3},
      {0x49,  &NES6502::IMM,     &NES6502::EOR, 2},
      {0x4A,  &NES6502::ACC,     &NES6502::LSR, 2},
      {0x4B,  &NES6502::IMM, &NES6502::INVALID, 2},
      {0x4C,  &NES6502::ABS,     &NES6502::JMP, 3},
      {0x4D,  &NES6502::ABS,     &NES6502::EOR, 4},
      {0x4E,  &NES6502::ABS,     &NES6502::LSR, 6},
      {0x4F,  &NES6502::ABS, &NES6502::INVALID, 6},
      {0x50,  &NES6502::REL,     &NES6502::RTS, 2},
      {0x51, &NES6502::INDY,     &NES6502::EOR, 5},
      {0x52,  &NES6502::IMP, &NES6502::INVALID, 2},
      {0x53, &NES6502::INDY, &NES6502::INVALID, 8},
      {0x54,  &NES6502::ZPX, &NES6502::INVALID, 4},
      {0x55,  &NES6502::ZPX,     &NES6502::EOR, 4},
      {0x56,  &NES6502::ZPX,     &NES6502::LSR, 6},
      {0x57,  &NES6502::ZPX, &NES6502::INVALID, 6},
      {0x58,  &NES6502::IMP,     &NES6502::CLI, 2},
      {0x59, &NES6502::ABSY,     &NES6502::EOR, 4},
      {0x5A,  &NES6502::IMP, &NES6502::INVALID, 2},
      {0x5B, &NES6502::ABSY, &NES6502::INVALID, 7},
      {0x5C, &NES6502::ABSX, &NES6502::INVALID, 4},
      {0x5D, &NES6502::ABSX,     &NES6502::EOR, 4},
      {0x5E, &NES6502::ABSX,     &NES6502::LSR, 7},
      {0x5F, &NES6502::ABSX, &NES6502::INVALID, 7},
      {0x60,  &NES6502::IMP,     &NES6502::RTS, 6},
      {0x61, &NES6502::INDX,     &NES6502::ADC, 6},
      {0x62,  &NES6502::IMP, &NES6502::INVALID, 2},
      {0x63, &NES6502::INDX, &NES6502::INVALID, 8},
      {0x64,  &NES6502::ZP0, &NES6502::INVALID, 3},
      {0x65,  &NES6502::ZP0,     &NES6502::ADC, 3},
      {0x66,  &NES6502::ZP0,     &NES6502::ROL, 5},
      {0x67,  &NES6502::ZP0, &NES6502::INVALID, 5},
      {0x68,  &NES6502::IMP,     &NES6502::PLA, 4},
      {0x69,  &NES6502::IMM,     &NES6502::ADC, 2},
      {0x6A,  &NES6502::ACC,     &NES6502::ROR, 2},
      {0x6B,  &NES6502::IMM, &NES6502::INVALID, 2},
      {0x6C,  &NES6502::IND,     &NES6502::JMP, 5},
      {0x6D,  &NES6502::ABS,     &NES6502::ADC, 4},
      {0x6E,  &NES6502::ABS,     &NES6502::ROL, 6},
      {0x6F,  &NES6502::ABS, &NES6502::INVALID, 6},
      {0x70,  &NES6502::REL,     &NES6502::BVS, 2},
      {0x71, &NES6502::INDY,     &NES6502::ADC, 5},
      {0x72,  &NES6502::IMP, &NES6502::INVALID, 2},
      {0x73, &NES6502::INDY, &NES6502::INVALID, 8},
      {0x74,  &NES6502::ZPX, &NES6502::INVALID, 4},
      {0x75,  &NES6502::ZPX,     &NES6502::ADC, 4},
      {0x76,  &NES6502::ZPX,     &NES6502::ROL, 6},
      {0x77,  &NES6502::ZPX, &NES6502::INVALID, 6},
      {0x78,  &NES6502::IMP,     &NES6502::SEI, 2},
      {0x79, &NES6502::ABSY,     &NES6502::ADC, 4},
      {0x7A,  &NES6502::IMP, &NES6502::INVALID, 2},
      {0x7B, &NES6502::ABSY, &NES6502::INVALID, 7},
      {0x7C, &NES6502::ABSX, &NES6502::INVALID, 4},
      {0x7D, &NES6502::ABSX,     &NES6502::ADC, 4},
      {0x7E, &NES6502::ABSX,     &NES6502::ROL, 7},
      {0x7F, &NES6502::ABSX, &NES6502::INVALID, 7},
      {0x80,  &NES6502::IMM, &NES6502::INVALID, 2},
      {0x81, &NES6502::INDX,     &NES6502::STA, 6},
      {0x82,  &NES6502::IMM, &NES6502::INVALID, 2},
      {0x83, &NES6502::INDX, &NES6502::INVALID, 6},
      {0x84,  &NES6502::ZP0,     &NES6502::STY, 3},
      {0x85,  &NES6502::ZP0,     &NES6502::STA, 3},
      {0x86,  &NES6502::ZP0,     &NES6502::STX, 3},
      {0x87,  &NES6502::ZP0, &NES6502::INVALID, 3},
      {0x88,  &NES6502::IMP,     &NES6502::DEY, 2},
      {0x89,  &NES6502::IMM, &NES6502::INVALID, 2},
      {0x8A,  &NES6502::IMP,     &NES6502::TXA, 2},
      {0x8B,  &NES6502::IMM, &NES6502::INVALID, 2},
      {0x8C,  &NES6502::ABS,     &NES6502::STY, 4},
      {0x8D,  &NES6502::ABS,     &NES6502::STA, 4},
      {0x8E,  &NES6502::ABS,     &NES6502::STX, 4},
      {0x8F,  &NES6502::ABS, &NES6502::INVALID, 4},
      {0x90,  &NES6502::REL,     &NES6502::BCC, 2},
      {0x91, &NES6502::INDY,     &NES6502::STA, 6},
      {0x92,  &NES6502::IMP, &NES6502::INVALID, 2},
      {0x93, &NES6502::INDY, &NES6502::INVALID, 6},
      {0x94,  &NES6502::ZPX,     &NES6502::STY, 4},
      {0x95,  &NES6502::ZPX,     &NES6502::STA, 4},
      {0x96,  &NES6502::ZPY,     &NES6502::STX, 4},
      {0x97,  &NES6502::ZPY, &NES6502::INVALID, 4},
      {0x98,  &NES6502::IMP,     &NES6502::TYA, 2},
      {0x99, &NES6502::ABSY,     &NES6502::STA, 5},
      {0x9A,  &NES6502::IMP,     &NES6502::TXS, 2},
      {0x9B, &NES6502::ABSY, &NES6502::INVALID, 5},
      {0x9C, &NES6502::ABSX, &NES6502::INVALID, 5},
      {0x9D, &NES6502::ABSX,     &NES6502::STA, 5},
      {0x9E, &NES6502::ABSY, &NES6502::INVALID, 5},
      {0x9F, &NES6502::ABSY, &NES6502::INVALID, 5},
      {0xA0,  &NES6502::IMM,     &NES6502::LDY, 2},
      {0xA1, &NES6502::INDX,     &NES6502::LDA, 6},
      {0xA2,  &NES6502::IMM,     &NES6502::LDX, 2},
      {0xA3, &NES6502::INDX, &NES6502::INVALID, 6},
      {0xA4,  &NES6502::ZP0,     &NES6502::LDY, 3},
      {0xA5,  &NES6502::ZP0,     &NES6502::LDA, 3},
      {0xA6,  &NES6502::ZP0,     &NES6502::LDX, 3},
      {0xA7,  &NES6502::ZP0, &NES6502::INVALID, 3},
      {0xA8,  &NES6502::IMP,     &NES6502::TAY, 2},
      {0xA9,  &NES6502::IMM,     &NES6502::LDA, 2},
      {0xAA,  &NES6502::IMP,     &NES6502::TAX, 2},
      {0xAB,  &NES6502::IMM, &NES6502::INVALID, 2},
      {0xAC,  &NES6502::ABS,     &NES6502::LDY, 4},
      {0xAD,  &NES6502::ABS,     &NES6502::LDA, 4},
      {0xAE,  &NES6502::ABS,     &NES6502::LDX, 4},
      {0xAF,  &NES6502::ABS, &NES6502::INVALID, 4},
      {0xB0,  &NES6502::REL,     &NES6502::BCC, 2},
      {0xB1, &NES6502::INDY,     &NES6502::LDA, 5},
      {0xB2,  &NES6502::IMP, &NES6502::INVALID, 2},
      {0xB3, &NES6502::INDY, &NES6502::INVALID, 5},
      {0xB4,  &NES6502::ZPX,     &NES6502::LDY, 4},
      {0xB5,  &NES6502::ZPX,     &NES6502::LDA, 4},
      {0xB6,  &NES6502::ZPY,     &NES6502::LDX, 4},
      {0xB7,  &NES6502::ZPY, &NES6502::INVALID, 4},
      {0xB8,  &NES6502::IMP,     &NES6502::CLV, 2},
      {0xB9, &NES6502::ABSY,     &NES6502::LDA, 4},
      {0xBA,  &NES6502::IMP,     &NES6502::TSX, 2},
      {0xBB, &NES6502::ABSY, &NES6502::INVALID, 4},
      {0xBC, &NES6502::ABSX,     &NES6502::LDY, 4},
      {0xBD, &NES6502::ABSX,     &NES6502::LDA, 4},
      {0xBE, &NES6502::ABSY,     &NES6502::LDX, 4},
      {0xBF, &NES6502::ABSY, &NES6502::INVALID, 4},
      {0xC0,  &NES6502::IMM,     &NES6502::CPY, 2},
      {0xC1, &NES6502::INDX,     &NES6502::CMP, 6},
      {0xC2,  &NES6502::IMM, &NES6502::INVALID, 2},
      {0xC3, &NES6502::INDX, &NES6502::INVALID, 8},
      {0xC4,  &NES6502::ZP0,     &NES6502::CPY, 3},
      {0xC5,  &NES6502::ZP0,     &NES6502::CMP, 3},
      {0xC6,  &NES6502::ZP0,     &NES6502::DEC, 5},
      {0xC7,  &NES6502::ZP0, &NES6502::INVALID, 5},
      {0xC8,  &NES6502::IMP,     &NES6502::INY, 2},
      {0xC9,  &NES6502::IMM,     &NES6502::CMP, 2},
      {0xCA,  &NES6502::IMP,     &NES6502::DEX, 2},
      {0xCB,  &NES6502::IMM, &NES6502::INVALID, 2},
      {0xCC,  &NES6502::ABS,     &NES6502::CPY, 4},
      {0xCD,  &NES6502::ABS,     &NES6502::CMP, 4},
      {0xCE,  &NES6502::ABS,     &NES6502::DEC, 6},
      {0xCF,  &NES6502::ABS, &NES6502::INVALID, 6},
      {0xD0,  &NES6502::REL,     &NES6502::BNE, 2},
      {0xD1, &NES6502::INDY,     &NES6502::CMP, 5},
      {0xD2,  &NES6502::IMP, &NES6502::INVALID, 2},
      {0xD3, &NES6502::INDY, &NES6502::INVALID, 8},
      {0xD4,  &NES6502::ZPX, &NES6502::INVALID, 4},
      {0xD5,  &NES6502::ZPX,     &NES6502::CMP, 4},
      {0xD6,  &NES6502::ZPX,     &NES6502::DEC, 6},
      {0xD7,  &NES6502::ZPX, &NES6502::INVALID, 6},
      {0xD8,  &NES6502::IMP,     &NES6502::CLD, 2},
      {0xD9, &NES6502::ABSY,     &NES6502::CMP, 4},
      {0xDA,  &NES6502::IMP, &NES6502::INVALID, 2},
      {0xDB, &NES6502::ABSY, &NES6502::INVALID, 7},
      {0xDC, &NES6502::ABSX, &NES6502::INVALID, 4},
      {0xDD, &NES6502::ABSX,     &NES6502::CMP, 4},
      {0xDE, &NES6502::ABSX,     &NES6502::DEC, 7},
      {0xDF, &NES6502::ABSX, &NES6502::INVALID, 7},
      {0xE0,  &NES6502::IMM,     &NES6502::CPY, 2},
      {0xE1, &NES6502::INDX,     &NES6502::EOR, 6},
      {0xE2,  &NES6502::IMM,     &NES6502::SBC, 2},
      {0xE3, &NES6502::INDX, &NES6502::INVALID, 8},
      {0xE4,  &NES6502::ZP0,     &NES6502::CPX, 3},
      {0xE5,  &NES6502::ZP0,     &NES6502::SBC, 3},
      {0xE6,  &NES6502::ZP0,     &NES6502::INC, 5},
      {0xE7,  &NES6502::ZP0, &NES6502::INVALID, 5},
      {0xE8,  &NES6502::IMP,     &NES6502::INX, 2},
      {0xE9,  &NES6502::IMM,     &NES6502::SBC, 2},
      {0xEA,  &NES6502::IMP,     &NES6502::NOP, 2},
      {0xEB,  &NES6502::IMM, &NES6502::INVALID, 2},
      {0xEC,  &NES6502::ABS,     &NES6502::CPY, 4},
      {0xED,  &NES6502::ABS,     &NES6502::SBC, 4},
      {0xEE,  &NES6502::ABS,     &NES6502::INC, 6},
      {0xEF,  &NES6502::ABS, &NES6502::INVALID, 6},
      {0xF0,  &NES6502::REL,     &NES6502::BEQ, 2},
      {0xF1, &NES6502::INDY,     &NES6502::SBC, 5},
      {0xF2,  &NES6502::IMP, &NES6502::INVALID, 2},
      {0xF3, &NES6502::INDY, &NES6502::INVALID, 8},
      {0xF4,  &NES6502::ZPX, &NES6502::INVALID, 4},
      {0xF5,  &NES6502::ZPX,     &NES6502::SBC, 4},
      {0xF6,  &NES6502::ZPX,     &NES6502::INC, 6},
      {0xF7,  &NES6502::ZPX, &NES6502::INVALID, 6},
      {0xF8,  &NES6502::IMP,     &NES6502::SED, 2},
      {0xF9, &NES6502::ABSY,     &NES6502::SBC, 4},
      {0xFA,  &NES6502::IMP, &NES6502::INVALID, 2},
      {0xFB, &NES6502::ABSY, &NES6502::INVALID, 7},
      {0xFC, &NES6502::ABSX, &NES6502::INVALID, 4},
      {0xFD, &NES6502::ABSX,     &NES6502::SBC, 4},
      {0xFE, &NES6502::ABSX,     &NES6502::INC, 7},
      {0xFF, &NES6502::ABSX, &NES6502::INVALID, 7},
  };
  std::cout << "NES6502 initialized" << std::endl;
}

NES6502::~NES6502() { std::cout << "NES6502 destroyed" << std::endl; }

uint8_t NES6502::read_pc8() {
  uint8_t byte = _bus.read(_pc);
  _pc++;
  return byte;
}

uint16_t NES6502::read_pc16() {
  uint16_t lo = read_pc8();
  uint16_t hi = read_pc8();
  return static_cast<uint16_t>(hi) << 8 | static_cast<uint16_t>(lo);
}

uint8_t  NES6502::read8(uint16_t addr) { return _bus.read(addr); }
uint16_t NES6502::read16(uint16_t addr) {
  uint8_t lo = read8(addr);
  uint8_t hi = read8(addr + 1);
  return static_cast<uint16_t>(hi) << 8 | static_cast<uint16_t>(lo);
}

uint16_t NES6502::read16_zp(uint16_t zp_addr) {
  uint8_t  wrapped_addr = static_cast<uint8_t>(zp_addr + 1);
  uint8_t  zp_lo = read8(zp_addr);
  uint8_t  zp_hi = read8(static_cast<uint16_t>(wrapped_addr));
  uint16_t addr =
      static_cast<uint16_t>(zp_hi) << 8 | static_cast<uint16_t>(zp_lo);
  return read16(addr);
}

void NES6502::write(uint16_t addr, uint8_t data) { _bus.write(addr, data); }

void NES6502::push_stk(uint8_t data) {
  _bus.write(_stp, data);
  _stp--;
}

uint8_t NES6502::pop_stk() {
  _stp++;
  return _bus.read(_stp);
}

/* Addressing Modes */

void NES6502::ABS() { _abs_addr = read_pc16(); }

void NES6502::ABSX() { _abs_addr = read_pc16() + _irx; }

void NES6502::ABSY() { _abs_addr = read_pc16() + _iry; }

void NES6502::ACC() { throw std::runtime_error("ACC not implemented"); }

void NES6502::IMM() { throw std::runtime_error("IMM not implemented"); }

void NES6502::IMP() { throw std::runtime_error("IMP not implemented"); }

void NES6502::IND() {
  uint16_t zp_addr = read_pc16();
  _abs_addr = read16_zp(zp_addr);
}

void NES6502::INDX() {
  uint8_t  operand = read_pc8();
  uint16_t addr = static_cast<uint8_t>(operand + _irx);
  _abs_addr = read16_zp(addr);
}

void NES6502::INDY() {
  uint8_t  operand = read_pc8();
  uint16_t addr = static_cast<uint8_t>(operand + _iry);
  _abs_addr = read16_zp(addr);
}

void NES6502::REL() {
  /// Relative addressing is strange because it is a signed 8-bit offset
  /// masquerading as an unsigned 8-bit offset.
  _rel_addr = static_cast<uint16_t>(read_pc8());
}

void NES6502::ZP0() { _abs_addr = read_pc8(); }
void NES6502::ZPX() { _abs_addr = static_cast<uint8_t>(read_pc8() + _irx); }
void NES6502::ZPY() { _abs_addr = static_cast<uint8_t>(read_pc8() + _iry); }

/* Operations */

// Load/Store Operations

uint8_t NES6502::LDA() {
  _acc = read8(_abs_addr);
  set_zero(_acc == 0);
  set_negative(_acc & 0x80);
  return _acc;
}

uint8_t NES6502::LDX() {
  _irx = read8(_abs_addr);
  set_zero(_irx == 0);
  set_negative(_irx & 0x80);
  return _irx;
}

uint8_t NES6502::LDY() {
  _iry = read8(_abs_addr);
  set_zero(_iry == 0);
  set_negative(_iry & 0x80);
  return _iry;
}

uint8_t NES6502::STA() {
  _bus.write(_abs_addr, _acc);
  return _acc;
}

uint8_t NES6502::STX() {
  _bus.write(_abs_addr, _irx);
  return _irx;
}

uint8_t NES6502::STY() {
  _bus.write(_abs_addr, _iry);
  return _iry;
}

// Register Transfers

uint8_t NES6502::TAX() {
  _irx = _acc;
  set_zero(_irx == 0);
  set_negative(_irx & 0x80);
  return _irx;
}

uint8_t NES6502::TAY() {
  _iry = _acc;
  set_zero(_iry == 0);
  set_negative(_iry & 0x80);
  return _iry;
}

uint8_t NES6502::TXA() {
  _acc = _irx;
  set_zero(_acc == 0);
  set_negative(_acc & 0x80);
  return _acc;
}

uint8_t NES6502::TYA() {
  _acc = _iry;
  set_zero(_acc == 0);
  set_negative(_acc & 0x80);
  return _acc;
}

// Stack Operations

uint8_t NES6502::TSX() {
  _irx = _stp;
  set_zero(_irx == 0);
  set_negative(_irx & 0x80);
  return _irx;
}
uint8_t NES6502::TXS() {
  _stp = _irx;
  return _stp;
}

uint8_t NES6502::PHA() {
  push_stk(_acc);
  return _acc;
}

uint8_t NES6502::PHP() {
  uint8_t pstat = static_cast<uint8_t>(_pstat_r.to_ulong());
  push_stk(pstat);
  return pstat;
}

uint8_t NES6502::PLA() {
  _acc = pop_stk();
  set_zero(_acc == 0);
  set_negative(_acc & 0x80);
  return _acc;
}

uint8_t NES6502::PLP() {
  uint8_t pstat = pop_stk();
  _pstat_r = std::bitset<8>(pstat);
  return pstat;
}

// Bitwise Operations

uint8_t NES6502::AND() {
  _acc = _acc & read8(_abs_addr);
  set_zero(_acc == 0);
  set_negative(_acc & 0x80);
  return _acc;
}
uint8_t NES6502::EOR() {
  _acc = _acc ^ read8(_abs_addr);
  set_zero(_acc == 0);
  set_negative(_acc & 0x80);
  return _acc;
}
uint8_t NES6502::ORA() {
  _acc = _acc | read8(_abs_addr);
  set_zero(_acc == 0);
  set_negative(_acc & 0x80);
  return _acc;
}
uint8_t NES6502::BIT() {
  uint8_t value = _acc & read8(_abs_addr);
  set_zero(value == 0);
  set_overflow(value & 0x40);
  set_negative(value & 0x80);
  return value;
}

// Arithmetic Operations

uint8_t NES6502::ADC() {
  throw std::runtime_error("ADC not implemented");
  uint8_t data = read8(_abs_addr);
  uint8_t result = _acc + data + get_carry();
  set_carry(result < _acc || result < data);
  set_overflow(result < _acc || result < data);
  set_zero(result == 0);
  set_negative(result & 0x80);
  _acc = result & 0xFF;
  return _acc;
}
uint8_t NES6502::SBC() {
  throw std::runtime_error("SBC not implemented");
  return 0;
}
uint8_t NES6502::CMP() {
  throw std::runtime_error("CMP not implemented");
  return 0;
}
uint8_t NES6502::CPX() {
  throw std::runtime_error("CPX not implemented");
  return 0;
}
uint8_t NES6502::CPY() {
  throw std::runtime_error("CPY not implemented");
  return 0;
}

// Increment/Decrement Operations

uint8_t NES6502::INC() {
  throw std::runtime_error("INC not implemented");
  return 0;
}
uint8_t NES6502::INX() {
  throw std::runtime_error("INX not implemented");
  return 0;
}
uint8_t NES6502::INY() {
  throw std::runtime_error("INY not implemented");
  return 0;
}
uint8_t NES6502::DEC() {
  throw std::runtime_error("DEC not implemented");
  return 0;
}
uint8_t NES6502::DEX() {
  throw std::runtime_error("DEX not implemented");
  return 0;
}
uint8_t NES6502::DEY() {
  throw std::runtime_error("DEY not implemented");
  return 0;
}

// Shift Operations

uint8_t NES6502::ASL() {
  throw std::runtime_error("ASL not implemented");
  return 0;
}
uint8_t NES6502::LSR() {
  throw std::runtime_error("LSR not implemented");
  return 0;
}
uint8_t NES6502::ROL() {
  throw std::runtime_error("ROL not implemented");
  return 0;
}
uint8_t NES6502::ROR() {
  throw std::runtime_error("ROR not implemented");
  return 0;
}

// Jump Operations

uint8_t NES6502::JMP() {
  throw std::runtime_error("JMP not implemented");
  return 0;
}
uint8_t NES6502::JSR() {
  throw std::runtime_error("JSR not implemented");
  return 0;
}
uint8_t NES6502::RTS() {
  throw std::runtime_error("RTS not implemented");
  return 0;
}

// Branching

uint8_t NES6502::BCC() {
  throw std::runtime_error("BCC not implemented");
  return 0;
}
uint8_t NES6502::BCS() {
  throw std::runtime_error("BCS not implemented");
  return 0;
}
uint8_t NES6502::BEQ() {
  throw std::runtime_error("BEQ not implemented");
  return 0;
}
uint8_t NES6502::BMI() {
  throw std::runtime_error("BMI not implemented");
  return 0;
}
uint8_t NES6502::BNE() {
  throw std::runtime_error("BNE not implemented");
  return 0;
}
uint8_t NES6502::BPL() {
  throw std::runtime_error("BPL not implemented");
  return 0;
}
uint8_t NES6502::BVC() {
  throw std::runtime_error("BVC not implemented");
  return 0;
}
uint8_t NES6502::BVS() {
  throw std::runtime_error("BVS not implemented");
  return 0;
}

// Status Flag Changes

uint8_t NES6502::CLC() {
  throw std::runtime_error("CLC not implemented");
  return 0;
}
uint8_t NES6502::CLD() {
  throw std::runtime_error("CLD not implemented");
  return 0;
}
uint8_t NES6502::CLI() {
  throw std::runtime_error("CLI not implemented");
  return 0;
}
uint8_t NES6502::CLV() {
  throw std::runtime_error("CLV not implemented");
  return 0;
}
uint8_t NES6502::SEC() {
  throw std::runtime_error("SEC not implemented");
  return 0;
}
uint8_t NES6502::SED() {
  throw std::runtime_error("SED not implemented");
  return 0;
}
uint8_t NES6502::SEI() {
  throw std::runtime_error("SEI not implemented");
  return 0;
}

// System Functions

uint8_t NES6502::BRK() {
  throw std::runtime_error("BRK not implemented");
  return 0;
}
uint8_t NES6502::NOP() {
  throw std::runtime_error("NOP not implemented");
  return 0;
}
uint8_t NES6502::RTI() {
  throw std::runtime_error("RTI not implemented");
  return 0;
}

// Invalid Operations

uint8_t NES6502::INVALID() {
  throw std::runtime_error("INVALID not implemented");
  return 0;
}

void    NES6502::set_carry(bool flag) { _pstat_r.set(0, flag); }
void    NES6502::set_zero(bool flag) { _pstat_r.set(1, flag); }
void    NES6502::set_interrupt_disable(bool flag) { _pstat_r.set(2, flag); }
void    NES6502::set_decimal_mode(bool flag) { _pstat_r.set(3, flag); }
void    NES6502::set_break(bool flag) { _pstat_r.set(4, flag); }
void    NES6502::set_unused(bool flag) { _pstat_r.set(5, flag); }
void    NES6502::set_overflow(bool flag) { _pstat_r.set(6, flag); }
void    NES6502::set_negative(bool flag) { _pstat_r.set(7, flag); }

uint8_t NES6502::get_carry() { return _pstat_r.test(0) ? 1 : 0; }
uint8_t NES6502::get_zero() { return _pstat_r.test(1) ? (1 << 1) : 0; }
uint8_t NES6502::get_interrupt_disable() {
  return _pstat_r.test(2) ? (1 << 2) : 0;
}
uint8_t NES6502::get_decimal_mode() { return _pstat_r.test(3) ? (1 << 3) : 0; }
uint8_t NES6502::get_break() { return _pstat_r.test(4) ? (1 << 4) : 0; }
uint8_t NES6502::get_unused() { return _pstat_r.test(5) ? (1 << 5) : 0; }
uint8_t NES6502::get_overflow() { return _pstat_r.test(6) ? (1 << 6) : 0; }
uint8_t NES6502::get_negative() { return _pstat_r.test(7) ? (1 << 7) : 0; }