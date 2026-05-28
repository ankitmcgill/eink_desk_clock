#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/binary_info.h"
#include "FreeRTOS.h"
#include "task.h"

#include "util_dataqueue.h"
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
    // Initialize All Enabled Stdio Backends
    stdio_init_all();

    // Initialize Wifi Chip Architecture
    cyw43_arch_init();

    printf("Starting main ...\n");
    s_print_program_information();

    printf("Starting main task ...\n");
    
    /* Create task */
    xTaskCreate(
        s_task_fn,              // Task function
        "DebugTask",            // Task name
        TASK_STACK_DEPTH_MAIN,  // Stack size
        NULL,                   // Task parameter
        TASK_PRIORITY_MAIN,     // Task priority
        &handle_task_main        // Task handle
    );

    /* Start scheduler */
    vTaskStartScheduler();

    /* Infinite Loop */
    while(true){
        // Do Nothing
    }
}

static void s_task_fn(void *pvParameters)
{
    static bool led_state = true;

    led_state = !led_state;
    while(true)
    {
        printf("Task Running ...\n");
        cyw43_arch_gpio_put(BSP_LED_GPIO, led_state);

        /* Delay for 1000 ms */
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void s_print_program_information(void)
{
    // Print Program Binary Information

    printf("--------------------------------------------\n");
    printf("Name : %s\n", PICO_PROGRAM_NAME);
    printf("Description : %s\n", PICO_PROGRAM_DESCRIPTION);
    printf("Version : %s\n", PICO_PROGRAM_VERSION_STRING);
    printf("Compile Date Time : %s\n", COMPILE_DATE_TIME);
    printf("\n");
    printf("GIT Branch : %s\n", GIT_BRANCH);
    printf("GIT Hash : %s\n", GIT_HASH);
    printf("GIT Tag : %s\n", GIT_TAG);
    printf("--------------------------------------------\n");
    printf("\n");
}