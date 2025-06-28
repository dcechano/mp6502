#include "./operation.hpp"
#include "./addr_mode.hpp"
#include <vector>

namespace AdrM = AddrMode;
namespace Op = Operation;

Op::OpList::OpList() {
  op_list = {
      {0x00,  AdrM::IMM,     Op::BRK, 7},
      {0x01, AdrM::INDX,     Op::ORA, 6},
      {0x02,  AdrM::IMP, Op::INVALID, 2},
      {0x03, AdrM::INDX, Op::INVALID, 8},
      {0x04,  AdrM::ZP0, Op::INVALID, 3},
      {0x05,  AdrM::ZP0,     Op::ORA, 3},
      {0x06,  AdrM::ZP0,     Op::ASL, 5},
      {0x07,  AdrM::ZP0, Op::INVALID, 5},
      {0x08,  AdrM::IMP,     Op::PHP, 3},
      {0x09,  AdrM::IMM,     Op::ORA, 2},
      {0x0A,  AdrM::ACC,     Op::ASL, 2},
      {0x0B,  AdrM::IMM, Op::INVALID, 2},
      {0x0C,  AdrM::ABS, Op::INVALID, 4},
      {0x0D,  AdrM::ABS,     Op::ORA, 4},
      {0x0E,  AdrM::ABS,     Op::ASL, 6},
      {0x0F,  AdrM::ABS, Op::INVALID, 6},
      {0x10,  AdrM::REL,     Op::BPL, 2},
      {0x11, AdrM::INDY,     Op::ORA, 5},
      {0x12,  AdrM::IMP, Op::INVALID, 2},
      {0x13, AdrM::INDY, Op::INVALID, 8},
      {0x14,  AdrM::ZPX, Op::INVALID, 4},
      {0x15,  AdrM::ZPX,     Op::ORA, 4},
      {0x16,  AdrM::ZPX,     Op::ASL, 6},
      {0x17,  AdrM::ZPX, Op::INVALID, 6},
      {0x18,  AdrM::IMP,     Op::CLC, 2},
      {0x19, AdrM::ABSY,     Op::ORA, 4},
      {0x1A,  AdrM::IMP, Op::INVALID, 2},
      {0x1B, AdrM::ABSY, Op::INVALID, 7},
      {0x1C, AdrM::ABSX, Op::INVALID, 4},
      {0x1D, AdrM::ABSX,     Op::ORA, 4},
      {0x1E, AdrM::ABSX,     Op::ASL, 7},
      {0x1F, AdrM::ABSX, Op::INVALID, 7},
      {0x20,  AdrM::ABS,     Op::JSR, 6},
      {0x21, AdrM::INDX,     Op::AND, 6},
      {0x22,  AdrM::IMP, Op::INVALID, 2},
      {0x23, AdrM::INDX, Op::INVALID, 8},
      {0x24,  AdrM::ZP0,     Op::BIT, 3},
      {0x25,  AdrM::ZP0,     Op::AND, 3},
      {0x26,  AdrM::ZP0,     Op::ROL, 5},
      {0x27,  AdrM::ZP0, Op::INVALID, 5},
      {0x28,  AdrM::IMP,     Op::PLP, 4},
      {0x29,  AdrM::IMM,     Op::AND, 2},
      {0x2A,  AdrM::ACC,     Op::ROL, 2},
      {0x2B,  AdrM::IMM, Op::INVALID, 2},
      {0x2C,  AdrM::ABS,     Op::BIT, 4},
      {0x2D,  AdrM::ABS,     Op::AND, 4},
      {0x2E,  AdrM::ABS,     Op::ROL, 6},
      {0x2F,  AdrM::ABS, Op::INVALID, 6},
      {0x30,  AdrM::REL,     Op::BMI, 2},
      {0x31, AdrM::INDY,     Op::AND, 5},
      {0x32,  AdrM::IMP, Op::INVALID, 2},
      {0x33, AdrM::INDY, Op::INVALID, 8},
      {0x34,  AdrM::ZPX, Op::INVALID, 4},
      {0x35,  AdrM::ZPX,     Op::AND, 4},
      {0x36,  AdrM::ZPX,     Op::ROL, 6},
      {0x37,  AdrM::ZPX, Op::INVALID, 6},
      {0x38,  AdrM::IMP,     Op::SEC, 2},
      {0x39, AdrM::ABSY,     Op::AND, 4},
      {0x3A,  AdrM::IMP, Op::INVALID, 2},
      {0x3B, AdrM::ABSY, Op::INVALID, 7},
      {0x3C, AdrM::ABSX, Op::INVALID, 4},
      {0x3D, AdrM::ABSX,     Op::AND, 4},
      {0x3E, AdrM::ABSX,     Op::ROL, 7},
      {0x3F, AdrM::ABSX, Op::INVALID, 7},
      {0x40,  AdrM::IMP,     Op::RTI, 6},
      {0x41, AdrM::INDX,     Op::EOR, 6},
      {0x42,  AdrM::IMP, Op::INVALID, 2},
      {0x43, AdrM::INDX, Op::INVALID, 8},
      {0x44,  AdrM::ZP0, Op::INVALID, 3},
      {0x45,  AdrM::ZP0,     Op::EOR, 3},
      {0x46,  AdrM::ZP0,     Op::LSR, 5},
      {0x47,  AdrM::ZP0, Op::INVALID, 5},
      {0x48,  AdrM::IMP,     Op::PHA, 3},
      {0x49,  AdrM::IMM,     Op::EOR, 2},
      {0x4A,  AdrM::ACC,     Op::LSR, 2},
      {0x4B,  AdrM::IMM, Op::INVALID, 2},
      {0x4C,  AdrM::ABS,     Op::JMP, 3},
      {0x4D,  AdrM::ABS,     Op::EOR, 4},
      {0x4E,  AdrM::ABS,     Op::LSR, 6},
      {0x4F,  AdrM::ABS, Op::INVALID, 6},
      {0x50,  AdrM::REL,     Op::RTS, 2},
      {0x51, AdrM::INDY,     Op::EOR, 5},
      {0x52,  AdrM::IMP, Op::INVALID, 2},
      {0x53, AdrM::INDY, Op::INVALID, 8},
      {0x54,  AdrM::ZPX, Op::INVALID, 4},
      {0x55,  AdrM::ZPX,     Op::EOR, 4},
      {0x56,  AdrM::ZPX,     Op::LSR, 6},
      {0x57,  AdrM::ZPX, Op::INVALID, 6},
      {0x58,  AdrM::IMP,     Op::CLI, 2},
      {0x59, AdrM::ABSY,     Op::EOR, 4},
      {0x5A,  AdrM::IMP, Op::INVALID, 2},
      {0x5B, AdrM::ABSY, Op::INVALID, 7},
      {0x5C, AdrM::ABSX, Op::INVALID, 4},
      {0x5D, AdrM::ABSX,     Op::EOR, 4},
      {0x5E, AdrM::ABSX,     Op::LSR, 7},
      {0x5F, AdrM::ABSX, Op::INVALID, 7},
      {0x60,  AdrM::IMP,     Op::RTS, 6},
      {0x61, AdrM::INDX,     Op::ADC, 6},
      {0x62,  AdrM::IMP, Op::INVALID, 2},
      {0x63, AdrM::INDX, Op::INVALID, 8},
      {0x64,  AdrM::ZP0, Op::INVALID, 3},
      {0x65,  AdrM::ZP0,     Op::ADC, 3},
      {0x66,  AdrM::ZP0,     Op::ROL, 5},
      {0x67,  AdrM::ZP0, Op::INVALID, 5},
      {0x68,  AdrM::IMP,     Op::PLA, 4},
      {0x69,  AdrM::IMM,     Op::ADC, 2},
      {0x6A,  AdrM::ACC,     Op::ROR, 2},
      {0x6B,  AdrM::IMM, Op::INVALID, 2},
      {0x6C,  AdrM::IND,     Op::JMP, 5},
      {0x6D,  AdrM::ABS,     Op::ADC, 4},
      {0x6E,  AdrM::ABS,     Op::ROL, 6},
      {0x6F,  AdrM::ABS, Op::INVALID, 6},
      {0x70,  AdrM::REL,     Op::BVS, 2},
      {0x71, AdrM::INDY,     Op::ADC, 5},
      {0x72,  AdrM::IMP, Op::INVALID, 2},
      {0x73, AdrM::INDY, Op::INVALID, 8},
      {0x74,  AdrM::ZPX, Op::INVALID, 4},
      {0x75,  AdrM::ZPX,     Op::ADC, 4},
      {0x76,  AdrM::ZPX,     Op::ROL, 6},
      {0x77,  AdrM::ZPX, Op::INVALID, 6},
      {0x78,  AdrM::IMP,     Op::SEI, 2},
      {0x79, AdrM::ABSY,     Op::ADC, 4},
      {0x7A,  AdrM::IMP, Op::INVALID, 2},
      {0x7B, AdrM::ABSY, Op::INVALID, 7},
      {0x7C, AdrM::ABSX, Op::INVALID, 4},
      {0x7D, AdrM::ABSX,     Op::ADC, 4},
      {0x7E, AdrM::ABSX,     Op::ROL, 7},
      {0x7F, AdrM::ABSX, Op::INVALID, 7},
      {0x80,  AdrM::IMM, Op::INVALID, 2},
      {0x81, AdrM::INDX,     Op::STA, 6},
      {0x82,  AdrM::IMM, Op::INVALID, 2},
      {0x83, AdrM::INDX, Op::INVALID, 6},
      {0x84,  AdrM::ZP0,     Op::STY, 3},
      {0x85,  AdrM::ZP0,     Op::STA, 3},
      {0x86,  AdrM::ZP0,     Op::STX, 3},
      {0x87,  AdrM::ZP0, Op::INVALID, 3},
      {0x88,  AdrM::IMP,     Op::DEY, 2},
      {0x89,  AdrM::IMM, Op::INVALID, 2},
      {0x8A,  AdrM::IMP,     Op::TXA, 2},
      {0x8B,  AdrM::IMM, Op::INVALID, 2},
      {0x8C,  AdrM::ABS,     Op::STY, 4},
      {0x8D,  AdrM::ABS,     Op::STA, 4},
      {0x8E,  AdrM::ABS,     Op::STX, 4},
      {0x8F,  AdrM::ABS, Op::INVALID, 4},
      {0x90,  AdrM::REL,     Op::BCC, 2},
      {0x91, AdrM::INDY,     Op::STA, 6},
      {0x92,  AdrM::IMP, Op::INVALID, 2},
      {0x93, AdrM::INDY, Op::INVALID, 6},
      {0x94,  AdrM::ZPX,     Op::STY, 4},
      {0x95,  AdrM::ZPX,     Op::STA, 4},
      {0x96,  AdrM::ZPY,     Op::STX, 4},
      {0x97,  AdrM::ZPY, Op::INVALID, 4},
      {0x98,  AdrM::IMP,     Op::TYA, 2},
      {0x99, AdrM::ABSY,     Op::STA, 5},
      {0x9A,  AdrM::IMP,     Op::TXS, 2},
      {0x9B, AdrM::ABSY, Op::INVALID, 5},
      {0x9C, AdrM::ABSX, Op::INVALID, 5},
      {0x9D, AdrM::ABSX,     Op::STA, 5},
      {0x9E, AdrM::ABSY, Op::INVALID, 5},
      {0x9F, AdrM::ABSY, Op::INVALID, 5},
      {0xA0,  AdrM::IMM,     Op::LDY, 2},
      {0xA1, AdrM::INDX,     Op::LDA, 6},
      {0xA2,  AdrM::IMM,     Op::LDX, 2},
      {0xA3, AdrM::INDX, Op::INVALID, 6},
      {0xA4,  AdrM::ZP0,     Op::LDY, 3},
      {0xA5,  AdrM::ZP0,     Op::LDA, 3},
      {0xA6,  AdrM::ZP0,     Op::LDX, 3},
      {0xA7,  AdrM::ZP0, Op::INVALID, 3},
      {0xA8,  AdrM::IMP,     Op::TAY, 2},
      {0xA9,  AdrM::IMM,     Op::LDA, 2},
      {0xAA,  AdrM::IMP,     Op::TAX, 2},
      {0xAB,  AdrM::IMM, Op::INVALID, 2},
      {0xAC,  AdrM::ABS,     Op::LDY, 4},
      {0xAD,  AdrM::ABS,     Op::LDA, 4},
      {0xAE,  AdrM::ABS,     Op::LDX, 4},
      {0xAF,  AdrM::ABS, Op::INVALID, 4},
      {0xB0,  AdrM::REL,     Op::BCC, 2},
      {0xB1, AdrM::INDY,     Op::LDA, 5},
      {0xB2,  AdrM::IMP, Op::INVALID, 2},
      {0xB3, AdrM::INDY, Op::INVALID, 5},
      {0xB4,  AdrM::ZPX,     Op::LDY, 4},
      {0xB5,  AdrM::ZPX,     Op::LDA, 4},
      {0xB6,  AdrM::ZPY,     Op::LDX, 4},
      {0xB7,  AdrM::ZPY, Op::INVALID, 4},
      {0xB8,  AdrM::IMP,     Op::CLV, 2},
      {0xB9, AdrM::ABSY,     Op::LDA, 4},
      {0xBA,  AdrM::IMP,     Op::TSX, 2},
      {0xBB, AdrM::ABSY, Op::INVALID, 4},
      {0xBC, AdrM::ABSX,     Op::LDY, 4},
      {0xBD, AdrM::ABSX,     Op::LDA, 4},
      {0xBE, AdrM::ABSY,     Op::LDX, 4},
      {0xBF, AdrM::ABSY, Op::INVALID, 4},
      {0xC0,  AdrM::IMM,     Op::CPY, 2},
      {0xC1, AdrM::INDX,     Op::CMP, 6},
      {0xC2,  AdrM::IMM, Op::INVALID, 2},
      {0xC3, AdrM::INDX, Op::INVALID, 8},
      {0xC4,  AdrM::ZP0,     Op::CPY, 3},
      {0xC5,  AdrM::ZP0,     Op::CMP, 3},
      {0xC6,  AdrM::ZP0,     Op::DEC, 5},
      {0xC7,  AdrM::ZP0, Op::INVALID, 5},
      {0xC8,  AdrM::IMP,     Op::INY, 2},
      {0xC9,  AdrM::IMM,     Op::CMP, 2},
      {0xCA,  AdrM::IMP,     Op::DEX, 2},
      {0xCB,  AdrM::IMM, Op::INVALID, 2},
      {0xCC,  AdrM::ABS,     Op::CPY, 4},
      {0xCD,  AdrM::ABS,     Op::CMP, 4},
      {0xCE,  AdrM::ABS,     Op::DEC, 6},
      {0xCF,  AdrM::ABS, Op::INVALID, 6},
      {0xD0,  AdrM::REL,     Op::BNE, 2},
      {0xD1, AdrM::INDY,     Op::CMP, 5},
      {0xD2,  AdrM::IMP, Op::INVALID, 2},
      {0xD3, AdrM::INDY, Op::INVALID, 8},
      {0xD4,  AdrM::ZPX, Op::INVALID, 4},
      {0xD5,  AdrM::ZPX,     Op::CMP, 4},
      {0xD6,  AdrM::ZPX,     Op::DEC, 6},
      {0xD7,  AdrM::ZPX, Op::INVALID, 6},
      {0xD8,  AdrM::IMP,     Op::CLD, 2},
      {0xD9, AdrM::ABSY,     Op::CMP, 4},
      {0xDA,  AdrM::IMP, Op::INVALID, 2},
      {0xDB, AdrM::ABSY, Op::INVALID, 7},
      {0xDC, AdrM::ABSX, Op::INVALID, 4},
      {0xDD, AdrM::ABSX,     Op::CMP, 4},
      {0xDE, AdrM::ABSX,     Op::DEC, 7},
      {0xDF, AdrM::ABSX, Op::INVALID, 7},
      {0xE0,  AdrM::IMM,     Op::CPY, 2},
      {0xE1, AdrM::INDX,     Op::EOR, 6},
      {0xE2,  AdrM::IMM,     Op::SBC, 2},
      {0xE3, AdrM::INDX, Op::INVALID, 8},
      {0xE4,  AdrM::ZP0,     Op::CPX, 3},
      {0xE5,  AdrM::ZP0,     Op::SBC, 3},
      {0xE6,  AdrM::ZP0,     Op::INC, 5},
      {0xE7,  AdrM::ZP0, Op::INVALID, 5},
      {0xE8,  AdrM::IMP,     Op::INX, 2},
      {0xE9,  AdrM::IMM,     Op::SBC, 2},
      {0xEA,  AdrM::IMP,     Op::NOP, 2},
      {0xEB,  AdrM::IMM, Op::INVALID, 2},
      {0xEC,  AdrM::ABS,     Op::CPY, 4},
      {0xED,  AdrM::ABS,     Op::SBC, 4},
      {0xEE,  AdrM::ABS,     Op::INC, 6},
      {0xEF,  AdrM::ABS, Op::INVALID, 6},
      {0xF0,  AdrM::REL,     Op::BEQ, 2},
      {0xF1, AdrM::INDY,     Op::SBC, 5},
      {0xF2,  AdrM::IMP, Op::INVALID, 2},
      {0xF3, AdrM::INDY, Op::INVALID, 8},
      {0xF4,  AdrM::ZPX, Op::INVALID, 4},
      {0xF5,  AdrM::ZPX,     Op::SBC, 4},
      {0xF6,  AdrM::ZPX,     Op::INC, 6},
      {0xF7,  AdrM::ZPX, Op::INVALID, 6},
      {0xF8,  AdrM::IMP,     Op::SED, 2},
      {0xF9, AdrM::ABSY,     Op::SBC, 4},
      {0xFA,  AdrM::IMP, Op::INVALID, 2},
      {0xFB, AdrM::ABSY, Op::INVALID, 7},
      {0xFC, AdrM::ABSX, Op::INVALID, 4},
      {0xFD, AdrM::ABSX,     Op::SBC, 4},
      {0xFE, AdrM::ABSX,     Op::INC, 7},
      {0xFF, AdrM::ABSX, Op::INVALID, 7},
  };
}

// Load/Store Operations
uint8_t Op::LDA() { return 0; }
uint8_t Op::LDX() { return 0; }
uint8_t Op::LDY() { return 0; }
uint8_t Op::STA() { return 0; }
uint8_t Op::STX() { return 0; }
uint8_t Op::STY() { return 0; }

// Register Transfers
uint8_t Op::TAX() { return 0; }
uint8_t Op::TAY() { return 0; }
uint8_t Op::TXA() { return 0; }
uint8_t Op::TYA() { return 0; }

// Stack Operations
uint8_t Op::TSX() { return 0; }
uint8_t Op::TXS() { return 0; }
uint8_t Op::PHA() { return 0; }
uint8_t Op::PLA() { return 0; }
uint8_t Op::PHP() { return 0; }
uint8_t Op::PLP() { return 0; }

// Bitwise Operations
uint8_t Op::AND() { return 0; }
uint8_t Op::EOR() { return 0; }
uint8_t Op::ORA() { return 0; }
uint8_t Op::BIT() { return 0; }

// Arithmetic Operations
uint8_t Op::ADC() { return 0; }
uint8_t Op::SBC() { return 0; }
uint8_t Op::CMP() { return 0; }
uint8_t Op::CPX() { return 0; }
uint8_t Op::CPY() { return 0; }

// Increment/Decrement Operations
uint8_t Op::INC() { return 0; }
uint8_t Op::INX() { return 0; }
uint8_t Op::INY() { return 0; }
uint8_t Op::DEC() { return 0; }
uint8_t Op::DEX() { return 0; }
uint8_t Op::DEY() { return 0; }

// Shift Operations
uint8_t Op::ASL() { return 0; }
uint8_t Op::LSR() { return 0; }
uint8_t Op::ROL() { return 0; }
uint8_t Op::ROR() { return 0; }

// Jump Operations
uint8_t Op::JMP() { return 0; }
uint8_t Op::JSR() { return 0; }
uint8_t Op::RTS() { return 0; }

// Branching
uint8_t Op::BCC() { return 0; }
uint8_t Op::BCS() { return 0; }
uint8_t Op::BEQ() { return 0; }
uint8_t Op::BMI() { return 0; }
uint8_t Op::BNE() { return 0; }
uint8_t Op::BPL() { return 0; }
uint8_t Op::BVC() { return 0; }
uint8_t Op::BVS() { return 0; }

// Status Flag Changes
uint8_t Op::CLC() { return 0; }
uint8_t Op::CLD() { return 0; }
uint8_t Op::CLI() { return 0; }
uint8_t Op::CLV() { return 0; }
uint8_t Op::SEC() { return 0; }
uint8_t Op::SED() { return 0; }
uint8_t Op::SEI() { return 0; }

// System Functions
uint8_t Op::BRK() { return 0; }
uint8_t Op::NOP() { return 0; }
uint8_t Op::RTI() { return 0; }

// Invalid Operations
uint8_t Op::INVALID() { return 0; }