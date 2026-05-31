#include <stdio.h>
#include <string.h>
#include <time.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/binary_info.h"
#include "FreeRTOS.h"
#include "task.h"

#include "driver_rtc.h"
#include "util_logging.h"
#include "define_rtos_tasks.h"
#include "bsp.h"

// Defines

// Internal Variables
TaskHandle_t handle_task_main;

// Internal Functions
static void s_task_fn(void *pvParameters);
static void s_print_program_information(void);

int main(void)
{
    struct tm t;

    // Initialize All Enabled Stdio Backends
    stdio_init_all();

    // Initialize Wifi Chip Architecture
    cyw43_arch_init();

    LOG_INFO("Starting Main ...");
    s_print_program_information();

    // Initializing Software Modules
    DRIVER_RTC_Init(); // RTC

    // Set RTC Time To Baseline 2000-01-01 00:00:00
    memset(&t, 0, sizeof(t));
    t.tm_mday = 1;    // Day Of Month 1-31
    t.tm_year = 100;  // Years Since 1900 (2000)
    DRIVER_RTC_SetTime(&t);

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
    struct tm t;

    while(true)
    {
        // Read And Print The Current RTC Time
        DRIVER_RTC_GetTime(&t);
        LOG_INFO("RTC Time : %04d-%02d-%02d %02d:%02d:%02d",
            t.tm_year + 1900,
            t.tm_mon + 1,
            t.tm_mday,
            t.tm_hour,
            t.tm_min,
            t.tm_sec
        );

        /* Delay for 1000 ms */
        vTaskDelay(pdMS_TO_TICKS(1000));
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
