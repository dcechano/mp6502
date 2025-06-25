#pragma once
#include <cstdint>

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
namespace AddrMode {

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

} // namespace AddrMode