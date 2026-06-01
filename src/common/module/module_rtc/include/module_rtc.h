// MODULE_RTC
// MAY 31, 2026

#ifndef _MODULE_RTC_
#define _MODULE_RTC_

// Header Includes
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include "util_dataqueue.h"

// Defines
#define MODULE_RTC_NOTIFICATION_TARGET_MAX  (3)

typedef enum
{
    MODULE_RTC_NOTIFICATION_1_SEC = 0,
    MODULE_RTC_NOTIFICATION_1_MIN,
    MODULE_RTC_NOTIFICATION_1_HOUR
}module_rtc_notification_type_t;

// Function Declarations
bool MODULE_RTC_Init(void);
bool MODULE_RTC_GetTime(struct tm* ptr);
bool MODULE_RTC_SetTime(struct tm* ptr);
bool MODULE_RTC_AddNotificationTarget(util_dataqueue_t* dq);

#endif
