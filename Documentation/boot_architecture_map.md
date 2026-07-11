# RDA 8809 Architectural Map & Hardware Boot Lifecycle

Based on cross-referencing the `bootROM` disassembly against the active SRAM `chunk1dism.txt`, we have discovered a major fallacy in the traditional understanding of the RDA 8809 boot process.

## 1. The Core Architectural Discovery
**The Fallacy:** We previously assumed that the code block at `0x81C00280` (analyzed in `chunk1dism.txt`) was a hardware-enforced first-stage boot sequence. We believed the silicon itself mandated the signature checks (like `0xB0075EC7`) before allowing the system to boot.

**The Reality:** The code at `0x81C00280` is entirely a software-level vendor payload (the "Flash Programmer") running out of volatile Internal SRAM. The hardware Boot ROM (`0x81E00000` to `0x81E14000`) is highly minimalist. It does **not** enforce cryptographic signatures or flash image validation blocks at the silicon level. Security enforcement is entirely self-contained within the vendor's software payload.

## 2. The True Silicon Hand-Off Mechanism

The physical Boot ROM executes the following highly rigid, chronological lifecycle immediately upon Power-on:

1. **Minimal Hardware Profile Selection (`0x1E000BC` to `0x1E000D4`)**:
   - The ROM reads from the `SYS_CTRL` block (`lw t0, 160(a0)` from `0xA1A000A0` which is `REG_SYS_CTRL_RESET_CAUSE`).
   - It masks specific boot mode flags and immediately writes them to `0x81C000C8` and `0x81C000CC` (`sw t0, 200(at)`).
   - *Result*: The SRAM headers are populated with the hardware state without executing any payload validation.

2. **Absolute Vector Hand-Off (`0x1E00420` to `0x1E0042C`)**:
   - The Boot ROM concludes its early setup by configuring the CP0 `Status` register.
   - It loads the base pointer to itself (`lui t0, 0x81e0`) and reads an explicit vector from an internal ROM constant offset (`lw t0, 1148(t0)` which equals `0x81E0047C`).
   - The instruction immediately following is `jr t0` (Jump Register unconditionally).
   - *Result*: The ROM blindly pulls the targeted address vector and jumps, completely stepping out of the way and handing absolute authority to whatever code resides at that address in SRAM (which lands at `0x81C00280`).

## 3. Exploitation Loophole for Custom Firmware

Because validation routines are purely software structures living inside the vendor's SRAM image, the silicon boot sequence presents a major exploitation loophole for clean-room deployments:

We can hook our own bare-metal code directly at the entry point (`0x81C00280`) without worrying about generating valid cryptographic signatures or image checksums matching `0xB0075EC7`. The silicon will happily execute our binary as long as we prime the critical hardware registers properly immediately upon receiving the `jr t0` branch.

### Minimalist Bare-Metal Boot Requirements
Upon catching the execution vector at `0x81C00280`, our custom code **must** execute the following critical operations to prevent a hardware crash:

1. **Watchdog Suppression**: The ROM does not completely kill the watchdog. The very first C/Assembly instruction must clear the timer masks and disable the watchdog (`REG_TIMER_WDTIMER_CTRL` at `0xA1A0200C`).
2. **Stack Pointer Configuration**: The `sp` must be pointed cleanly downward from a boundary inside `KSEG0` (e.g. `0x81C08000`).
3. **Clock Gating Initialization**: Peripheral clocks (GPIO, Timer) must be explicitly enabled (`REG_SYS_CTRL_CLK_SYS_ENABLE` at `0xA1A00018`) to allow external hardware signaling (like our LED blink sequence) to function.
