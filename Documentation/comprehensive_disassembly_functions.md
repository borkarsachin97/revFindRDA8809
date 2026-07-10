# Comprehensive Disassembly Functions and Loops

## Main Setup Function (`0x81c00280`)
This is the primary entry point in chunk 1, initialized via a jump from chunk 0 (`0x81C000A4`).
1. Allocates stack (`addiu sp,sp,-40`).
2. Reads `0x81C000CC` and stores in `v1`. Extracts the `0x40` bit flag to `v0`.
3. If the bit is set (`beqz v0`), it performs a validation loop checking `0x81C00278`.

## Core Configuration Loops

### Setup Loop for SRAM Initialization
```assembly
81c00364:	8d590274 	lw	t9,628(t2)
81c00368:	00003021 	move	a2,zero
81c0036c:	03200013 	mtlo	t9
...
81c003b4:	1448ffeb 	bne	v0,t0,0x81c00364
```
This loop runs from `0x81c00364` to `0x81c003b4`. It uses `mtlo`, `dmult`, and `mflo` (MIPS Multiply operations) along with reading data to initialize segments of SRAM. It stops when a counter (`v0`) hits the limit (`t0`, which is set to `3` just before the loop).

### Factory ROM Invocation block
Immediately after the SRAM loop, at `0x81c003bc`, it uses `jal 0x81e002c0` and then evaluates ROM jump conditions based on states derived from `632(v0)` (which maps to `0x81C00278`).
If the data evaluates incorrectly, the bootloader loops continuously or resets the hardware.

## Peripheral Register Initialization
Throughout chunk 1, `v0` or `t5` are frequently loaded with `0xA1A0XXXX` addresses, targeting hardware mapped spaces.
- `0xA1A00000` (SYS_CTRL): System resets, clock enables.
- `0xA1A02000` (TIMER): The disassembly shows writes to `a1a02000` indicating timer and watchdog clears (`sb zero,8192(t5)`).
- `0xA1A04410` (MEM_BRIDGE): Writes here are configuring memory bridges for ROM, SRAM, and external SPI CS lines.

## Wait-for-Event Spinloops
```assembly
81c015c4:	24630001 	addiu	v1,v1,1
81c015c8:	8c490000 	lw	t1,0(v0)
81c015cc:	00000000 	nop
81c015d0:	1520fffc 	bnez	t1,0x81c015c4
```
This is a standard polling loop reading from a memory-mapped register pointer `v0` and spinning until `t1` becomes 0. It iterates `v1` on each failure. This is classic busy-waiting for a hardware peripheral (e.g. SPI Flash ready, PLL lock).
