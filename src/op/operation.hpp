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
/* Load/Store Operations */
uint8_t LDA(); // Load Accumulator
uint8_t LDX(); // Load X Register
uint8_t LDY(); // Load Y Register
uint8_t STA(); // Store Accumulator
uint8_t STX(); // Store X Register
uint8_t STY(); // Store Y Register

/* Register Transfers */
uint8_t TAX(); // Transfer Accumulator to X
uint8_t TAY(); // Transfer Accumulator to Y
uint8_t TXA(); // Transfer X to Accumulator
uint8_t TYA(); // Transfer Y to Accumulator

/* Stack Operations */
uint8_t TSX(); // Transfer Stack Pointer to X
uint8_t TXS(); // Transfer X to Stack Pointer
uint8_t PHA(); // Push Accumulator
uint8_t PLA(); // Pop Accumulator
uint8_t PHP(); // Push Processor Status
uint8_t PLP(); // Pop Processor Status

/* Bitwise Operations */
uint8_t AND(); // AND
uint8_t EOR(); // Exclusive OR
uint8_t ORA(); // OR
uint8_t BIT(); // Test Bits

/* Arithmetic Operations */
uint8_t ADC(); // Add with Carry
uint8_t SBC(); // Subtract with Carry
uint8_t CMP(); // Compare Accumulator:w
uint8_t CPX(); // Compare X
uint8_t CPY(); // Compare Y

/* Increment/Decrement Operations */
uint8_t INX(); // Increment X
uint8_t INY(); // Increment Y
uint8_t INC(); // Increment Memory
uint8_t DEC(); // Decrement Memory
uint8_t DEX(); // Decrement X
uint8_t DEY(); // Decrement Y

/* Shift Operations */
uint8_t ASL(); // Arithmetic Shift Left
uint8_t LSR(); // Logical Shift Right
uint8_t ROL(); // Rotate Left
uint8_t ROR(); // Rotate Right

/* Jump Operations */
uint8_t JMP(); // Jump
uint8_t JSR(); // Jump to Subroutine
uint8_t RTS(); // Return from Subroutine

/* Branching */
uint8_t BCC(); // Branch on Carry Clear
uint8_t BCS(); // Branch on Carry Set
uint8_t BEQ(); // Branch on Equal
uint8_t BMI(); // Branch on Minus
uint8_t BNE(); // Branch on Not Equal
uint8_t BPL(); // Branch on Plus
uint8_t BVC(); // Branch on Overflow Clear
uint8_t BVS(); // Branch on Overflow Set

/* Status Flag Changes */
uint8_t CLC(); // Clear Carry Flag
uint8_t CLD(); // Clear Decimal Flag
uint8_t CLI(); // Clear Interrupt Disable
uint8_t CLV(); // Clear Overflow Flag
uint8_t SEC(); // Set Carry Flag
uint8_t SED(); // Set Decimal Flag
uint8_t SEI(); // Set Interrupt Disable

/* System Functions */
uint8_t BRK(); // Force Interrupt
uint8_t NOP(); // No Operation
uint8_t RTI(); // Return from Interrupt

/* Invalid Operations */
uint8_t INVALID(); // Invalid Operation

} // namespace Operation