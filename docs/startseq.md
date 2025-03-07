1. **Power-On Reset**
- CPU starts with:
  - A = X = Y = 0
  - P (Status) = 0x34 (IRQ disabled)
  - S (Stack Pointer) = 0xFD
  - Internal RAM is generally uninitialized
- PPU starts with random values in registers

2. **ROM Loading**
- The cartridge ROM is mapped into memory:
  - PRG-ROM maps to CPU address space ($8000-$FFFF)
  - CHR-ROM/RAM maps to PPU address space ($0000-$1FFF)
- The reset vector is read from $FFFC-$FFFD
  - This contains the address where program execution should begin

3. **CPU Reset Sequence**
- CPU reads reset vector from $FFFC-$FFFD
- Sets Program Counter (PC) to this address
- Begins executing code from that location

4. **PPU Initialization**
- Takes about 30,000 CPU cycles (~27384 master clock cycles)
- During this time, the PPU is in an unstable state
- Game code typically waits for this warm-up period
- After warm-up, PPU begins rendering frames

5. **Game Initialization**
- Game code initializes variables
- Sets up PPU registers
- Loads initial graphics data
- Begins main game loop

Example of typical startup code: