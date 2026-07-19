# Low-Level Hardware Sequence Analysis

## 1. Architectural Baseline Questions

**Register Address Width:**
The RDA 8809 internal peripheral register space is structurally a **32-bit layout**. Every definition in the SDK maps hardware to `uint32_t` (`REG32`), aligned to 4-byte boundaries (e.g., offsets jump from `0x00`, `0x04`, `0x08`, `0x0C`). The base pointers (`0xA1A00000`, etc.) inherently address 32-bit registers. However, SPI Flash internal commands or certain sub-system payloads often operate on byte (8-bit) semantics when targeting external chips.

**Disambiguation of `intRegWrite(0x00000005, 0xEE)`:**
In standard embedded C interfaces for this MIPS architecture, `0x00000005` represents the **target internal register offset** (often addressing a specific byte register internal to an external SPI chip or an I2C device), while `0xEE` is the **actual 8-bit data byte** being written. Given the lack of a formal `intRegWrite` function in the standard headers, this pseudo-code format implies writing the payload byte `0xEE` into the register offset `0x05` of a peripheral currently selected by the hardware state.


## 2. Deep-Dive Sequence Analysis

The sequence provided dictates a highly specific hardware validation and execution transition flow. Here is the exact chronological, step-by-step breakdown tracing the physical registers involved in the BootROM-to-Flash-Programmer handoff:

### Initial Memory Bridge Configuration
- **Step 1:** `readWord(0xa1a04410)`
  - **Register:** `REG_MEM_BRIDGE_CS_CONFIG_0_CS_MODE` (inside `MEM_BRIDGE`).
  - **Effect:** The system reads the Chip Select 0 (CS0) configuration, likely checking the current wait-states or physical memory mapping mode assigned to the SPI Flash interface.

### SPI Flash Pre-Conditioning (Reset/Wakeup)
- **Step 2 & 3:** `writeWord(0xa1a25000, 0x00000066)` & `writeWord(0xa1a25000, 0x00000099)`
  - **Register:** `REG_SPI_FLASH_SPI_CMD_ADDR` (inside `SPI_FLASH`).
  - **Effect:** In standard SPI Flash (like Winbond, GigaDevice, etc.), sending opcode `0x66` (Enable Reset) followed immediately by `0x99` (Reset Device) triggers a physical hardware reset of the external flash chip. This forces the external memory into a known state, guaranteeing no hanging burst modes or continuous read modes disrupt the bootloader.

### Internal SPI Flash Unlocking
- **Step 4:** `intRegWrite(0x00000005, 0xEE)`
  - **Context:** The pseudo-code dictates an internal register write via the SPI controller. Offset `0x05` generally refers to the SPI Flash's internal Status Register 1 (RDSR/WRSR).
  - **Effect:** Writing `0xEE` (1110 1110) modifies the block protection or status bits of the external chip, potentially unlocking specific protected sectors for writing (clearing Block Protect or enabling Quad Enable bits).

### System PinMux & Reset State Checks
- **Step 5:** `readWord(0x01a24000)`
  - **Register:** `REG_CONFIG_REGS_BASE` (CFG_REGS).
  - **Effect:** The bootloader reads the system PinMux layout. It likely checks if the external UART debug pins are pulled high/low or configured for normal operation vs programming mode.
- **Step 6:** `readWord(0x01a000a0)`
  - **Register:** `REG_SYS_CTRL_RESET_CAUSE` (inside `SYS_CTRL`).
  - **Effect:** Reads the hardware reset vector (Watchdog, Software Reset, Boot Mode pins).

### Secondary Flash Operation
- **Step 7 & 8:** `writeWord(0xa1a25000, 0x00000066)` & `writeWord(0xa1a25000, 0x00000099)`
  - **Effect:** Another external flash reset. Frequently required if the first reset was ignored because the flash was previously suspended in deep power-down mode (DPD).
- **Step 9:** `intRegWrite(0x00000005, 0xFD)`
  - **Effect:** Modifying the Status Register 1 again. `0xFD` (1111 1101) typically clears or sets strict block protection or quad-mode bits before the primary flashing payload begins executing.

### Boot Mode Software Override
- **Step 10:** `writeWord(0x01a000a0, 0x00200000)`
  - **Register:** `REG_SYS_CTRL_RESET_CAUSE`.
  - **Effect:** The bitmask `0x00200000` corresponds to `SYS_CTRL_SW_BOOT_MODE(n)`. By setting this flag, the bootloader locks the Software Boot Mode flag in hardware. If the system resets or watchdog triggers shortly after, the ROM immediately recognizes it is inside a flashing/programming routine rather than a cold boot.
- **Step 11:** `readWord(0x01a000a0)`
  - **Effect:** Hardware verification read-back. Verifies that the internal APB bus correctly latched the Software Boot Mode bit.

### Software State Acknowledgment
- **Step 12:** `readWord(0x81c000cc)`
  - **Context:** `0x81C000CC` is the active cached SRAM parameter offset (as identified in `chunk_1_disasm.txt: lw v1, 204(a1)`).
  - **Effect:** Reads the current Boot Profile Code passed by the ROM.
- **Step 13:** `writeWord(0x81c000cc, 0x00000006)`
  - **Effect:** The firmware explicitly overwrites the Boot Profile Code in SRAM to `0x00000006`. This tells any subsequent functions in the `while(1)` command loop that the hardware unlock and SPI flash reset phases are perfectly complete, shifting the state machine to purely wait for FPC commands (Erase/Write) over USB or UART.
