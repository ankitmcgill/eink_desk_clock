// DRIVER_BUZZER
// MAY 30, 2026

#ifndef _DRIVER_BUZZER_
#define _DRIVER_BUZZER_

// Header Includes
#include <stdint.h>
#include <stdbool.h>

// Defines

// Function Declarations
uint8_t DRIVER_BUZZER_Init(uint8_t pin, uint32_t freq, float duty);
bool DRIVER_BUZZER_On(uint8_t slice);
bool DRIVER_BUZZER_Off(uint8_t slice);

#endif
