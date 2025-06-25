#pragma once

namespace Instruction {

/* 
 * 6502 Instruction Set Reference
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
} // namespace Instruction