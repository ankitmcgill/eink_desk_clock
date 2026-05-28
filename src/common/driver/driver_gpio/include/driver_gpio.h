// DRIVER_GPIO
// MAY 28, 2026

#ifndef _DRIVER_GPIO_
#define _DRIVER_GPIO_

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    DRIVER_GPIO_PIN_DIR_IN = 0,
    DRIVER_GPIO_PIN_DIR_OUT
}driver_gpio_pin_dir_t;

bool DRIVER_GPIO_Setup(uint8_t pin, driver_gpio_pin_dir_t pin_dir);
bool DRIVER_GPIO_Set(uint8_t pin);
bool DRIVER_GPIO_Unset(uint8_t pin);
bool DRIVER_GPIO_Toggle(uint8_t pin);

#endif