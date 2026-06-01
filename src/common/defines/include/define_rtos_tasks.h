// DEFINE RTOS TASKS
// SEPTEMBER 9, 2025

#ifndef _DEFINE_RTOS_TASKS_
#define _DEFINE_RTOS_TASKS_

#include "FreeRTOS.h"
#include "task.h"

// Task Priority
#define TASK_PRIORITY_MAIN           (1)
#define TASK_PRIORITY_RTC            (2)

// Task Stack Depth
#define TASK_STACK_DEPTH_MAIN        (512)
#define TASK_STACK_DEPTH_RTC         (512)

// Task Debug Tag
#define DEBUG_TAG_MAIN              ("Main")
#define DEBUG_TAG_RTC               ("Rtc")

// Task Handles
extern TaskHandle_t handle_task_main;
extern TaskHandle_t handle_task_rtc;

#endif