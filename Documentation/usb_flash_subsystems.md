# USB and Flash Subsystems

The disassembly contains chunks that handle the hardware initialization block, primarily focused around checking flags and managing clock control configurations (at `SYS_CTRL`).

From our analysis of the `0x81c000cc` and `0x81c00278` fields:
- `0x81C000CC` corresponds to the Boot Profile code flags, matching things set during the `boot_rom` execution phase (e.g. `0x40` means a specific profile/mode is checked to trigger standard initialization or bypass it).
- We see many accesses to `0xA1A0XXXX`, indicating intensive configuration of the `SYS_CTRL`, `TIMER`, and `MEM_BRIDGE` registers during the boot sequence setup.
- Curiously, the USB Subsystem (`USBC` at `0x01A80000`/`0xA1A80000`) does not seem to be actively configured in this precise early trace of the `8809_00400000.fp` binary snippet, suggesting either the USB functionality is deferred to later boot stages (in other `fp` blocks not dumped) or handled implicitly via the initial ROM.

## Flash (SPI)
- The flash boot sector is mapped to `REG_CS0_BASE` (`0x00000000` cached / `0x80000000` uncached) which is KSEG0 `0x80000000`. We observed several references and jumps to `0x81e0...` which may point to either a different SRAM bank or SPI flash memory being addressed (often XIP on these architectures).

The code actively calls into `0x81e0...` memory segments (`jal` and `jalx` for MIPS16 mode). This is almost certainly mapped to external SPI Flash (or another SRAM bank) as the internal SRAM (where the bootloader is executing from) starts at `0x81c00000` and likely ends before `0x81e00000`. The jumps mean that the bootloader (after performing initial setup and hardware mapping checks in SRAM) delegates tasks out to these memory regions for continued execution.
