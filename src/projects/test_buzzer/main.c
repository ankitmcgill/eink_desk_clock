#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/binary_info.h"
#include "FreeRTOS.h"
#include "task.h"

#include "driver_gpio.h"
#include "driver_buzzer.h"
#include "util_logging.h"
#include "util_dataqueue.h"
#include "define_rtos_tasks.h"
#include "bsp.h"

// Defines

// Internal Variables
static uint8_t pwm_slice;
TaskHandle_t handle_task_main;

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

    // Initializing Software Modules

    // Set Buzzer pin
    pwm_slice = DRIVER_BUZZER_Init(BSP_BUZZER_GPIO, 2000, 20.0);

    LOG_INFO("Starting Main task ... slice");
    
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
    while(true)
    {
        LOG_INFO("Task Running ... Beep-Beep Alarm");

        DRIVER_BUZZER_On(pwm_slice);
        vTaskDelay(pdMS_TO_TICKS(100));
        DRIVER_BUZZER_Off(pwm_slice);
        vTaskDelay(pdMS_TO_TICKS(100));
        DRIVER_BUZZER_On(pwm_slice);
        vTaskDelay(pdMS_TO_TICKS(100));
        DRIVER_BUZZER_Off(pwm_slice);

        /* Delay for 1000 ms */
        vTaskDelay(pdMS_TO_TICKS(2000));
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