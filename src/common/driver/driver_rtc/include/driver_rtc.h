// DRIVER_RTC
// MAY 31, 2026

#ifndef _DRIVER_RTC_
#define _DRIVER_RTC_

// Header Includes
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

// Defines
#define DRIVER_RTC_ALARM_SEC    (0x01)  // Alarm1 (Once Per Second) Fired
#define DRIVER_RTC_ALARM_MIN    (0x02)  // Alarm2 (Once Per Minute) Fired

typedef void (*driver_rtc_tick_cb_t)(void);

// Function Declarations
bool DRIVER_RTC_Init(void);
bool DRIVER_RTC_GetTime(struct tm* ptr);
bool DRIVER_RTC_SetTime(struct tm* ptr);
bool DRIVER_RTC_SetTickCallback(driver_rtc_tick_cb_t cb);
bool DRIVER_RTC_GetAlarmFlags(uint8_t* flags);

#endif
