// DRIVER_WS2812
// JUNE 8, 2026

#ifndef _DRIVER_WS2812_
#define _DRIVER_WS2812_

// Header Includes
#include <stdint.h>
#include <stdbool.h>

// Defines
#define DRIVER_WS2812_LED_COUNT     (11)

// Function Declarations
bool DRIVER_WS2812_Init(void);
bool DRIVER_WS2812_SetColor(uint32_t* color);

#endif
