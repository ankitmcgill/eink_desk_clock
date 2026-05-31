// DRIVER_RTC
// MAY 31, 2026

#ifndef _DRIVER_RTC_
#define _DRIVER_RTC_

// Header Includes
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

// Defines
typedef void (*driver_rtc_tick_cb_t)(void);

// Function Declarations
bool DRIVER_RTC_Init(void);
bool DRIVER_RTC_GetTime(struct tm* ptr);
bool DRIVER_RTC_SetTime(struct tm* ptr);
bool DRIVER_RTC_SetTickCallback(driver_rtc_tick_cb_t cb);

#endif
