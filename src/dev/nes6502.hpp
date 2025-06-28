#pragma once
#include <iostream>

#include "./apu.hpp"
#include <bitset>
#include <cstdint>
#include <vector>

class NES6502 {
public:
  NES6502();
  ~NES6502();

private:
  uint16_t _pc; // Program Counter
  uint8_t  _stp; // Stack Pointer
  uint8_t  _acc; // Accumulator
  uint8_t  _irx; // Index Register X
  uint8_t  _iry; // Index Register Y
  uint8_t  _proc_stat; // Processor Status
  /*
   * 7  bit  0
   * ---- ----
   * NVss DIZC
   * |||| ||||
   * |||| |||+- Carry
   * |||| |||`- Zero
   * |||| ||`- Interrupt Disable
   * |||| |`- Decimal Mode
   * |||| `- Break
   * |||` `- Unused
   * ||` `- Overflow
   */
  std::bitset<8> _pstat_r; // Processor Status Register

  struct Instruction {
    uint8_t opcode;
    uint8_t (NES6502::*addr_mode)(void) = nullptr;
    uint8_t (NES6502::*op_exec)(void) = nullptr;
    uint8_t cycles;
  };
  std::vector<Instruction> _instr;

private:
  /*
 * 6502 Addressing Modes Documentation
 * ===================================
 * 
 * The 6502 microprocessor supports 13 different addressing modes that determine
 * how operands are accessed and how effective addresses are calculated.
 * 
 * IMM (Immediate):
 * ZP (Zero Page):
 * ZPX (Zero Page, X):
 * ZPY (Zero Page, Y):
 * ABS (Absolute):
 * ABSX (Absolute, X):
 * ABSY (Absolute, Y):
 * REL (Relative):
 * IND (Indirect):
 * INDX (Indirect, X):
 * INDY (Indirect, Y):
 *
 * Additional Notes:
 * - Zero page addressing is faster and uses less memory than absolute addressing
 * - Page boundary crossing adds 1 cycle for indexed addressing modes
 * - Indirect addressing has a bug: ($XXFF) doesn't wrap to $XX00 for the high byte
 * - Relative addressing is only used by branch instructions (BCC, BCS, BEQ, etc.)
 * - Immediate addressing is the fastest but only works with read operations
 */

  /**
 * ABS (Absolute):
 *   - Format: $XXXX
 *   - Description: Full 16-bit address (little-endian)
 *   - Example: LDA $1234 loads from memory location $1234
 *   - Bytes: 3 (opcode + low byte + high byte)
 *   - Cycles: 4
 */
  uint8_t ABS();

  /**
 * ABSX (Absolute, X):
 *   - Format: $XXXX,X
 *   - Description: Absolute address plus X register
 *   - Example: LDA $1234,X loads from $1234 + X
 *   - Bytes: 3 (opcode + low byte + high byte)
 *   - Cycles: 4 (5 if page boundary crossed)
 */
  uint8_t ABSX();

  /**
 * ABSY (Absolute, Y):
 *   - Format: $XXXX,Y
 *   - Description: Absolute address plus Y register
 *   - Example: LDA $1234,Y loads from $1234 + Y
 *   - Bytes: 3 (opcode + low byte + high byte)
 *   - Cycles: 4 (5 if page boundary crossed)
 */
  uint8_t ABSY();

  /**
 * ACC (Accumulator):
 *   - Format: (no operand)
 *   - Description: Instruction operates on accumulator
 *   - Example: LDA #$42 loads the value $42 directly into the accumulator
 *   - Bytes: 1 (opcode only)
 */
  uint8_t ACC();

  /**
 * IMM (Immediate):
 *   - Format: #$XX or #XX
 *   - Description: The operand is the next byte in the instruction stream
 *   - Example: LDA #$42 loads the value $42 directly into the accumulator
 *   - Bytes: 2 (opcode + immediate value)
 *   - Cycles: 2
 */
  uint8_t IMM();

  /**
 * IMP (Implied):
 *   - Format: (no operand)
 *   - Description: Instruction operates on accumulator or has no operand
 *   - Example: CLC, SEC, TAX, INX, DEX, etc.
 *   - Bytes: 1 (opcode only)
 *   - Cycles: 2
 */
  uint8_t IMP();

  /**
 * REL (Relative):
 *   - Format: $XX (signed offset)
 *   - Description: Branch instructions only, signed 8-bit offset from PC+2
 *   - Example: BEQ $FE branches back 2 bytes if zero flag is set
 *   - Bytes: 2 (opcode + signed offset)
 *   - Cycles: 2 (3 if branch taken, 4 if page boundary crossed)
 */
  uint8_t REL();

  /**
 * IND (Indirect):
 *   - Format: ($XXXX)
 *   - Description: 16-bit address points to the actual operand address
 *   - Example: JMP ($1234) jumps to the address stored at $1234-$1235
 *   - Bytes: 3 (opcode + low byte + high byte)
 *   - Cycles: 5
 */
  uint8_t IND();

  /**
 * INDX (Indirect, X):
 *   - Format: ($XX,X)
 *   - Description: Zero page address plus X register, then indirect
 *   - Example: LDA ($42,X) loads from address stored at ($0042 + X) & $FF
 *   - Bytes: 2 (opcode + zero page address)
 *   - Cycles: 6
 */
  uint8_t INDX();

  /**
 * INDY (Indirect, Y):
 *   - Format: ($XX),Y
 *   - Description: Zero page address points to 16-bit address, then add Y
 *   - Example: LDA ($42),Y loads from (address at $0042-$0043) + Y
 *   - Bytes: 2 (opcode + zero page address)
 *   - Cycles: 5 (6 if page boundary crossed)
 */
  uint8_t INDY();

  /**
 * ZP (Zero Page):
 *   - Format: $XX
 *   - Description: Operand is a single byte address in zero page (addresses $0000-$00FF)
 *   - Example: LDA $42 loads the value from memory location $0042
 *   - Bytes: 2 (opcode + zero page address)
 *   - Cycles: 3
 */
  uint8_t ZP0();

  /**
 * ZPX (Zero Page, X):
 *   - Format: $XX,X
 *   - Description: Zero page address plus X register (with wraparound)
 *   - Example: LDA $42,X loads from ($0042 + X) & $FF
 *   - Bytes: 2 (opcode + zero page address)
 *   - Cycles: 4
 */
  uint8_t ZPX();

  /**
 * ZPY (Zero Page, Y):
 *   - Format: $XX,Y
 *   - Description: Zero page address plus Y register (with wraparound)
 *   - Example: LDA $42,Y loads from ($0042 + Y) & $FF
 *   - Bytes: 2 (opcode + zero page address)
 *   - Cycles: 4
 */
  uint8_t ZPY();

private:
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
};
