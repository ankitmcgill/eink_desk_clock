cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Set Board To Pico 2 W
set(PICO_BOARD pico2_w)

# Include Pico SDK Cmake
include(../pico_sdk_import.cmake)

project(swd C CXX ASM)

# Initialize the Raspberry Pi Pico SDK
# Pull in PICO SDK (must be before project)
pico_sdk_init()

add_executable(swd
    main.c
    )

# pull in common dependencies
target_link_libraries(swd pico_stdlib)
target_link_libraries(swd pico_cyw43_arch_none)

# create map/bin/hex file etc.
pico_add_extra_outputs(swd)
