# RDA 8809 Custom Payload Analysis: Backlight ON
**Date:** July 16, 2026

## Overview
This document analyzes the newly integrated custom bare-metal payload (`src/main.c`) designed to turn on the keypad and LCD backlights of the RDA 8809 SoC. It highlights the hardware register configurations, SPI initialization, and details the discrepancies between theoretical implementation and practical execution.

## 1. Initialization and Stack Configuration
Unlike the previous soft reboot payload, this implementation manually forces the stack pointer (`$sp`) to the top of the internal SRAM region before executing any C logic:
```c
__asm__ volatile ("li $sp, 0x81C0FF00" : : : "sp");
```
This is a critical stabilization step. Because this code executes as a raw `.bin` via the ROM bootloader, there is no OS or `start.S` assembly stub to set up the stack frame. Hardcoding the stack pointer to `0x81C0FF00` guarantees that local variables and function calls (if any) will not corrupt the execution entry point at `0x81C00280`.

## 2. Watchdog and System Unlock
To prevent the hardware watchdog from resetting the system during our custom execution loop, the payload zeroes out the `WDTimer_Ctrl` and `WDTimer_LoadVal` registers at `0xA1A02000`. Following this, the system control matrix at `0xA1A00000` is unlocked by writing the magic word `0xA50001`.

*Note:* The code currently comments out the `Clk_Per_Enable` and `Sys_Rst_Clr` peripheral clock overrides. This indicates that the Boot ROM already leaves the SPI3 peripheral clock running before jumping to our payload.

## 3. SPI3 Configuration
The SPI3 controller (`0xA1A13000`) is manually brought online with the following specific configuration words:
* `ctrl = 0x2019D821`: This matches the idle state configuration previously extracted from the vendor disassembly. It enables the SPI block (`SPI_ENABLE` bit 0) and sets the frame size to 32-bits.
* `cfg = 0x00030007`: Sets the `SPI_CLOCK_DIVIDER` to 3, yielding an operational SPI clock of ~13 MHz.

## 4. PMU Backlight SPI Writes
To turn the LCD and keypad backlights on, the payload pushes raw 32-bit frames directly into the SPI3 TX buffer (`0xA1A13008`). The commands follow the structure:
`SPI_CS(0) | (0 << 25) | (Register << 16) | Data`

The sequence executed is:
1. `0x30` = `0x8001` (PMU_RESET)
2. `0x01` = `0x0345` (IRQ settings)
3. `0x02` = `0x4CC0` (LDO Enable)
4. `0x04` = `0x0EF0` (BLLED LDO Enable)
5. `0x19` = `0x4CC0` (LED_SETTING1 - Direct control)
6. `0x1A` = `0x4CC0` (LED_SETTING2 - Brightness levels)
7. `0x36` = `0x4CC0` (THERMAL_CALIBRATION - Max current limit)
8. `0x3E` = `0x4CC0` (LED_SETTING5 - RGB LEDs)
9. `0x1A` = `0x0F00` (LED_SETTING2 - Final Brightness target)

## 5. Software Delay vs. Hardware Polling
A major structural discrepancy exists between this payload and the stock vendor firmware:

**Hardware Polling (Stock):**
In the stock firmware (e.g., `hal_IspiRegWrite`), after pushing a frame to the SPI TX buffer, the code enters a `while` loop that reads the SPI status register (`0xA1A13004`) until the `SPI_ACTIVE_STATUS` bit drops and the `TX_SPACE` FIFO clears.

**Software Delay (Payload):**
This custom payload entirely bypasses status polling. Instead, it utilizes a brute-force software `for` loop macro:
```c
#define SAFE_DELAY() \
    for (uint32_t i = 0; i < 999999; i++) { \
        uint32_t b = 10; \
    }
```
This forces the MIPS core to stall, artificially giving the SPI hardware enough time to shift the bits out over the MOSI line before the next command is written. While not robust for production RTOS environments, it is highly effective for bare-metal proof-of-concept injection.

## 6. Implementation Notes & Discrepancies
* **Header Mismatch:** The payload is titled `// RDA 8809 – Backlight ON + 5-second delay (HWTimer robust)`. However, the implementation does not utilize the `HWTimer_CurVal` (`0xA1A02014`) for timing, nor does it delay for 5 seconds before rebooting. It uses the `SAFE_DELAY()` software loop between SPI writes, and the final reboot line (`hwp_sysCtrl->Sys_Rst_Set = SYS_CTRL_SOFT_RST;`) is commented out, meaning the payload hangs indefinitely in the final `while(1)` trap.
