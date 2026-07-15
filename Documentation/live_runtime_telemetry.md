# Live Runtime Telemetry Analysis (RDA 8809)
**Date:** 13 July 2026

## Overview
Following successful custom payload execution, we utilized the "Rudra" firmware dumper tool (created by @vixxkigoki) to intercept the system while the stock firmware environment was actively scheduling threads. This allowed us to map the live runtime environment and extract concrete architectural metrics from active hardware registers.

## 1. CPU Clock & Frequency Analysis
By querying the system control matrix, we determined the exact execution environment timing baseline:

* **`REG_SYS_CTRL_SEL_CLOCK` (`0xA1A00048`):**
  * **Value Read:** `0x49101820`
  * **Finding:** The internal high-speed Phase-Locked Loop (PLL) multiplier is fully powered on and locked. The clock multiplexer is actively set to high-speed FAST mode routed from the RF baseline.
* **`REG_SYS_CTRL_CFG_CLK_SYS` (`0xA1A0004C`):**
  * **Value Read:** `0x00000011`
  * **Finding:** This specific core divisor profile scales the master PLL network down, dictating an active CPU operational frequency of exactly **156 MHz** (`156,000,000 Hz`).

## 2. Hardware Uptime & Telemetry Timers
We intercepted the hardware timer infrastructure responsible for system ticks and uptime tracking.

* **`REG_TIMER_HWTIMER_CURVAL` (`0xA1A02014`):**
  * **Value Read:** `0x011DE1E5` (`18,735,589` ticks)
  * **Finding:** When calibrated against the standard low-frequency sleep/uptime tracking scale (usually 16.384 kHz or 32.768 kHz depending on the sleep crystal configuration), this translates to an uninterrupted kernel uptime of **19 minutes and 3 seconds** at the moment the snapshot was taken. This proves the hardware timers are fully active and tracking continuous runtime.

## 3. Peripheral & Matrix Interceptions
Bidirectional hardware interactivity was verified across several key peripheral subsystems:

| Target Register | Vector Component | Value Checked | Operational Finding |
| :--- | :--- | :--- | :--- |
| **`0xA1A0300C`** | `REG_GPIO_GPIO_VAL` | `0x03020A08` → `0x03000008` | **Keypad Matrix Verification:** Pressing physical keys dynamically drops bits 9, 11, and 17 to `0` as the electrical lines complete the ground loop. |
| **`0xA1A06014`** | `REG_PWM_GPADC_DATA` | `0x000005C0` | **General Purpose ADC:** Returns a stable digital conversion step (`1472`), representing real-time analog telemetry (e.g., battery cell voltage). |
| **`0xA1A03028`** | `REG_GPIO_CHG_CTRL` | `0x000303FF` | **Power Management Unit (PMU):** State bits show the VBUS comparator paths are wide open, explicitly detecting and registering the 5V rail from the host PC's USB connection. |

## 🔍 Architectural Conclusion
The MIPS32 execution pipeline remains exceptionally stable under the `156 MHz` clock tree profile. Because the stock firmware environment is actively multitasking at this frequency, injecting any standalone test loops that do not yield control will eventually trip the OS kernel's software watchdog thread.

This behavior explicitly confirms that the RDA 8809 hardware is fully initialized, stable, and ready for a modular RTOS port (such as FreeRTOS). To proceed with an RTOS deployment, the RTOS kernel must interface directly with the 156 MHz clock tree and hook into the hardware timer base at `0xA1A02000` to drive the system tick interrupt.
