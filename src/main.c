// ============================================================
// RDA 8809 – Backlight ON + 5‑second delay (HWTimer robust)
// ============================================================

typedef unsigned int uint32_t;

#ifndef REG32
#define REG32 volatile uint32_t
#endif

// ---- System Control ----
typedef volatile struct {
    REG32 REG_Dbg;           // 0x00
    REG32 Sys_Rst_Set;       // 0x04
    REG32 Sys_Rst_Clr;       // 0x08
    REG32 Clk_Per_Enable;    // 0x24
} HWP_SYS_CTRL_T;

#define REG_SYS_CTRL_BASE   0xA1A00000
#define hwp_sysCtrl         ((HWP_SYS_CTRL_T*) REG_SYS_CTRL_BASE)
#define SYS_CTRL_PROTECT_UNLOCK  (0xA50001)
#define SYS_CTRL_SOFT_RST        (1 << 31)

// ---- Timer ----
#define TIMER_BASE          0xA1A02000

typedef volatile struct {
    REG32 OSTimer_Ctrl;          // 0x00
    REG32 OSTimer_CurVal;        // 0x04
    REG32 WDTimer_Ctrl;          // 0x08
    REG32 WDTimer_LoadVal;       // 0x0C
    REG32 HWTimer_Ctrl;          // 0x10
    REG32 HWTimer_CurVal;        // 0x14  – down‑counter
    REG32 TimerIRQMaskSet;        //
    REG32 TimerIRQMaskClr;        //
} HWP_TIMER_T;


#define hwp_timer            ((HWP_TIMER_T*) TIMER_BASE)


// ---- SPI3 ----
typedef volatile struct {
    REG32 ctrl;          // 0x00
    REG32 status;        // 0x04
    REG32 rxtx_buffer;   // 0x08
    REG32 cfg;           // 0x0C
    REG32 pin_control;   // 0x18
    REG32 irq;           // 0x1C
} HWP_SPI3_T;

#define REG_SPI3_BASE       0xA1A13000
#define hwp_spi3            ((HWP_SPI3_T*) REG_SPI3_BASE)
#define SPI_CS(cs)              (((cs) & 3) << 29)

#define SAFE_DELAY() \
    for (uint32_t i = 0; i < 999999; i++) { \
        uint32_t b = 10; \
    }

uint32_t __attribute__((section(".fixptr"))) vendor_fixed_pointer = 0x81C00280;

// ============================================================
// Entry point
// ============================================================
void __attribute__((section(".bl_entry"))) bl_entry(void) {
    __asm__ volatile ("li $sp, 0x81C0FF00" : : : "sp");

    // ---- Disable watchdog ----
    hwp_timer->WDTimer_Ctrl  = 0x00;
    hwp_timer->WDTimer_LoadVal = 0x00;

    // ---- Unlock & enable clocks ----
    hwp_sysCtrl->REG_Dbg = SYS_CTRL_PROTECT_UNLOCK;
  //  hwp_sysCtrl->Clk_Per_Enable = 0xFFFFFFFF;
   // hwp_sysCtrl->Sys_Rst_Clr = 0x7FFFFFFF;

    // ---- Configure SPI3 ----
    hwp_spi3->ctrl        = 0x2019D821;
    hwp_spi3->cfg         = 0x00030007;
    hwp_spi3->pin_control = 0x00000000;
    hwp_spi3->irq         = 0x00000000;

    // ---- PMU reset & backlight ON ----
    hwp_spi3->rxtx_buffer = SPI_CS(0) | (0 << 25) | (0x30 << 16) | 0x8001;
    SAFE_DELAY();
    hwp_spi3->rxtx_buffer = SPI_CS(0) | (0 << 25) | (0x01 << 16) | 0x0345;
    SAFE_DELAY();
    hwp_spi3->rxtx_buffer = SPI_CS(0) | (0 << 25) | (0x02 << 16) | 0x4CC0;
    SAFE_DELAY();
    hwp_spi3->rxtx_buffer = SPI_CS(0) | (0 << 25) | (0x04 << 16) | 0x0EF0;
    SAFE_DELAY();
    hwp_spi3->rxtx_buffer = SPI_CS(0) | (0 << 25) | (0x19 << 16) | 0x4CC0;
    SAFE_DELAY();
    hwp_spi3->rxtx_buffer = SPI_CS(0) | (0 << 25) | (0x1A << 16) | 0x4CC0;
    SAFE_DELAY();
    hwp_spi3->rxtx_buffer = SPI_CS(0) | (0 << 25) | (0x36 << 16) | 0x4CC0;
    SAFE_DELAY();
    hwp_spi3->rxtx_buffer = SPI_CS(0) | (0 << 25) | (0x3E << 16) | 0x4CC0;
    SAFE_DELAY();
    hwp_spi3->rxtx_buffer = SPI_CS(0) | (0 << 25) | (0x1A << 16) | 0x0F00;


    // ---- Reboot (success) ----
   // hwp_sysCtrl->Sys_Rst_Set = SYS_CTRL_SOFT_RST;

    while(1) {
        __asm__ volatile ("" : : : "memory");
    }
}
