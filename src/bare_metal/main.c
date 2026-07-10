#include <stdint.h>
#include "../hw_structs/sys_ctrl.h"
#include "../hw_structs/timer.h"
#include "../hw_structs/gpio.h"

// Define a test LED pin (e.g. GPIO bit 5)
#define LED_PIN 5

// Simple delay using a spinloop
void delay(volatile uint32_t count) {
    while (count--) {
        __asm__ volatile ("nop");
    }
}

int main(void) {
    // 1. Kill the Watchdog Timer
    timer_disable_watchdog();

    // 2. Stabilize and enable peripheral system clocks
    sys_ctrl_init_clocks();

    // 3. Configure LED pin as output
    gpio_set_output(LED_PIN);

    // 4. Spinloop: Toggle LED infinitely
    while (1) {
        gpio_toggle_pin(LED_PIN);

        // Delay for visual effect
        delay(500000);
    }

    return 0; // Unreachable
}
