#pragma once
#include <iostream>

#include "./bus.hpp"
#include <bitset>
#include <cstdint>
#include <vector>

struct Instruction;

class NES6502 {
public:
  NES6502();
  ~NES6502();

private:
  /* Program Counter. The address of the next instruction to be executed. */
  uint16_t pc;
  /* Stack Pointer. 0xFF is the top of the stack.
   * Pushing values to the stack decrements the stack pointer.
   */
  uint8_t stp;
  /* Accumulator. The register that holds the result of arithmetic operations. */
  uint8_t acc;
  /* Index Register X. The register that holds the index for indexed addressing modes. */
  uint8_t irx;
  /* Index Register Y. The register that holds the index for indexed addressing modes. */
  uint8_t iry;
  /* Data fetched from memory */
  uint8_t fetched_data;
  /* Memory address */
  uint16_t abs_addr;
  /* Relative address */
  uint16_t rel_addr;

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
  std::bitset<8> pstat_r;

  struct Instruction {
    uint8_t opcode;
    void (NES6502::*addr_mode)(void) = nullptr;
    uint8_t (NES6502::*op_exec)(void) = nullptr;
    uint8_t cycles;
  };
  std::vector<Instruction> instr;
  /* The current instruction being executed. */
  Instruction curr_inst;

  Bus         bus;

private:
  /* Fetch the next operand to the current instruction. Reads from the
   * abs_addr and returns the value read.
   */
  uint8_t fetch();

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
  void    write(uint16_t addr, uint8_t data);

  void    push_stk(uint8_t data);
  uint8_t pop_stk();

private:
  /*
 * 6502 Addressing Modes Documentation
 * ===================================
 * 
 * The 6502 microprocessor supports 13 different addressing modes that determine
 * how operands are accessed and how effective addresses are calculated.
 * 
 * ACC (Accumulator):
 * IMM (Immediate):
 * IMP (Implied):
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
 *   - Shorthand: [ABS]
 *   - Format: $XXXX
 *   - Description: The operand is a 16-bit address in memory.
 *   - Example: LDA $1234 loads from memory location $1234
 *   - Bytes: 3 (opcode + low byte + high byte)
 *   - Cycles: 4
 */
  void ABS();

  /**
 * ABSX (Absolute Indexed X):
 *   - Shorthand: [ABS, X]
 *   - Format: $XXXX,X
 *   - Description: Absolute address plus X register
 *   - Example: LDA $1234,X loads from $1234 + X
 *   - Bytes: 3 (opcode + low byte + high byte)
 *   - Cycles: 4 (5 if page boundary crossed)
 */
  void ABSX();

  /**
 * ABSY (Absolute Indexed Y):
 *   - Shorthand: [ABS, Y]
 *   - Format: $XXXX,Y
 *   - Description: Absolute address plus Y register
 *   - Example: LDA $1234,Y loads from $1234 + Y
 *   - Bytes: 3 (opcode + low byte + high byte)
 *   - Cycles: 4 (5 if page boundary crossed)
 */
  void ABSY();

  /**
 * ACC (Accumulator):
 *   - Shorthand: [Accum]
 *   - Format: (no operand)
 *   - Description: Instruction operates on accumulator
 *   - Example: LDA #$42 loads the value $42 directly into the accumulator
 *   - Bytes: 1 (opcode only)
 */
  void ACC();

  /**
 * IMM (Immediate):
 *   - Shorthand: [IMM]
 *   - Format: #$XX or #XX
 *   - Description: The operand is the next byte in the instruction stream
 *   - Example: LDA #$42 loads the value $42 directly into the accumulator
 *   - Bytes: 2 (opcode + immediate value)
 *   - Cycles: 2
 */
  void IMM();

  /**
 * IMP (Implied):
 *   - Shorthand: [IMP]
 *   - Format: (no operand)
 *   - Description: Instruction operates on accumulator or has no operand
 *   - Example: CLC, SEC, TAX, INX, DEX, etc.
 *   - Bytes: 1 (opcode only)
 *   - Cycles: 2
 */
  void IMP();

  /**
 * REL (Relative):
 *   - Shorthand: [Relative]
 *   - Format: $XX (signed offset)
 *   - Description: Branch instructions only, signed 8-bit offset from PC+2
 *   - Example: BEQ $FE branches back 2 bytes if zero flag is set
 *   - Bytes: 2 (opcode + signed offset)
 *   - Cycles: 2 (3 if branch taken, 4 if page boundary crossed)
 */
  void REL();

  /**
 * IND (Indirect Absolute):
 *   - Shorthand: [(ABS, X)]
 *   - Format: ($XXXX)
 *   - Description: Used only by JMP instruction. It is a combination of
 *     indirect and absolute addressing modes. The operand is a 16-bit address
 *     that will contain another address where the effective (jump) address can be found. 
 *     This allows the destination of the jump to be updated dynamically at 
 *     runtime by mutating the value pointed to by the JMP instruction's immediate operand.
 *   - Example: JMP $1234 will result in reading the 16-bit address at $1234-$1235. This
 *     value will be interpreted as another address to where the effective address of the jump is stored.
 *     $[operand] -> $[intermediate address] -> $[effective address]
 *   - Bytes: 3 (opcode + low byte + high byte)
 *   - Cycles: 5
 */
  void IND();

  /**
 * INDX (Indexed Indirect, X):
 *   - Shorthand: [(IND, X)]
 *   - Format: ($XX,X)
 *   - Description: The second byte of the instruction is added
 *     to the contents of the X register, discarding the carry. The
 *     result points to a memory location on page zero whose contents 
 *     is the low byte of the effective address. The high byte is
 *     read at $[$[low_addr] + 1 (wrapping add)].
 *   - Example: LDA ($42,X) loads from (address at $0042 + Y)
 *   - Bytes: 2 (opcode + zero page address)
 *   - Cycles: 6
 */
  void INDX();

  /**
 * INDY (Indirect Indexed, Y):
 *   - Shorthand: [(IND), Y]
 *   - Format: ($XX),Y
 *   - Description: The operand is a zero page address, the contents
 *     of which are added to the contents of the Y register to form 
 *     the low byte of the effective address. The carry from this addition
 *     is added to the contents of the next zero page memory location to form
 *     the high byte of the effective address. 
 *   - Example: LDA ($42,Y) loads from address stored at ($0042-$0043 + Y) & $FF
 *   - Bytes: 2 (opcode + zero page address)
 *   - Cycles: 6
 */
  void INDY();

  /**
 * ZP (Zero Page):
 *   - Shorthand: [ZP]
 *   - Format: $XX
 *   - Description: Operand is a single byte address in zero page (addresses $0000-$00FF)
 *   - Example: LDA $42 loads the value from memory location $0042
 *   - Bytes: 2 (opcode + zero page address)
 *   - Cycles: 3
 */
  void ZP0();

  /**
 * ZPX (Zero Page, X):
 *   - Shorthand: [ZP, X]
 *   - Format: $XX,X
 *   - Description: Zero page address plus X register (with wraparound)
 *     to form the effective address. No carry is added to the high byte.
 *   - Example: LDA $42,X loads from ($0042 + X) & $FF
 *   - Bytes: 2 (opcode + zero page address)
 *   - Cycles: 4
 */
  void ZPX();

  /**
 * ZPY (Zero Page, Y):
 *   - Shorthand: [ZP, Y]
 *   - Format: $XX,Y
 *   - Description: Zero page address plus Y register (with wraparound)
 *     to form the effective address. No carry is added to the high byte.
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
   * Loads a byte of memory into the X register setting the zero and negative flags.
   * 
   * @return The loaded value
   */
  uint8_t LDX();

  /**
   * Loads a byte of memory into the Y register setting the zero and negative flags.
   * 
   * @return The loaded value
   */
  uint8_t LDY();

  /**
   * Stores the contents of the accumulator into memory.
   * 
   * @return The stored value (accumulator contents)
   */
  uint8_t STA();

  /**
   * Stores the contents of the X register into memory.
   * 
   * @return The stored value (X register contents)
   */
  uint8_t STX();

  /**
   * Stores the contents of the Y register into memory.
   * 
   * @return The stored value (Y register contents)
   */
  uint8_t STY();

  /* Register Transfers */

  /**
   * Transfers the contents of the accumulator to the X register.
   * Sets the zero and negative flags based on the transferred value.
   * 
   * @return The transferred value (X register contents)
   */
  uint8_t TAX();

  /**
   * Transfers the contents of the accumulator to the Y register.
   * Sets the zero and negative flags based on the transferred value.
   * 
   * @return The transferred value (Y register contents)
   */
  uint8_t TAY();

  /**
   * Transfers the contents of the X register to the accumulator.
   * Sets the zero and negative flags based on the transferred value.
   * 
   * @return The transferred value (accumulator contents)
   */
  uint8_t TXA();

  /**
   * Transfers the contents of the Y register to the accumulator.
   * Sets the zero and negative flags based on the transferred value.
   * 
   * @return The transferred value (accumulator contents)
   */
  uint8_t TYA();

  /* Stack Operations */

  /**
   * Transfers the contents of the stack pointer to the X register.
   * Sets the zero and negative flags based on the transferred value.
   * 
   * @return The transferred value (X register contents)
   */
  uint8_t TSX();

  /**
   * Transfers the contents of the X register to the stack pointer.
   * Does not affect any status flags.
   * 
   * @return The transferred value (stack pointer contents)
   */
  uint8_t TXS();

  /**
   * Pushes the contents of the accumulator onto the stack.
   * 
   * @return The pushed value (accumulator contents)
   */
  uint8_t PHA();

  /**
   * Pulls a value from the stack into the accumulator.
   * Sets the zero and negative flags based on the pulled value.
   * 
   * @return The pulled value (accumulator contents)
   */
  uint8_t PLA();

  /**
   * Pushes the processor status register onto the stack.
   * 
   * @return The pushed value (processor status register contents)
   */
  uint8_t PHP();

  /**
   * Pulls a value from the stack into the processor status register.
   * 
   * @return The pulled value (processor status register contents)
   */
  uint8_t PLP();

  /* Bitwise Operations */

  /**
   * Performs a bitwise AND between the accumulator and a memory location.
   * Stores the result in the accumulator and sets the zero and negative flags.
   * 
   * @return The result of the AND operation
   */
  uint8_t AND();

  /**
   * Performs a bitwise exclusive OR between the accumulator and a memory location.
   * Stores the result in the accumulator and sets the zero and negative flags.
   * 
   * @return The result of the XOR operation
   */
  uint8_t EOR();

  /**
   * Performs a bitwise OR between the accumulator and a memory location.
   * Stores the result in the accumulator and sets the zero and negative flags.
   * 
   * @return The result of the OR operation
   */
  uint8_t ORA();

  /**
   * Performs a bitwise AND between the accumulator and a memory location.
   * Sets the zero flag based on the result, and copies bits 6 and 7 of the memory
   * value to the overflow and negative flags respectively.
   * 
   * @return The result of the AND operation
   */
  uint8_t BIT();

  /* Arithmetic Operations */

  /**
   * Adds the contents of a memory location to the accumulator along with the carry bit.
   * Sets the carry, zero, overflow, and negative flags based on the result.
   * 
   * @return The result of the addition
   */
  uint8_t ADC();

  /**
   * Subtracts the contents of a memory location from the accumulator along with the carry bit.
   * Sets the carry, zero, overflow, and negative flags based on the result.
   * 
   * @return The result of the subtraction
   */
  uint8_t SBC();

  /**
   * Compares the contents of the accumulator with a memory location (A - M).
   * Sets the carry, zero, and negative flags based on the comparison.
   * 
   * @return The result of the comparison (accumulator - memory)
   */
  uint8_t CMP();

  /**
   * Compares the contents of the X register with a memory location (X - M).
   * Sets the carry, zero, and negative flags based on the comparison.
   * 
   * @return The result of the comparison (X register - memory)
   */
  uint8_t CPX();

  /**
   * Compares the contents of the Y register with a memory location (Y - M).
   * Sets the carry, zero, and negative flags based on the comparison.
   * 
   * @return The result of the comparison (Y register - memory)
   */
  uint8_t CPY();

  /* Increment/Decrement Operations */

  /**
   * Increments the X register by one.
   * Sets the zero and negative flags based on the result.
   * 
   * @return The incremented value
   */
  uint8_t INX();

  /**
   * Increments the Y register by one.
   * Sets the zero and negative flags based on the result.
   * 
   * @return The incremented value
   */
  uint8_t INY();

  /**
   * Increments the contents of a memory location by one.
   * Sets the zero and negative flags based on the result.
   * 
   * @return The incremented value
   */
  uint8_t INC();

  /**
   * Decrements the contents of a memory location by one.
   * Sets the zero and negative flags based on the result.
   * 
   * @return The decremented value
   */
  uint8_t DEC();

  /**
   * Decrements the X register by one.
   * Sets the zero and negative flags based on the result.
   * 
   * @return The decremented value
   */
  uint8_t DEX();

  /**
   * Decrements the Y register by one.
   * Sets the zero and negative flags based on the result.
   * 
   * @return The decremented value
   */
  uint8_t DEY();

  /* Shift Operations */

  /**
   * Shifts the contents of a memory location or the accumulator left by one bit.
   * The least significant bit is filled with zero, and the most significant bit
   * is shifted into the carry flag. Sets the carry, zero, and negative flags.
   * 
   * @return The shifted value
   */
  uint8_t ASL();

  /**
   * Shifts the contents of a memory location or the accumulator right by one bit.
   * The most significant bit is filled with zero, and the least significant bit
   * is shifted into the carry flag. Sets the carry, zero, and negative flags.
   * 
   * @return The shifted value
   */
  uint8_t LSR();

  /**
   * Rotates the contents of a memory location or the accumulator left by one bit.
   * The carry flag is shifted into the least significant bit, and the most
   * significant bit is shifted into the carry flag. Sets the carry, zero, and negative flags.
   * 
   * @return The rotated value
   */
  uint8_t ROL();

  /**
   * Rotates the contents of a memory location or the accumulator right by one bit.
   * The carry flag is shifted into the most significant bit, and the least
   * significant bit is shifted into the carry flag. Sets the carry, zero, and negative flags.
   * 
   * @return The rotated value
   */
  uint8_t ROR();

  /* Jump Operations */

  /**
   * Transfers program control to a different location specified by the operand.
   * 
   * @return Undefined (jump instruction)
   */
  uint8_t JMP();

  /**
   * Pushes the address of the next instruction minus one onto the stack,
   * then transfers program control to a different location specified by the operand.
   * 
   * @return Undefined (jump instruction)
   */
  uint8_t JSR();

  /**
   * Pulls the program counter from the stack, then adds one to the pulled value.
   * 
   * @return Undefined (return instruction)
   */
  uint8_t RTS();

  /* Branching */

  /**
   * Branches if the carry flag is clear (0).
   * 
   * @return Undefined (branch instruction)
   */
  uint8_t BCC();

  /**
   * Branches if the carry flag is set (1).
   * 
   * @return Undefined (branch instruction)
   */
  uint8_t BCS();

  /**
   * Branches if the zero flag is set (1).
   * 
   * @return Undefined (branch instruction)
   */
  uint8_t BEQ();

  /**
   * Branches if the negative flag is set (1).
   * 
   * @return Undefined (branch instruction)
   */
  uint8_t BMI();

  /**
   * Branches if the zero flag is clear (0).
   * 
   * @return Undefined (branch instruction)
   */
  uint8_t BNE();

  /**
   * Branches if the negative flag is clear (0).
   * 
   * @return Undefined (branch instruction)
   */
  uint8_t BPL();

  /**
   * Branches if the overflow flag is clear (0).
   * 
   * @return Undefined (branch instruction)
   */
  uint8_t BVC();

  /**
   * Branches if the overflow flag is set (1).
   * 
   * @return Undefined (branch instruction)
   */
  uint8_t BVS();

  /* Status Flag Changes */

  /**
   * Clears the carry flag (sets it to 0).
   * 
   * @return Undefined (flag instruction)
   */
  uint8_t CLC();

  /**
   * Clears the decimal flag (sets it to 0).
   * 
   * @return Undefined (flag instruction)
   */
  uint8_t CLD();

  /**
   * Clears the interrupt disable flag (sets it to 0).
   * 
   * @return Undefined (flag instruction)
   */
  uint8_t CLI();

  /**
   * Clears the overflow flag (sets it to 0).
   * 
   * @return Undefined (flag instruction)
   */
  uint8_t CLV();

  /**
   * Sets the carry flag (sets it to 1).
   * 
   * @return Undefined (flag instruction)
   */
  uint8_t SEC();

  /**
   * Sets the decimal flag (sets it to 1).
   * 
   * @return Undefined (flag instruction)
   */
  uint8_t SED();

  /**
   * Sets the interrupt disable flag (sets it to 1).
   * 
   * @return Undefined (flag instruction)
   */
  uint8_t SEI();

  /* System Functions */

  /**
   * Forces the generation of an interrupt request.
   * Sets the break flag and pushes the program counter and processor status onto the stack.
   * 
   * @return Undefined (interrupt instruction)
   */
  uint8_t BRK();

  /**
   * Performs no operation. The program counter is incremented as if a read operation was performed.
   * 
   * @return Undefined (no operation)
   */
  uint8_t NOP();

  /**
   * Pulls the processor status and program counter from the stack.
   * Restores the processor to the state it was in before the interrupt occurred.
   * 
   * @return Undefined (return instruction)
   */
  uint8_t RTI();

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

  void    set_zn(uint8_t val);
};
