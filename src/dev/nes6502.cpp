#include "./nes6502.hpp"
#include <iostream>
#include <vector>

NES6502::NES6502() {
  _pc = 0x0000;
  _stp = 0xFD;
  _acc = 0;
  _irx = 0;
  _iry = 0;
  _proc_stat = 0;
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

/* Addressing Modes */

uint8_t NES6502::ABS() { return 0; }
uint8_t NES6502::ABSX() { return 0; }
uint8_t NES6502::ABSY() { return 0; }
uint8_t NES6502::ACC() { return 0; }
uint8_t NES6502::IMM() { return 0; }
uint8_t NES6502::IMP() { return 0; }
uint8_t NES6502::IND() { return 0; }
uint8_t NES6502::INDX() { return 0; }
uint8_t NES6502::INDY() { return 0; }
uint8_t NES6502::REL() { return 0; }
uint8_t NES6502::ZP0() { return 0; }
uint8_t NES6502::ZPX() { return 0; }
uint8_t NES6502::ZPY() { return 0; }

// Load/Store Operations
uint8_t NES6502::LDA() { return 0; }
uint8_t NES6502::LDX() { return 0; }
uint8_t NES6502::LDY() { return 0; }
uint8_t NES6502::STA() { return 0; }
uint8_t NES6502::STX() { return 0; }
uint8_t NES6502::STY() { return 0; }

// Register Transfers
uint8_t NES6502::TAX() { return 0; }
uint8_t NES6502::TAY() { return 0; }
uint8_t NES6502::TXA() { return 0; }
uint8_t NES6502::TYA() { return 0; }

// Stack Operations
uint8_t NES6502::TSX() { return 0; }
uint8_t NES6502::TXS() { return 0; }
uint8_t NES6502::PHA() { return 0; }
uint8_t NES6502::PLA() { return 0; }
uint8_t NES6502::PHP() { return 0; }
uint8_t NES6502::PLP() { return 0; }

// Bitwise Operations
uint8_t NES6502::AND() { return 0; }
uint8_t NES6502::EOR() { return 0; }
uint8_t NES6502::ORA() { return 0; }
uint8_t NES6502::BIT() { return 0; }

// Arithmetic Operations
uint8_t NES6502::ADC() { return 0; }
uint8_t NES6502::SBC() { return 0; }
uint8_t NES6502::CMP() { return 0; }
uint8_t NES6502::CPX() { return 0; }
uint8_t NES6502::CPY() { return 0; }

// Increment/Decrement Operations
uint8_t NES6502::INC() { return 0; }
uint8_t NES6502::INX() { return 0; }
uint8_t NES6502::INY() { return 0; }
uint8_t NES6502::DEC() { return 0; }
uint8_t NES6502::DEX() { return 0; }
uint8_t NES6502::DEY() { return 0; }

// Shift Operations
uint8_t NES6502::ASL() { return 0; }
uint8_t NES6502::LSR() { return 0; }
uint8_t NES6502::ROL() { return 0; }
uint8_t NES6502::ROR() { return 0; }

// Jump Operations
uint8_t NES6502::JMP() { return 0; }
uint8_t NES6502::JSR() { return 0; }
uint8_t NES6502::RTS() { return 0; }

// Branching
uint8_t NES6502::BCC() { return 0; }
uint8_t NES6502::BCS() { return 0; }
uint8_t NES6502::BEQ() { return 0; }
uint8_t NES6502::BMI() { return 0; }
uint8_t NES6502::BNE() { return 0; }
uint8_t NES6502::BPL() { return 0; }
uint8_t NES6502::BVC() { return 0; }
uint8_t NES6502::BVS() { return 0; }

// Status Flag Changes
uint8_t NES6502::CLC() { return 0; }
uint8_t NES6502::CLD() { return 0; }
uint8_t NES6502::CLI() { return 0; }
uint8_t NES6502::CLV() { return 0; }
uint8_t NES6502::SEC() { return 0; }
uint8_t NES6502::SED() { return 0; }
uint8_t NES6502::SEI() { return 0; }

// System Functions
uint8_t NES6502::BRK() { return 0; }
uint8_t NES6502::NOP() { return 0; }
uint8_t NES6502::RTI() { return 0; }

// Invalid Operations
uint8_t NES6502::INVALID() { return 0; }