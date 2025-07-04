#pragma once
#include <iostream>

#include "./bus.hpp"
#include <bitset>
#include <cstdint>
#include <vector>

class NES6502 {
public:
  NES6502();
  ~NES6502();

private:
  /* Opcode. The current instruction being executed. */
  uint8_t _opcode;
  /* Program Counter. The address of the next instruction to be executed. */
  uint16_t _pc;
  /* Stack Pointer. 0xFF is the top of the stack.
   * Pushing values to the stack decrements the stack pointer.
   */
  uint8_t _stp;
  /* Accumulator. The register that holds the result of arithmetic operations. */
  uint8_t _acc;
  /* Index Register X. The register that holds the index for indexed addressing modes. */
  uint8_t _irx;
  /* Index Register Y. The register that holds the index for indexed addressing modes. */
  uint8_t _iry;
  /* Data fetched from memory */
  uint8_t _fetched_data;
  /* Memory address */
  uint16_t _abs_addr;
  /* Relative address */
  uint16_t _rel_addr;

  /*
   * Processor Status Register
   * 
   * This diagram shows big endian order for the purpose of 
   * printing, but in reality we use the indices shown.
   * Index 0 is Carry, 1 is Zero, etc.
   * 
   * 76543210
   * NV1BDIZC
   * ||||||||
   * |||||||+- Carry
   * ||||||+-- Zero
   * |||||+-- Interrupt Disable
   * ||||+-- Decimal Mode
   * |||+--- Break
   * ||+---- Unused
   * |+---- Overflow
   * +-- Negative
   */
  std::bitset<8> _pstat_r;

  struct Instruction {
    uint8_t opcode;
    void (NES6502::*addr_mode)(void) = nullptr;
    uint8_t (NES6502::*op_exec)(void) = nullptr;
    uint8_t cycles;
  };
  std::vector<Instruction> _instr;

  Bus                      _bus;

private:
  /* Cycle operations */

  /* Fetch the next byte pointed to by the program counter.
   * Increments the program counter by 1.
   */
  uint8_t read_pc8();

  /* Fetch the next 2 bytes pointed to by the program counter.
   * Increments the program counter by 2.
   */
  uint16_t read_pc16();

  /* Read a byte from memory. */
  uint8_t read8(uint16_t addr);

  /* Read a 16-bit value from memory. */
  uint16_t read16(uint16_t addr);

  /* Read a 16-bit value from zero page memory. */
  uint16_t read16_zp(uint16_t addr);

  /* Write a byte to memory. */
  void write(uint16_t addr, uint8_t data);

  void push_stk(uint8_t data);
  uint8_t pop_stk();

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
  void ABS();

  /**
 * ABSX (Absolute, X):
 *   - Format: $XXXX,X
 *   - Description: Absolute address plus X register
 *   - Example: LDA $1234,X loads from $1234 + X
 *   - Bytes: 3 (opcode + low byte + high byte)
 *   - Cycles: 4 (5 if page boundary crossed)
 */
  void ABSX();

  /**
 * ABSY (Absolute, Y):
 *   - Format: $XXXX,Y
 *   - Description: Absolute address plus Y register
 *   - Example: LDA $1234,Y loads from $1234 + Y
 *   - Bytes: 3 (opcode + low byte + high byte)
 *   - Cycles: 4 (5 if page boundary crossed)
 */
  void ABSY();

  /**
 * ACC (Accumulator):
 *   - Format: (no operand)
 *   - Description: Instruction operates on accumulator
 *   - Example: LDA #$42 loads the value $42 directly into the accumulator
 *   - Bytes: 1 (opcode only)
 */
  void ACC();

  /**
 * IMM (Immediate):
 *   - Format: #$XX or #XX
 *   - Description: The operand is the next byte in the instruction stream
 *   - Example: LDA #$42 loads the value $42 directly into the accumulator
 *   - Bytes: 2 (opcode + immediate value)
 *   - Cycles: 2
 */
  void IMM();

  /**
 * IMP (Implied):
 *   - Format: (no operand)
 *   - Description: Instruction operates on accumulator or has no operand
 *   - Example: CLC, SEC, TAX, INX, DEX, etc.
 *   - Bytes: 1 (opcode only)
 *   - Cycles: 2
 */
  void IMP();

  /**
 * REL (Relative):
 *   - Format: $XX (signed offset)
 *   - Description: Branch instructions only, signed 8-bit offset from PC+2
 *   - Example: BEQ $FE branches back 2 bytes if zero flag is set
 *   - Bytes: 2 (opcode + signed offset)
 *   - Cycles: 2 (3 if branch taken, 4 if page boundary crossed)
 */
  void REL();

  /**
 * IND (Indirect):
 *   - Format: ($XXXX)
 *   - Description: 16-bit address points to the actual operand address
 *   - Example: JMP ($1234) jumps to the address stored at $1234-$1235
 *   - Bytes: 3 (opcode + low byte + high byte)
 *   - Cycles: 5
 */
  void IND();

  /**
 * INDX (Indirect, X):
 *   - Format: ($XX,X)
 *   - Description: Zero page address plus X register, then indirect
 *   - Example: LDA ($42,X) loads from address stored at ($0042 + X) & $FF
 *   - Bytes: 2 (opcode + zero page address)
 *   - Cycles: 6
 */
  void INDX();

  /**
 * INDY (Indirect, Y):
 *   - Format: ($XX),Y
 *   - Description: Zero page address points to 16-bit address, then add Y
 *   - Example: LDA ($42),Y loads from (address at $0042-$0043) + Y
 *   - Bytes: 2 (opcode + zero page address)
 *   - Cycles: 5 (6 if page boundary crossed)
 */
  void INDY();

  /**
 * ZP (Zero Page):
 *   - Format: $XX
 *   - Description: Operand is a single byte address in zero page (addresses $0000-$00FF)
 *   - Example: LDA $42 loads the value from memory location $0042
 *   - Bytes: 2 (opcode + zero page address)
 *   - Cycles: 3
 */
  void ZP0();

  /**
 * ZPX (Zero Page, X):
 *   - Format: $XX,X
 *   - Description: Zero page address plus X register (with wraparound)
 *   - Example: LDA $42,X loads from ($0042 + X) & $FF
 *   - Bytes: 2 (opcode + zero page address)
 *   - Cycles: 4
 */
  void ZPX();

  /**
 * ZPY (Zero Page, Y):
 *   - Format: $XX,Y
 *   - Description: Zero page address plus Y register (with wraparound)
 *   - Example: LDA $42,Y loads from ($0042 + Y) & $FF
 *   - Bytes: 2 (opcode + zero page address)
 *   - Cycles: 4
 */
  void ZPY();

private:
  /* 
 * 6502 Operation Set Reference
 * http://www.6502.org/users/obelisk/6502/instructions.html
 *
*/
  /* Load/Store Operations */
  /**
   * Loads a byte of memory into the accumulator setting the zero and
   * negative flags as appropriate.
   */
  uint8_t LDA(); // Load Accumulator

  /**
   * Loads a byte of memory into the X register setting the zero and
   */
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

private:
  /* Flag operations*/

  void    set_carry(bool flag);
  void    set_zero(bool flag);
  void    set_interrupt_disable(bool flag);
  void    set_decimal_mode(bool flag);
  void    set_break(bool flag);
  void    set_unused(bool flag);
  void    set_overflow(bool flag);
  void    set_negative(bool flag);

  uint8_t get_carry();
  uint8_t get_zero();
  uint8_t get_interrupt_disable();
  uint8_t get_decimal_mode();
  uint8_t get_break();
  uint8_t get_unused();
  uint8_t get_overflow();
  uint8_t get_negative();
};
