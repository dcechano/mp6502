#pragma once

#include "./addr_mode.hpp"
#include <cstdint>
#include <vector>

namespace Operation {

struct Instruction {
  uint8_t opcode;
  uint8_t (*addr_mode)(void);
  uint8_t (*op_exec)(void);
  uint8_t cycles;
};

uint8_t noop() { return 0; }
class OpList {
public:
  OpList();
  ~OpList();
  Instruction get(size_t);

private:
  std::vector<Instruction> op_list;
};

/* 
 * 6502 Operation Set Reference
 * http://www.6502.org/users/obelisk/6502/instructions.html
 *
*/
enum OpCode {
  /* Load/Store Operations */
  LDA, // Load Accumulator
  LDX, // Load X Register
  LDY, // Load Y Register
  STA, // Store Accumulator
  STX, // Store X Register
  STY, // Store Y Register

  /* Register Transfers */
  TAX, // Transfer Accumulator to X
  TAY, // Transfer Accumulator to Y
  TXA, // Transfer X to Accumulator
  TYA, // Transfer Y to Accumulator

  /* Stack Operations */
  TSX, // Transfer Stack Pointer to X
  TXS, // Transfer X to Stack Pointer
  PHA, // Push Accumulator
  PLA, // Pop Accumulator
  PHP, // Push Processor Status
  PLP, // Pop Processor Status

  /* Bitwise Operations */
  AND, // AND
  EOR, // Exclusive OR
  ORA, // OR
  BIT, // Test Bits

  /* Arithmetic Operations */
  ADC, // Add with Carry
  SBC, // Subtract with Carry
  INC, // Increment Memory
  DEC, // Decrement Memory

  /* Increment/Decrement Operations */
  INX, // Increment X
  INY, // Increment Y
  DEX, // Decrement X
  DEY, // Decrement Y

  /* Shift Operations */
  ASL, // Arithmetic Shift Left
  LSR, // Logical Shift Right
  ROL, // Rotate Left
  ROR, // Rotate Right

  /* Jump Operations */
  JMP, // Jump
  JSR, // Jump to Subroutine
  RTS, // Return from Subroutine

  /* Branching */
  BCC, // Branch on Carry Clear
  BCS, // Branch on Carry Set
  BEQ, // Branch on Equal
  BMI, // Branch on Minus
  BNE, // Branch on Not Equal
  BPL, // Branch on Plus
  BVC, // Branch on Overflow Clear
  BVS, // Branch on Overflow Set

  /* Status Flag Changes */
  CLC, // Clear Carry Flag
  CLD, // Clear Decimal Flag
  CLI, // Clear Interrupt Disable
  CLV, // Clear Overflow Flag
  SEC, // Set Carry Flag
  SED, // Set Decimal Flag
  SEI, // Set Interrupt Disable

  /* System Functions */
  BRK, // Force Interrupt
  NOP, // No Operation
  RTI, // Return from Interrupt

  /* Invalid Operations */
  INVALID, // Invalid Operation

};
} // namespace Operation