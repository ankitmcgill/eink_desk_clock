// MODULE_RTC
// MAY 31, 2026

// Header Includes
#include <time.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "module_rtc.h"
#include "driver_rtc.h"
#include "util_dataqueue.h"
#include "define_rtos_tasks.h"

// Defines

// Local Variables
TaskHandle_t handle_task_rtc;
static util_dataqueue_t* s_notification_targets[MODULE_RTC_NOTIFICATION_TARGET_MAX];
static uint8_t s_notification_targets_count = 0;
static SemaphoreHandle_t s_i2c_mutex = NULL;

// Local Functions
static void s_task_fn(void* pvParameters);
static void s_rtc_tick_cb(void);
static void s_notify(module_rtc_notification_type_t type);

// External Functions

// MODULE_RTC_Init: Initialise The RTC Module And Start Its Task.
//   returns: True On Success, False Otherwise
bool MODULE_RTC_Init(void)
{
    // Create The I2C Access Mutex
    s_i2c_mutex = xSemaphoreCreateMutex();
    if (s_i2c_mutex == NULL)
    {
        return false;
    }

    // Bring Up The RTC Driver Synchronously So The Bus Is Ready Before
    // The Scheduler Starts (Allows MODULE_RTC_SetTime Right After Init)
    DRIVER_RTC_Init();

    // Create The RTC Task
    xTaskCreate(
        s_task_fn,              // Task function
        DEBUG_TAG_RTC,          // Task name
        TASK_STACK_DEPTH_RTC,   // Stack size
        NULL,                   // Task parameter
        TASK_PRIORITY_RTC,      // Task priority
        &handle_task_rtc        // Task handle
    );

    // Register The Tick Callback Once The Task Handle Is Valid
    DRIVER_RTC_SetTickCallback(s_rtc_tick_cb);

    return true;
}

// MODULE_RTC_GetTime: Read The Current Time From The RTC.
//   ptr    : Destination Struct Populated With The Read Time
//   returns: True On Success, False Otherwise
bool MODULE_RTC_GetTime(struct tm* ptr)
{
    bool result;

    // Guard The Shared I2C Bus
    xSemaphoreTake(s_i2c_mutex, portMAX_DELAY);
    result = DRIVER_RTC_GetTime(ptr);
    xSemaphoreGive(s_i2c_mutex);

    return result;
}

// MODULE_RTC_SetTime: Write The Current Time To The RTC.
//   ptr    : Source Struct Holding The Time To Write
//   returns: True On Success, False Otherwise
bool MODULE_RTC_SetTime(struct tm* ptr)
{
    bool result;

    // Guard The Shared I2C Bus
    xSemaphoreTake(s_i2c_mutex, portMAX_DELAY);
    result = DRIVER_RTC_SetTime(ptr);
    xSemaphoreGive(s_i2c_mutex);

    return result;
}

// MODULE_RTC_AddNotificationTarget: Register A Data Queue To Be Notified.
//   dq     : Data Queue To Receive RTC Notifications
//   returns: True If Added, False If The Target Array Is Full
bool MODULE_RTC_AddNotificationTarget(util_dataqueue_t* dq)
{
    if (s_notification_targets_count >= MODULE_RTC_NOTIFICATION_TARGET_MAX)
    {
        return false;
    }

    s_notification_targets[s_notification_targets_count] = dq;
    s_notification_targets_count++;

    return true;
}

// Local Functions

// s_task_fn: RTC Task. Initialises The Driver And Fans Out Notifications.
//   pvParameters : Unused Task Parameter
//   returns: None
static void s_task_fn(void* pvParameters)
{
    uint8_t flags;
    struct tm t;

    while (true)
    {
        // Wait For An Alarm Interrupt Signalled From The ISR
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Determine Which Alarm(s) Fired And Clear Them
        DRIVER_RTC_GetAlarmFlags(&flags);

        if (flags & DRIVER_RTC_ALARM_SEC)
        {
            s_notify(MODULE_RTC_NOTIFICATION_1_SEC);
        }

        if (flags & DRIVER_RTC_ALARM_MIN)
        {
            s_notify(MODULE_RTC_NOTIFICATION_1_MIN);

            // Derive The Hourly Notification On Minute Rollover
            MODULE_RTC_GetTime(&t);
            if (t.tm_min == 0)
            {
                s_notify(MODULE_RTC_NOTIFICATION_1_HOUR);
            }
        }
    }
}

// s_rtc_tick_cb: Driver Tick Callback (ISR Context). Signals The RTC Task.
//   returns: None
static void s_rtc_tick_cb(void)
{
    BaseType_t woken = pdFALSE;

    vTaskNotifyGiveFromISR(handle_task_rtc, &woken);
    portYIELD_FROM_ISR(woken);
}

// s_notify: Queue A Notification To Every Registered Target.
//   type   : Notification Type To Queue
//   returns: None
static void s_notify(module_rtc_notification_type_t type)
{
    util_dataqueue_item_t item;
    uint8_t i;

    item.data = (uint8_t)type;
    item.data_type = DATA_TYPE_NOTIFICATION;

    for (i = 0; i < s_notification_targets_count; i++)
    {
        UTIL_DATAQUEUE_MessageQueue(s_notification_targets[i], &item, 0);
    }
}
