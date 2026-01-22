#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/binary_info.h"

// Defines
#define DELAY_LED_MS    (200)

// Internal Variables

// Internal Functions

int main(void)
{
    // Initialize Wifi Chip Architecture
    cyw43_arch_init();

    // Toggle LED 3 Times
    for(uint8_t i = 0; i < 3; i++)
    {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);
        sleep_ms(DELAY_LED_MS);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);
        sleep_ms(DELAY_LED_MS);
    }
}