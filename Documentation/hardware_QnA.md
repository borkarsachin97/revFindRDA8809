# User Answers based on RDA 8809 architecture

**What kind of ROM this soc support and how to flash firmware into it ?**
- Based on the `pdf/datasheet.pdf`, the RDA 8809 (related to the RDA 8851CL architecture) supports booting from an external SPI Flash. The firmware is typically flashed by writing it to the connected SPI memory (usually via a specific flashing protocol like LOD through serial UART, or USB Device mode). A bootloader residing in the internal ROM initializes the system and attempts to load the subsequent boot stage from the SPI flash memory (at `REG_CS0_BASE` / `0x08000000`).

**How many sram and pram and ram is there in soc ?**
- **System SRAM**: Based on `globals.h`, `SYS_SRAM_SIZE = 2^(13+2) = 2^15 = 32 KB`.
- **Baseband SRAM**: Based on `globals.h`, `BB_SRAM_SIZE = 32 KB`.
- **Internal RAM**: Based on `mem_bridge.h`, the size is `32768 * INT_RAM_NB_BLOCK` where `INT_RAM_NB_BLOCK` is 2. So the Internal RAM is `64 KB`.
- **PSRAM**: Based on the datasheet, there is an integrated PSRAM on chip of `64Mb (8 MB)` size.
- **VOC RAM**: For the vocoder/DSP, memory sizes are X (0x5000), Y (0x5000), I (0x5000).

**What are there address ?**
- **System / Internal SRAM Base (for XCPU)**: `0x01C00000` (Loaded from MIPS KSEG0 as `0x81C00000` for uncached/direct access or `0xA1C00000`).
- **Baseband SRAM Base**: `0x01900000`
- **SPI Flash (CS0)**: `0x08000000`

**How to turn on led ?**
- LEDs are controlled via the **PWM** (Pulse Width Modulation) subsystem (mapped at `0x01A06000` / `0xA1A06000`). Specifically, writing to the Light Pulse Generator (LPG) configuration registers like `REG_PWM_LPG_CONFIG` allows software to set the "On Time" (`PWM_LPG_ONTIME`) to modulate LED brightness. Alternatively, LEDs can be toggled using standard **GPIO** pins (`REG_GPIO_BASE` at `0x01A03000`).

**What type of soc is this ?**
- It is a GSM/GPRS Single-Chip Terminal SoC based on the MIPS architecture (containing a main XCPU and a baseband BCPU). It includes an integrated multimedia system, Bluetooth, FM tuner, power management (PMIC/battery charger), and baseband modems (GPRS Class 10).

**How does lcd get initilized?**
- The LCD controller block is called **Gouda** (`REG_GOUDA_BASE_LO`), which supports SPI or parallel connections.
- Initialization occurs by configuring the system control block (`CFG_REGS`) to map the LCD data pins (`CFG_REGS_MODE_PIN_LCD_DATA_8` up to `15` for parallel) or setting `CFG_REGS_SPI_CAM_LCD_MASK`.
- Then, LCD commands are sent to the controller via `gd_lcd_ctrl` and `gd_lcd_timing` to configure the data stride, memory addressing, and rendering destinations (`GOUDA_DESTINATION_LCD_CS_0` or `1`).
