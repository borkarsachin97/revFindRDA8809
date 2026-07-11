# Bare-Metal LED Initialization & System Environment Trace

To replicate the physical operational state for toggling the onboard LED exactly as handled by the raw compiled boot sequence, we must mimic the strict sequential configuration matrix tracked in `chunk1dism.txt`.

## 1. System Environment Status

### Interrupt Gating
Before entering any functional control paths or jumping to the polling loop, the firmware aggressively masks the MIPS Coprocessor 0 state:
```assembly
81c00de0: mtc0 zero,$13  ; Clear Exception Cause
81c00de8: mtc0 k1,$12    ; Write to Exception Status (0x0040C001)
```
This entirely shuts down software interrupts and unmasks only the standard non-maskable hardware exceptions bound to KSEG1 vectors. It strictly overrides the `SYS_IRQ` controller by killing the CPU's ability to trigger nested exception handling while the hardware vectors are being aligned.

### Watchdog Timer Control
The code disables the standard Watchdog/OS Timer timeouts by heavily utilizing a master pointer targeting `REG_TIMER_OSTIMER_CTRL`:
```assembly
81c00b24: a1a02000 sb zero,8192(t5)  ; Writes 0x0 to 0xA1A02000
```
This forces the `OSTimer` block into a halted state, preventing an early reboot cascade. Notably, it doesn't write directly to `0xA1A0200C` (WDTIMER), implying that turning off the main OS clock tree cascades down to disable the Watchdog implicitly.

## 2. Compiler Clock & Override Dependencies

To enable the `GPIO` or `PWM` blocks responsible for driving the LED, the `SYS_CTRL` clock gates must be overridden:
- `REG_SYS_CTRL_CLK_PER_ENABLE` (`0xA1A00024`) must be activated for the specific peripheral bitmask (e.g. `1<<10` for PWM or `1<<8` for GPIO as mapped in `sys_ctrl_asm.h`).
- The disassembly shows `sb zero, 0(t5)` writes to `0xA1A00000` (`REG_SYS_CTRL_REG_DBG`). Writing a value to `REG_DBG` unlocks the protected `SYS_CTRL` registers, bypassing the silicon lockout so the firmware can alter the peripheral clock multiplexing without throwing a bus fault.

## 3. Configuration Matrix: Driving the LED

In RDA architectures, onboard status indicators are almost exclusively routed through the **TESTER_CHIP** debugging logic or **GPO** (General Purpose Outputs).
From our disassembly trace, we spotted aggressive stores to the `0x01880000` memory regions:
```assembly
81c009c0: a1880000 sb t0,0(t4)
```
While not traditionally classified as `PWM` (`0xA1A06000`), the `TESTER_CHIP_BASE` is strictly mapped at `0x01880000`. Inside this block (`tester_chip_asm.h`), the macro `REG_TESTER_CHIP_LED_CTRL` (`0x01880040`) directly governs the physical test pads mapped to the evaluation boards.

Additionally, `CFG_REGS` at `0x01A24000` handles the physical `GPO` / `PinMux` routing.
The sequence configures:
1. `REG_DBG` unlock.
2. `SYS_CTRL` clock enablement.
3. `CFG_REGS` PinMux multiplexing to ensure the physical pad acts as a General Purpose Output (`GPO_5_MASK`).
4. Writing the duty cycle or status bit directly into the `TESTER_CHIP_LED_CTRL` or `REG_GPIO_GPO_SET` array.

To replicate this in a custom standalone bare-metal environment:
```c
// 1. Unlock System Registers
hw_sys_ctrl->REG_DBG = SYS_CTRL_WRITE_UNLOCK;

// 2. Clear Watchdog
hw_timer->OSTimer_Ctrl = 0x0;

// 3. Enable GPIO/PWM Clocks
hw_sys_ctrl->Clk_Sys_Enable = SYS_CTRL_ENABLE_SYSD_PWM | 0x...;

// 4. Toggle LED via GPIO (or Tester Chip block)
hw_gpio->gpo_set = (1 << LED_PIN);
```
