#include "gpio.h"

// GPIO Base address is mapped via hw_gpio pointing to 0xA1A03000

void gpio_set_output(uint32_t pin) {
    // REG_GPIO_GPIO_OEN_SET_OUT is at offset 0x04
    hw_gpio->gpio_oen_set_out = (1 << pin);
}

void gpio_toggle_pin(uint32_t pin) {
    // If the pin is currently high, clear it. Else set it.
    // REG_GPIO_GPIO_VAL is at offset 0x0C
    if (hw_gpio->gpio_val & (1 << pin)) {
        // REG_GPIO_GPIO_CLR is at offset 0x14
        hw_gpio->gpio_clr = (1 << pin);
    } else {
        // REG_GPIO_GPIO_SET is at offset 0x10
        hw_gpio->gpio_set = (1 << pin);
    }
}
