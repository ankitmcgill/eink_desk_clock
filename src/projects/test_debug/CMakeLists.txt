cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Set Board To Pico 2 W
set(PICO_BOARD pico2_w)

# Include Pico SDK Cmake
include(../pico_sdk_import.cmake)

project(test_debug C CXX ASM)

# Initialize the Raspberry Pi Pico SDK
# Pull in PICO SDK (must be before project)
pico_sdk_init()

add_executable(test_debug
    main.c
    )

# pull in common dependencies
target_link_libraries(test_debug pico_stdlib)
target_link_libraries(test_debug pico_cyw43_arch_none)

# Enable Semihostin Stdio
pico_enable_stdio_usb(test_debug 1)
pico_enable_stdio_uart(test_debug 0)

# Binary Information
pico_set_program_name(test_debug "Test_debug")
pico_set_program_description(test_debug "Test_debug test project")
pico_set_program_version(test_debug "0.1")

# create map/bin/hex file etc.
pico_add_extra_outputs(test_debug)