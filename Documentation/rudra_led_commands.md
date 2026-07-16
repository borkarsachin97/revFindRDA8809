# RDA 8809 LED Control via Rudra

**Date:** 13 July 2026

## Overview
This document outlines the exact `rudra` hardware flasher commands required to manually turn on the onboard LEDs (backlight / RGB) by writing directly to the PMU (Power Management Unit) over the SPI3 (ISPI) bus interface.

The `rudra` interface allows us to directly execute 32-bit hardware register reads (`-rreg`) and writes (`-wreg`).

### SPI Transaction Mechanics
To communicate with the PMU, the command structure utilizes the ISPI Command/Address/Data register mapped at `0xA1A13008` (ISPI TX Buffer). We poll the ISPI Status Register at `0xA1A13004` to ensure the SPI transaction has successfully completed before issuing the next command.

The PMU registers targeted below correspond directly to the LDO enabling and PWM brightness limits analyzed during our reverse engineering of `pmd_SetLevel`.

---

## The Command Sequence

**1. Enable the LED LDO**
* Register: `0x02`
* Data: `0x4CC0`
```bash
sudo ./rudra -wreg 0xA1A13008 0x00024CC0
sudo ./rudra -rreg 0xA1A13004
```

**2. Enable BLLED LDO (LDO_ACTIVE_SETTING2)**
* Register: `0x04`
* Data: `0x0EF0`
* *Purpose:* Clears the `rgbLedOff` bit, providing power to the backlight driver.
```bash
sudo ./rudra -wreg 0xA1A13008 0x00040EF0
sudo ./rudra -rreg 0xA1A13004
```

**3. Direct Backlight Control (LED_SETTING1)**
* Register: `0x19`
* Data: `0x4CC0`
```bash
sudo ./rudra -wreg 0xA1A13008 0x00194CC0
sudo ./rudra -rreg 0xA1A13004
```

**4. Set Brightness Level (LED_SETTING2)**
* Register: `0x1A`
* Data: `0x4CC0`
* *Purpose:* Modifies `BL_IBIT_ACT` and clears `BL_OFF_ACT` to drive current.
```bash
sudo ./rudra -wreg 0xA1A13008 0x001A4CC0
sudo ./rudra -rreg 0xA1A13004
```

**5. Thermal Calibration / Current Limit (THERMAL_CALIBRATION)**
* Register: `0x36`
* Data: `0x4CC0`
* *Purpose:* Modifies the current reduction fraction for the LED driver.
```bash
sudo ./rudra -wreg 0xA1A13008 0x00364CC0
sudo ./rudra -rreg 0xA1A13004
```

**6. RGB LED Enable (LED_SETTING5)**
* Register: `0x3E`
* Data: `0x4CC0`
* *Purpose:* Enables and drives the independent RGB LED array channels.
```bash
sudo ./rudra -wreg 0xA1A13008 0x003E4CC0
sudo ./rudra -rreg 0xA1A13004
```
