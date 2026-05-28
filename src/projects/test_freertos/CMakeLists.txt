cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Set Board To Pico 2 W
set(PICO_BOARD pico2_w)

# Include Pico SDK Cmake
include(pico_sdk_import.cmake)

# Include FreeRTOS
include(./freertos_config/FreeRTOS_Kernel_import.cmake)

project(test_freertos C CXX ASM)

# Initialize the Raspberry Pi Pico SDK
# Pull in PICO SDK (must be before project)
pico_sdk_init()

add_executable(test_freertos
    main.c

    # Source -> Common
    ../../common/util/util_dataqueue/util_dataqueue.c
    )

# Add Project Folders Include Path
target_include_directories(test_freertos PUBLIC ./)
target_include_directories(test_freertos PUBLIC ./freertos_config)
target_include_directories(test_freertos PUBLIC ../../common/others/bsp/include)
target_include_directories(test_freertos PUBLIC ../../common/others/defines/include)
target_include_directories(test_freertos PUBLIC ../../common/util/util_dataqueue/include)

# pull in common dependencies
target_link_libraries(test_freertos pico_stdlib)
target_link_libraries(test_freertos pico_cyw43_arch_none)
target_link_libraries(test_freertos FreeRTOS-Kernel-Heap4)

# Enable USB Stdio
# Disable UART Stdio
pico_enable_stdio_usb(test_freertos 0)
pico_enable_stdio_uart(test_freertos 1)

# Binary Information
pico_set_program_name(test_freertos "Test_freertos")
pico_set_program_description(test_freertos "test_freertos test project")
pico_set_program_version(test_freertos "0.1")

# create map/bin/hex file etc.
pico_add_extra_outputs(test_freertos)

# Add Compile Time Definitions For Program Metadata
# Compile Date Time
execute_process(
    COMMAND
        bash -c "date"
    OUTPUT_VARIABLE
        COMPILE_DATE_TIME
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
# Git Branch & Hash
execute_process(
    COMMAND
        bash -c "git rev-parse --abbrev-ref HEAD"
    OUTPUT_VARIABLE
        GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND
        bash -c "git rev-parse --short HEAD"
    OUTPUT_VARIABLE
        GIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND
        bash -c "git tag --points-at HEAD"
    OUTPUT_VARIABLE
        GIT_TAG
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
add_definitions(-DCOMPILE_DATE_TIME="${COMPILE_DATE_TIME}" -DGIT_BRANCH="${GIT_BRANCH}" -DGIT_HASH="${GIT_HASH}" -DGIT_TAG="${GIT_TAG}")