# Chronological Execution Path and Internal State Logic

## 1. Timeline of the Initialization Sequence

The RDA 8809 ROM initializes standard KSEG mappings and jumps to the SRAM loaded bootloader firmware. The firmware begins executing at `0x81c00280`.

### Early Flags and Boot Decision
- Upon entry, the stack is immediately shifted.
- The instruction `lw v1, 204(a1)` reads from `0x81C000CC`. This address contains flags set by the Boot ROM describing the reset cause or boot mode (e.g. `0x40`).

### System Control (SYS_CTRL) Initialization
- The disassembly quickly jumps into `lui v0, 0xa1a0` (0xA1A00000 base for `SYS_CTRL`).
- **Clock Initialization:** We see writes relative to this base pointer modifying offsets like `0x040c` (`REG_SYS_CTRL_CFG_CLK_...`) mapping to clock enablement for standard peripherals.

### Watchdog Timer Control
- We observed memory manipulations to `0xA1A02000` block:
  - Instructions like `sb zero, 8192(t5)` directly target `0xA1A02000` (TIMER base). These writes clear the OSTIMER control registers and subsequently disable or feed the watchdog via `REG_TIMER_WDTIMER_CTRL` to prevent unexpected hardware resets during the rest of the bootloader setup.

## 2. Branch & Control Flow Map

The firmware evaluates conditions rapidly during the first hundred instructions.
- `81c0029c: beqz v0, 0x81c002e0`: If the Boot ROM profile masked bits (at `0x81c000cc`) are zero, the firmware jumps entirely over a huge block of state parsing, meaning it bypasses certain extended configurations.
- `81c002b4: bnez a0, 0x81c002e0`: Validates data loaded from `0x81c00278`. If it's malformed (non-zero `0x3` masked), it again jumps to error/skip handlers.

### Loop Transitions
- `81c00364` to `81c003b4`: This is a tight `bne v0, t0, 0x81c00364` loop that iterates exactly 3 times (based on `li t0, 3`). This loop clears or hashes `a1` blocks linked to SRAM payload verification.
- `81c00498` to `81c004ac`: A classic `memcpy` or block zeroing loop in assembly (`lw`, `sw`, `addiu`, `sltu`, `bnez`). It runs until `v0 < a0`.

### Error Handlers
If conditions like matching expected hardware IDs fail (e.g. `15280025 bne t1,t0,0x81c003c4`), it jumps to fallback modes, which execute `jal 0x81e002c0`—a call deep into the ROM, presumably to trigger a reset or halt state.

## 3. RAM State & Data Mapping Analysis

Global variables stored in the `.data` or `.bss` section inside SRAM are used to dictate the execution path.
Based on memory traces:

- **0x81C000CC (`Boot Profile Code`)**: Extracted at entry point `0x81c00280`. Determines if the SOC should go through deep initialization (`andi v0, v1, 0x40`).
- **0x81C00278**: The first structural validation pointer.
  - Accessed at `0x81c002a8`.
  - It is immediately masked with `0x3`. If it's non-zero, it signifies an invalid state or unconfigured pointer, forcing a jump to error recovery (`0x81c002e0`).
  - Next, it is mathematically verified: `addu t0, v0, t1` (where `t1 = 0x7E400000`). This checks if the pointer is within the valid KSEG address bounds.
- **0x81C00274**: The secondary structural pointer.
  - Accessed at `0x81c002e8`.
  - Also verified via bitmask `0x3` for alignment.
  - It proceeds to check bounds against memory blocks: `addu a0, v0, a3` (where `a3 = 0x5e400000`).
  - Following the boundary checks, the code dereferences this pointer `lw t1, 0(v0)` and checks it against magic hardware IDs or CRC signatures (`ori t0, t2, 0x5ec6`). If the pointer structures do not match the expected firmware signatures, execution falls to the Boot ROM.

This structure strongly suggests that `0x81C00274` and `0x81C00278` store memory descriptors (likely containing the base addresses and boundaries of the firmware chunks), generated and placed by the preceding ROM execution before handing control to the SRAM.

### UART PinMux Configuration
- After the clocks are stabilized, the system typically routes internal debug signals via PinMux. In the RDA architecture, `IOMUX` or `CFG_REGS` handles this.
- If we look for writes near the `CFG_REGS_BASE` (`0x01A24000`), we see instructions hitting the `CFG_REGS` offset block. The `CFG_REGS_MODE_PIN_UART...` settings explicitly route the UART TX/RX lines from the internal debug bus to the physical chip test pads to allow for trace output (e.g. standard `printf` via UART).

### Transition to Main Execution
- Early boot concludes when the firmware has validated the CRC descriptors (`0x81C00274`), set up the stack, and initialized `SYS_CTRL` (clocks/PLL/RAM setup).
- The transition occurs right after the final boot verification branches. The code either:
  1. Jumps back to the ROM (`0x81e0...`) if failure occurs.
  2. Falls through into the primary command processor loop (mapped closely to `flash_programmer.c`'s `main()`), which spins and polls for commands from the debug interface or USB to begin programming the SPI flash.

### Master Pointer Optimization for Hardware Access
A key compiler optimization discovered in the disassembly is the use of a master pointer for hardware access.
Instead of loading `lui` for every peripheral block, the compiler initializes `t5` with `0xA1A00000` (the `SYS_CTRL` base).
When the bootloader needs to clear the Watchdog timer, it uses `sb zero, 8192(t5)`. Since `8192` translates to `0x2000`, this instruction elegantly reaches `0xA1A02000`, mapping directly to `REG_TIMER_OSTIMER_CTRL` inside the `TIMER` block. This minimizes instruction density during critical path bootstrapping.

### Payload Boundary Validation (MIPS Overflow Wrapping)
Right after dereferencing `0x81C00278` and `0x81C00274`, the firmware performs bounds validation using a classic MIPS unsigned addition overflow trick:

```assembly
81c002b8:	3c097e40 	lui	t1,0x7e40    ; t1 = 0x7E400000
81c002bc:	00494021 	addu	t0,v0,t1   ; t0 = v0 + 0x7E400000
81c002c0:	3c030001 	lui	v1,0x1       ; v1 = 0x00010000 (65536 bytes / 64 KB)
81c002c4:	0103382b 	sltu	a3,t0,v1   ; a3 = (t0 < 64KB) ? 1 : 0
81c002c8:	14e00006 	bnez	a3,0x81c002e4
```
This is a standard compiler idiom for `if (v0 >= BASE && v0 < BASE + LIMIT)`.
- `v0` is the address pointer.
- `0x7E400000` is the two's complement of `0x81C00000`. By adding `0x7E400000` to a `KSEG0` SRAM pointer (e.g. `0x81C0XXXX`), the result wraps around past `0xFFFFFFFF` back to zero (relative to the base).
- The `sltu` threshold is `0x00010000` (64 KB).

This proves the Boot ROM enforces an exact **64 KB maximum payload limit** for chunks loaded into the `0x81C00000` (`KSEG0`) memory space.
The exact same validation is duplicated for the `KSEG1` uncached region using `0x5E400000` (`-0xA1C00000`), verifying memory blocks accessed without caching.
