# Sub-register Level Analysis of the Bootloader Initialization

By tracing the base pointers loaded with `lui` (Load Upper Immediate) and the subsequent offsets in memory instructions, we have extracted the precise hardware micro-addresses and their functionalities.

## Block: `0xA1A00000` (`SYS_CTRL`)
- **Base `0xA1A00000`**
- **Offset `0x00A0` (`0xA1A000A0`)**: `REG_SYS_CTRL_RESET_CAUSE`
  - *Disassembly:* `lw v1,160(a1)`
  - *Context:* The bootloader reads the reset cause to determine if this was a cold boot, watchdog reset, or wakeup.

## Block: `0xA1A04000` (`MEM_BRIDGE`)
- **Base `0xA1A04000`**
- **Offset `0x040C` (`0xA1A0440C`)**: `REG_MEM_BRIDGE_CS_TIME_WRITE`
  - *Disassembly:* `sw a2,1036(t2)`
  - *Context:* Sets the Chip Select (CS) write timing for external memory/peripherals on the memory bridge.
- **Offset `0x0418` (`0xA1A04418`)**: `REG_MEM_BRIDGE_CS_CONFIG_1_CS_MODE`
  - *Disassembly:* `sw a1,1048(t2)`
  - *Context:* Configures the operating mode for Chip Select 1 (CS1).

## Block: `0xA1A0D000` (`DEBUG_PORT`)
- **Base `0xA1A0D000`**
- **Offset `0x0000` (`0xA1A0D000`)**: `REG_DEBUG_PORT_DEBUG_PORT_MODE`
  - *Disassembly:* `sw a0,0(t1)` / `lw a1,0(t1)`
  - *Context:* Configures the system debug port (enables logging over specific interfaces).
- **Offset `0x0044` (`0xA1A0D044`)**: `REG_DEBUG_PORT_PXTS_TAG_1`
  - *Disassembly:* `sw v1,68(t1)`
  - *Context:* Writing tags for the host debugging suite or profiler logic.

## Block: `0xA1A25000` (`SPI_FLASH`)
- **Base `0xA1A25000`**
- **Offset `0x0014` (`0xA1A25014`)**: `REG_SPI_FLASH_SPI_CONFIG`
  - *Disassembly:* `sw ra,0(a1)` (where `a1` was pre-calculated to `0xA1A25014`)
  - *Context:* Writes the main configuration register for the SPI Flash controller. This happens after the SRAM setup but before the main flash jumping logic.

## Block: `0x81C00000` (`INTERNAL SRAM / BOOT STATE`)
- **Offset `0x00CC` (`0x81C000CC`)**: Boot Profile Code
  - *Disassembly:* `lw v1,204(a1)`
  - *Context:* Loaded at entry, masked with `0x40`.
- **Offset `0x0274` / `0x0278` (`0x81C00274` / `0x81C00278`)**: Jump Configuration / Reset states.
  - *Disassembly:* `lw v0,632(a2)` / `lw a1,628(v0)`
  - *Context:* State flags heavily referenced in the initial jump vectoring to determine validation and booting schemes.
