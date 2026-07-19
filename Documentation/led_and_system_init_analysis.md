# Bare-Metal LED Initialization & System Environment Trace

## Correction: PMD and HAL Execution Trace
In our previous analysis, we hypothesized that the LED states were managed natively via `REG_PWM_BASE` (`0xA1A06000`) or standard `GPIO_BASE`. Based on new SDK sources provided (`pmd_SetLevel` and `pmd_EnablePower`), the true hardware configuration for the LCD Backlight, Keypad Backlight, and Camera Flash LEDs utilizes the **Power Management Device (PMD)**, typically driven via an SPI or I2C bus bridge connecting the Baseband (or XCPU) to an external companion PMIC (Power Management IC).

### The Software Matrix
The vendor code provided shows:
```c
hal_MapEngineOpen();
pmd_Open(tgt_GetPmdConfig());
pmd_SetPowerMode(PMD_IDLEPOWER);

hal_SwRequestClk(FOURCC_TOOL, HAL_CLK_RATE_104M);
hal_TimWatchDogClose();

pmd_SetLevel(PMD_LEVEL_LCD, 7);
pmd_SetLevel(PMD_LEVEL_KEYPAD, 7);
pmd_EnablePower(PMD_POWER_CAMERA_FLASH, TRUE);
```

### 1. Watchdog and Clock Overrides
`hal_TimWatchDogClose()` directly halts the timer hardware. The source for this confirms:
```c
hwp_timer->WDTimer_Ctrl = TIMER_STOP; // Write 0x10 to 0xA1A02008 (REG_TIMER_WDTIMER_CTRL)
hwp_timer->WDTimer_LoadVal = 0;       // Write 0x0 to 0xA1A0200C
```
This requires specific overrides to the `SYS_CTRL` block, notably modifying the Clock Tree. `hal_SwRequestClk(FOURCC_TOOL, HAL_CLK_RATE_104M)` scales the PLL outputs.

### 2. LED/Backlight Control (PMD Matrix)
`pmd_SetLevel` evaluates the target hardware profile. Depending on the exact PMIC attached (e.g. `pmu_8809` or `pmu_8955`), `PMD_LEVEL_TYPE_PWL0` translates into a call to `hal_PwlSelLevel(HAL_PWL_0, value)`.
When this occurs natively inside the processor (not over an external PMIC I2C bus), it hits the **Pulse Width Light (PWL)** registers:
```c
// Enabling LED/LCD Backlight at level 7
Val = PWM_PWL1_SET_OE | PWM_PWL1_EN_H | PWM_PWL1_THRESHOLD(level);
hwp_pwm->PWL1_Config = Val; // Writes to 0xA1A0600C (REG_PWM_PWL1_CONFIG)
```

However, if it's the `PMD_POWER_CAMERA_FLASH`, it may toggle an actual GPIO via `hal_GpioSetOut()`.

### Replicating the Sequence (Bare-Metal)
To strictly emulate turning on the LEDs manually without the massive HAL/PMD library overhead, you need to:

1. **Kill Watchdog:**
```c
hw_timer->WDTimer_Ctrl = (1 << 4); // TIMER_STOP
hw_timer->WDTimer_LoadVal = 0;
```
2. **Enable PWM Clock:**
```c
hw_sys_ctrl->REG_DBG = SYS_CTRL_WRITE_UNLOCK;
hw_sys_ctrl->Clk_Sys_Enable = (1 << 29); // Enable SYSD_PWM
```
3. **Configure PWL (Pulse Width Light) Register:**
```c
// Turn PWL1 fully ON (Force High, Output Enable)
hw_pwm->PWL1_Config = (1 << 20) | (1 << 18); // PWM_PWL1_SET_OE | PWM_PWL1_FORCE_H
```
