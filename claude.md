# Project Overview
A E-ink panel table clock using Raspberry pi RP2350 microcontroller

## Build
cmake ..                                                                                                                                            # pre-compile
make -j8                                                                                                                                            # build
openocd -f interface/cmsis-dap.cfg -f target/rp2350.cfg -c "adapter speed 4000" -c "init; reset halt; flash erase_sector 0 0 last; reset run; exit" # Erase
openocd -f interface/cmsis-dap.cfg -f target/rp2350.cfg -c "adapter speed 4000" -c "program test_debug.bin 0x10000000 verify reset exit"            # Program
openocd -f interface/cmsis-dap.cfg -f target/rp2350.cfg -c "adapter speed 4000" -c "reset exit"                                                     # Reboot

## Project Structure
project/
- hardware/             # Hardware schematic and pcb 
- scripts/              # Helper scripts to compile, build, erase, program and restart the hardware target
- src/                  # Source code
    - common/           # Common source code for use by any project
        - bsp/          # Definitions of common hardware GPIO mappings
        - defines/      # Defintions of common data types and exten variables 
        - driver/       # Driver layer folder containing software modules interacting with hardware layer
        - module /      # Module layer folder containing business logic software modules making use of driver layer modules
        - util /        # Util folder containing utility modules
    - projects/         # Indvidual projects using the common source code & containing project specific code

## Conventions
- Naming
    - Lower case for variables
    - Static variable name starts with `s_`
    - Macros name starts with module name and an underscope and is in all upper case
    - Static function name starts with `s_`
    - All comments should be in TitleCase

- Memory
    - Caller frees what caller allocates

## Coding Style
- Max line length: 100 chars
- Use `stdint.h` types (`uint8_t`, `uint32_t`) over bare `int`
- Header guards over `#pragma once`
- No implicit fallthrough in switch — use `/* fallthrough */` comment
- Always add a default case in switch
- Function name always starts with name of the module, then `_` and then the function name in Camel case
- Function always starts with a comment outlining the purpose of the function along with it's arguments and returns
- H file has the following structure
    - Comment outlining the module name & date created (same as c file)
    - Header include guard (module name in all caps starting and ending with `-`)
    - Header includes
    - Defines
    - Function declerations
    - Each section should begin with a comment
- C file follows the following structure
    - Comment outlining the module name & date created
    - Header includes
    - Defines
    - Local/static variables
    - Local/static function declarations
    - External/public function definitions
    - Local/static function defintions
    - Each section should begin with a comment

