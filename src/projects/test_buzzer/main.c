#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/binary_info.h"
#include "FreeRTOS.h"
#include "task.h"

#include "driver_gpio.h"
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

    printf("Starting Main ...\n");
    s_print_program_information();

    // Initializing Software Modules

    // Set Buzzer pin
    DRIVER_GPIO_Setup(BSP_BUZZER_GPIO, DRIVER_GPIO_PIN_DIR_OUT);

    printf("Starting Main task ...\n");
    
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
}

static void s_task_fn(void *pvParameters)
{
    static bool buzzer_state = true;

    while(true)
    {
        printf("Task Running ...\n");

        buzzer_state = !buzzer_state;
        DRIVER_GPIO_Toggle(BSP_BUZZER_GPIO);

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