# RDA 8809 Peripheral Mapping

Based on the header files and the base addresses, we can map the `0x1A0X_XXXX` region which is mapped to MIPS Uncached Space at `0xA1A0XXXX` in the assembly:

- `0x01A00000` (`0xA1A00000`): SYS_CTRL (System Control, Clocks, PLL)
- `0x01A01000` (`0xA1A01000`): SYS_IRQ (Interrupt Controller)
- `0x01A02000` (`0xA1A02000`): TIMER (OS Timer, Watchdog, HW Timer)
- `0x01A03000` (`0xA1A03000`): GPIO
- `0x01A04000` (`0xA1A04000`): MEM_BRIDGE (Memory Bridge config)
- `0x01A05000` (`0xA1A05000`): KEYPAD
- `0x01A06000` (`0xA1A06000`): PWM (and LED controls)
- `0x01A07000` (`0xA1A07000`): I2C_MASTER
- `0x01A08000` (`0xA1A08000`): DMA
- `0x01A09000` (`0xA1A09000`): SYS_IFC
- `0x01A0A000` (`0xA1A0A000`): CALENDAR
- `0x01A0B000` (`0xA1A0B000`): SYS_COMREGS
- `0x01A0C000` (`0xA1A0C000`): PAGE_SPY
- `0x01A0D000` (`0xA1A0D000`): DEBUG_PORT
- `0x01A0E000` (`0xA1A0E000`): RF_SPI
- `0x01A0F000` (`0xA1A0F000`): TCU

These mappings are critical to understanding how the bootloader configures the system. For instance, when `lui v0, 0xa1a0` is executed, followed by writing to `0(v0)`, the code is modifying the `SYS_CTRL` registers, specifically `REG_SYS_CTRL_REG_DBG` or resetting subsystems depending on the offset.
