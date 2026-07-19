# Hardware Peripherals Structural Mapping

As part of the clean-room implementation for FreeRTOS, we have migrated the macro-based definitions into formal C structs. These are grouped exactly as requested into 16 distinct subsystems matching the `0x01A00000` to `0x01A0FFFF` address space.

All generated `.h` and `.c` files reside in `src/hw_structs/`.

## 1. Core Infrastructure & System Control
- **`sys_ctrl.h` / `sys_ctrl.c`**: Contains `HWP_SYS_CTRL_T` mapped to `0xA1A00000`. Manages system reset vectors, peripheral clock enabling, and PLL configurations.
- **`sys_irq.h` / `sys_irq.c`**: Contains `HWP_SYS_IRQ_T` mapped to `0xA1A01000`. Handles the primary interrupt masks and cause routing registers.
- **`timer.h` / `timer.c`**: Contains `HWP_TIMER_T` mapped to `0xA1A02000`. Exposes OSTimer, Watchdog, and HWTimer blocks with precise 4-byte boundaries.
- **`mem_bridge.h` / `mem_bridge.c`**: Contains `HWP_MEM_BRIDGE_T` mapped to `0xA1A04000`. Exposes the CS configuration limits and wait states for RAM/ROM buses.

## 2. External Interface & Communication Drivers
- **`gpio.h` / `gpio.c`**: Contains `HWP_GPIO_T` mapped to `0xA1A03000`. Structure defines I/O pins, pull-ups, set/clear semantics.
- **`i2c_master.h` / `i2c_master.c`**: Contains `HWP_I2C_MASTER_T` mapped to `0xA1A07000`. (Note: Also points natively to 2/3 extensions internally).
- **`debug_port.h` / `debug_port.c`**: Contains `HWP_DEBUG_PORT_T` mapped to `0xA1A0D000`. Debug interface configurations often used by ROM for serial logging.
- **`rf_spi.h` / `rf_spi.c`**: Contains `HWP_RF_SPI_T` mapped to `0xA1A0E000`. Manages the physical SPI connection to the radio transceiver.

## 3. High-Speed Data Pipelines
- **`dma.h` / `dma.c`**: Contains `HWP_DMA_T` mapped to `0xA1A08000`. Sets DMA transfers across the memory bus.
- **`sys_ifc.h` / `sys_ifc.c`**: Contains `HWP_SYS_IFC_T` mapped to `0xA1A09000`. Direct mapping of Peripheral to Memory DMA configurations.
- **`sys_comregs.h` / `sys_comregs.c`**: Contains `HWP_COMREGS_T` mapped to `0xA1A0B000`. The Mailbox scratchpad used between XCPU and BCPU.

## 4. Housekeeping & Security Modules
- **`keypad.h` / `keypad.c`**: Contains `HWP_KEYPAD_T` mapped to `0xA1A05000`.
- **`pwm.h` / `pwm.c`**: Contains `HWP_PWM_T` mapped to `0xA1A06000`. Maps Light Pulse Generators (LPG) and standard PWM for LEDs.
- **`calendar.h` / `calendar.c`**: Contains `HWP_CALENDAR_T` mapped to `0xA1A0A000`. Maps the RTC logic and alarm interrupts.
- **`page_spy.h` / `page_spy.c`**: Contains `HWP_PAGE_SPY_T` mapped to `0xA1A0C000`. Exposes registers for access violation and memory fault trapping.
- **`tcu.h` / `tcu.c`**: Contains `HWP_TCU_T` mapped to `0xA1A0F000`. Used for strict time alignment relative to network slots.
