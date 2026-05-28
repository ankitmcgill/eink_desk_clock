// DEFINE RTOS TASKS
// SEPTEMBER 9, 2025

#ifndef _DEFINE_RTOS_TASKS_
#define _DEFINE_RTOS_TASKS_

#include "FreeRTOS.h"
#include "task.h"

// Task Priority
#define TASK_PRIORITY_MAIN           (1)

// Task Stack Depth
#define TASK_STACK_DEPTH_MAIN        (512)

// Task Debug Tag
#define DEBUG_TAG_MAIN              ("Main")

// Task Handles
extern TaskHandle_t handle_task_main;

#endif