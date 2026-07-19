# BootROM Disassembly Analysis

## Sourcing and Location
We analyzed a secondary payload extracted from the physical `0x01E00000` base address, which matches the `REG_INT_ROM_BASE` macro configuration in the RDA SDK.
- **Size:** Exactly 81,920 bytes (80 KB) mapped. Code occupies only ~18 KB, and the rest is `0xFF` or `0x00` padding.
- **Access Pattern:** Since the base is `0x01E00000`, the MIPS processor executes it via the KSEG0 cached window at `0x81E00000` or the KSEG1 uncached window at `0xA1E00000`.

## Disassembly Analysis
We disassembled the raw binary mapped to the `0x81E00000` base execution vector. The results directly corroborate our earlier architectural mappings:

1. **Watchdog Context Saving (`0x81E00014` - `0x81E00030`)**:
   Immediately upon branching, the boot ROM loads `k0` with `0xA198` (`REG_BB_SRAM_BASE`). It stores the core registers (`at`, `a0`, `a1`, `t0`, `t1`, `a3`, `t3`) into offsets of `0xA1980004` to `0xA198002C`. This matches exactly the `boot_rom.S` comments: `// Save context for Watchdog in BB SRAM`.

2. **System Reset Cause Polling (`0x81E00034`)**:
   ```assembly
   1e00034: a0a1043c   lui a0,0xa1a0
   1e00038: a000888c   lw  t0,160(a0)  ; Reading REG_SYS_CTRL_RESET_CAUSE (0xA1A000A0)
   ```
   The ROM polls the hardware reset registers to distinguish cold boots from software watchdog events before it initializes the memory.

3. **Writing Boot Profile Flags (`0x81E0005C`)**:
   ```assembly
   1e0005c: c081013c   lui at,0x81c0
   1e00060: cc0025ac   sw  a1,204(at)  ; Writing to 0x81C000CC
   ```
   The BootROM directly generates the `Boot Profile Code Flags` and stores them in the SRAM header at `0x81C000CC`. This directly answers *how* the SRAM bootloader `chunk_1` accessed flags at that location—the factory ROM pre-calculates and stages them there!

## Conclusion
The provided `bootROM` is unquestionably the factory-programmed masked ROM for the RDA SoC. The operations inside the ROM natively trigger the `SYS_CTRL` validations, initialize `.bss`, and finally hand off execution control to the external payload flash sequence (which lands at `0x81c00280`), acting as the stage-0 bootloader bridging raw silicone to our custom payloads.
