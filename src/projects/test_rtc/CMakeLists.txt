cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Set Board To Pico 2 W
set(PICO_BOARD pico2_w)

# Include Pico SDK Cmake
include(pico_sdk_import.cmake)

# Include FreeRTOS
include(./freertos_config/FreeRTOS_Kernel_import.cmake)

project(test_rtc C CXX ASM)

# Initialize the Raspberry Pi Pico SDK
# Pull in PICO SDK (must be before project)
pico_sdk_init()

add_executable(test_rtc
    main.c

    # Driver
    ../../common/driver/driver_rtc/driver_rtc.c
)

# Add Project Folders Include Path
target_include_directories(test_rtc PUBLIC ./)
target_include_directories(test_rtc PUBLIC ./freertos_config)
target_include_directories(test_rtc PUBLIC ../../common/driver/driver_rtc/include)
target_include_directories(test_rtc PUBLIC ../../common/util/util_logging/include)
target_include_directories(test_rtc PUBLIC ../../common/bsp/include)
target_include_directories(test_rtc PUBLIC ../../common/defines/include)

# pull in common dependencies
target_link_libraries(test_rtc pico_stdlib)
target_link_libraries(test_rtc pico_cyw43_arch_none)
target_link_libraries(test_rtc hardware_i2c)
target_link_libraries(test_rtc FreeRTOS-Kernel-Heap4)

# Enable USB Stdio
# Disable UART Stdio
pico_enable_stdio_usb(test_rtc 0)
pico_enable_stdio_uart(test_rtc 1)

# Binary Information
pico_set_program_name(test_rtc "test_rtc")
pico_set_program_description(test_rtc "test_rtc test project")
pico_set_program_version(test_rtc "0.1")

# create map/bin/hex file etc.
pico_add_extra_outputs(test_rtc)

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
