# CPU Memory Map

| Address Range | Size | Description | Category |
|---------------|------|-------------|----------|
| $0000-$07FF | $0800 | 2KB internal RAM | System RAM |
| $0800-$17FF | $0800 | Mirror of $0000-$07FF | System RAM |
| $1800-$17FF | $0800 | Mirror of $0000-$07FF | System RAM |
| $1800-$1FFF | $0800 | Mirror of $0000-$07FF | System RAM |
| $2000-$2007 | $0008 | PPU registers | I/O Registers |
| $2008-$3FFF | $1FF8 | Mirror of $2000-$2007 (repeats every 8 bytes) | I/O Registers |
| $4000-$4017 | $0018 | APU and I/O registers | I/O Registers |
| $4018-$401F | $0008 | APU test mode registers | I/O Registers |
| $4020-$FFFF | $BFE0 | PRG ROM, PRG RAM, and mapper registers | Cartridge Space |
