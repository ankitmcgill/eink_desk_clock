// DRIVER_GPIO
// MAY 28, 2026

#include "pico/stdlib.h"

#include "driver_gpio.h"

// Local Variables

// Local Functions

// External Functions
bool DRIVER_GPIO_Setup(uint8_t pin, driver_gpio_pin_dir_t pin_dir)
{
    //Set Pin Parameters

    gpio_init(pin);
    gpio_set_dir(pin, pin_dir);
    DRIVER_GPIO_Set(pin);
}

bool DRIVER_GPIO_Set(uint8_t pin)
{
    // Set Specified GPIO

    gpio_put(pin, true);
}

bool DRIVER_GPIO_Unset(uint8_t pin)
{
    // Unset Specified GPIO

    gpio_put(pin, false);
}

bool DRIVER_GPIO_Toggle(uint8_t pin)
{
    // Toggle Specified GPIO

    bool gpio_val = gpio_get(pin);
    gpio_put(pin, !gpio_val);
}