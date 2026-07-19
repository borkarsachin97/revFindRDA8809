#include "timer.h"

// TIMER Base address is mapped via hw_timer pointing to 0xA1A02000

void timer_disable_watchdog(void) {
    // Write 0 to OSTimer_Ctrl and WDTimer_Ctrl to halt counters
    hw_timer->OSTimer_Ctrl = 0;
    hw_timer->WDTimer_Ctrl = 0;

    // As seen in the disassembly: sb zero, 8192(t5) -> 0xA1A02000
    // We clear masks and timers completely
    hw_timer->Timer_Irq_Mask_Clr = 0xFFFFFFFF;
    hw_timer->Timer_Irq_Clr = 0xFFFFFFFF;
}
