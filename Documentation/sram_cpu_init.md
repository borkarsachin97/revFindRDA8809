# SRAM and CPU Initialization Analysis

The boot ROM loads `chunk_1_0x01c0027c.bin` into SRAM starting at `0x81c0027c` and jumps to `0x81c00280`.

```assembly
81c00280:	3c0581c0 	lui	a1,0x81c0
81c00284:	8ca300cc 	lw	v1,204(a1)       ; Load 0x81C000CC (boot state)
81c00288:	27bdffd8 	addiu	sp,sp,-40      ; Allocate stack frame in SRAM
81c0028c:	30620040 	andi	v0,v1,0x40     ; Check flag 0x40
...
81c0029c:	10400010 	beqz	v0,0x81c002e0  ; Jump over some init if 0
81c002a0:	00808021 	move	s0,a0
81c002a4:	3c0681c0 	lui	a2,0x81c0
81c002a8:	8cc20278 	lw	v0,632(a2)       ; Load 0x81c00278 (more boot state)
```

1. **Stack Initialization**: Stack pointer (`sp`) is assumed to have been initialized by the ROM (which usually sets it to the top of internal SRAM). The chunk immediately decrements it by 40 (`0x28` bytes) to create its own stack frame.
2. **Boot Flags Check**: It checks `0x81C000CC` against `0x40`. Depending on these bits (and `0x81C00278`), it determines the boot profile mode.
3. **Hardware Resets**:
   - `SYS_CTRL_SYS_RST_CLR` is located at offset `0x008` from `SYS_CTRL_BASE` (`0xA1A00000`). Disassembly later handles peripheral clocks and resets via writes relative to `a1a0XXXX`.
   - Before handing over execution to the main application or downloading further over USB, the core initializes SRAM variables based on the boot profile flags.
