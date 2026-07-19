# Technical Findings from Habr Article "Minor improvement of firmware for a push-button phone on RDA8826 (SC6533, MIPS)"
**Date:** 13 July 2026

## Overview
This document summarizes the reverse-engineering insights and architectural findings extracted from the Habr article concerning the RDA8826 (also known as SC6533g) feature phone SoC. This SoC is closely related to the RDA8809 series analyzed in this project.

## Key Technical Findings

### 1. SoC and Architecture Details
* **Processor:** The RDA8826 operates on the RDA8809e2 core, utilizing the **MIPS16e** Application Specific Extension. This confirms our earlier analysis that the core supports both 32-bit (MIPS32) and compressed 16-bit (MIPS16e) instruction sets to minimize firmware footprint.
* **Memory Map (Confirmed):**
    * **Code Base / Flash:** `0x08000000` or `0x88000000` (4 MB)
    * **RAM (PSRAM):** `0x82000000` (8 MB)
    * **SRAM:** `0x81C00000` (64 KB)
    * **BootROM:** `0x81E00000`
    * **Config Registers Base:** `0x01A24000` (accessed via KSEG1 as `0xA1A24000`)
* **Components:** It features a baseband processor (CT8851C), Bluetooth (rdabt_8809), and FM modules (rdafm_8809).

### 2. Firmware Characteristics & "CoolMMI"
* **Firmware Format:** The official firmware is supplied in a `.lod` format. This format describes the flashing process block-by-block, specifying load addresses and binary data encoded as 4-byte little-endian.
* **Operating System:** The UI is built upon "CoolMMI", a fork of the Pixtel MMI framework. It shares significant architectural similarities with Mediatek's PlutoMMI.
* **Code Quality & Architecture:** The OS heavily relies on global variables, monolithic unified `.c` files controlled by massive `#define` blocks, and magic constants. Menu items, strings, and resources are referenced by numeric IDs rather than pointers, making static analysis difficult.

### 3. Compression and BootROM Reuse
* **BootROM Overlays:** To save flash space, common C library functions (`strlen`, `strcmp`, `memcpy`, `sprintf`, `qsort`, etc.) are hardcoded into the BootROM and executed directly from there.
* **LZMA Compression:** Major modules (WAP browser, media player, FM radio, engineering menu) and language strings are compressed using LZMA on the flash. They are decompressed into RAM (`0x82000000`) exactly once upon boot.
* **RAM Execution:** Critical OS code is copied from Flash into RAM and executed from there to maximize execution speed.

### 4. Reverse Engineering Quirks
* **RAM Dumping via USB:** The author discovered that the flashing protocol allows arbitrary memory reads while the phone is running. By bridging a UART read tool to USB via `usb-serial` driver overrides (`echo 1e04 0900 | sudo tee /sys/bus/usb-serial/drivers/generic/new_id`), the entire 8 MB of RAM can be dumped. This allows researchers to analyze the uncompressed resources and code natively.
* **MIPS16e Toolchain Issues:** Standard inline assembly tools (like `rasm2` or `kstool`) fail or struggle to correctly assemble MIPS16e instructions. The author recommends using `mips64-linux-gnu-as` with the `-mips16 -32` flags, noting that GCC will pad and align blocks with `nop` instructions automatically.

### 5. Specific Hardware Tweaks Analyzed
* **Gameloft DRM Bypass:** Paid games use a simple pseudo-random generation based on the current minute, hashed with the phone's ID (e.g., `28060` for Inoi 101) to generate a response code. The author bypassed this by patching the uncompressed initialization functions in RAM.
* **Audio Volume Configuration:**
    * **Audio Control Register:** `audio_spk_cfg` located at `0x00000028` relative to the Config Base (`0xA1A24028` in KSEG1).
    * **SPK_GAIN Bitmask:** `(((n)&15)<<0)`
    * The volume mapping uses a software translation function that maps user UI levels (e.g., 0-7) to hardcoded gain jumps (e.g., 2, 4, 6, 8, 10, 12, 14). Modifying the immediate values in the `volume_maps` function smooths out the volume curve.

## Conclusion
The article strongly validates our ongoing structural reverse-engineering of the RDA 8809. It confirms the `0x81C...` SRAM, the `0xA1A...` hardware overlay space, and the fundamental presence of MIPS16e mixed-mode execution. Furthermore, it highlights how the vendor leverages LZMA packing and BootROM library reuse to fit complex execution payloads into highly constrained flash environments.
