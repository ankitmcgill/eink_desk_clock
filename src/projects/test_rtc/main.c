#include <stdio.h>
#include <string.h>
#include <time.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/binary_info.h"
#include "FreeRTOS.h"
#include "task.h"

#include "module_rtc.h"
#include "util_dataqueue.h"
#include "util_logging.h"
#include "define_rtos_tasks.h"
#include "bsp.h"

// Defines
#define MAIN_NOTIFICATION_QUEUE_LEN     (8)

// Internal Variables
TaskHandle_t handle_task_main;
static util_dataqueue_t s_main_dq;

// Internal Functions
static void s_task_fn(void *pvParameters);
static void s_print_program_information(void);

int main(void)
{
    // Initialize All Enabled Stdio Backends
    stdio_init_all();

    // Initialize Wifi Chip Architecture
    cyw43_arch_init();

    LOG_INFO("Starting Main ...");
    s_print_program_information();

    // Create This Task's Notification Queue And Register It With module_rtc
    UTIL_DATAQUEUE_Create(&s_main_dq, MAIN_NOTIFICATION_QUEUE_LEN);

    // Initializing Software Modules
    MODULE_RTC_Init(); // RTC
    MODULE_RTC_AddNotificationTarget(&s_main_dq);

    // Seed The RTC With A Known Baseline 2000-01-01 00:00:00
    {
        struct tm t;
        memset(&t, 0, sizeof(t));
        t.tm_mday = 1;    // Day Of Month 1-31
        t.tm_year = 100;  // Years Since 1900 (2000)
        MODULE_RTC_SetTime(&t);
    }

    LOG_INFO("Starting Main task ...");

    /* Create task */
    xTaskCreate(
        s_task_fn,              // Task function
        "MainTask",             // Task name
        TASK_STACK_DEPTH_MAIN,  // Stack size
        NULL,                   // Task parameter
        TASK_PRIORITY_MAIN,     // Task priority
        &handle_task_main       // Task handle
    );

    /* Start scheduler */
    vTaskStartScheduler();
}

static void s_task_fn(void *pvParameters)
{
    util_dataqueue_item_t item;
    struct tm t;

    while(true)
    {
        // Block Until An RTC Notification Arrives
        if (!UTIL_DATAQUEUE_MessageGet(&s_main_dq, &item, portMAX_DELAY))
        {
            continue;
        }

        // Read The Current Time For The Log
        MODULE_RTC_GetTime(&t);

        switch ((module_rtc_notification_type_t)item.data)
        {
            case MODULE_RTC_NOTIFICATION_1_SEC:
                LOG_DEBUG("1 SEC  @ %04d-%02d-%02d %02d:%02d:%02d",
                    t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
                    t.tm_hour, t.tm_min, t.tm_sec);
                break;

            case MODULE_RTC_NOTIFICATION_1_MIN:
                LOG_DEBUG("1 MIN  @ %04d-%02d-%02d %02d:%02d:%02d",
                    t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
                    t.tm_hour, t.tm_min, t.tm_sec);
                break;

            case MODULE_RTC_NOTIFICATION_1_HOUR:
                LOG_DEBUG("1 HOUR @ %04d-%02d-%02d %02d:%02d:%02d",
                    t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
                    t.tm_hour, t.tm_min, t.tm_sec);
                break;

            default:
                LOG_WARNING("Unknown RTC notification : %d", item.data);
                break;
        }
    }
}

static void s_print_program_information(void)
{
    // Print Program Binary Information

    LOG_INFO("--------------------------------------------");
    LOG_INFO("Name : %s", PICO_PROGRAM_NAME);
    LOG_INFO("Description : %s", PICO_PROGRAM_DESCRIPTION);
    LOG_INFO("Version : %s", PICO_PROGRAM_VERSION_STRING);
    LOG_INFO("Compile Date Time : %s", COMPILE_DATE_TIME);
    LOG_INFO("\n");
    LOG_INFO("GIT Branch : %s", GIT_BRANCH);
    LOG_INFO("GIT Hash : %s", GIT_HASH);
    LOG_INFO("GIT Tag : %s", GIT_TAG);
    LOG_INFO("--------------------------------------------");
}
