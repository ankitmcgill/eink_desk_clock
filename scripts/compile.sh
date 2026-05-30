# Compile
# 8/27/25

#!/bin/bash

if [ "$#" -eq 0 ]; then
    echo "Usage: $0 <string1> [string2 ... stringN] binary_name"
    echo "Strings:"
    echo "  cmake"
    echo "  make"
    echo "  info - UF2 Binary Info"
    echo "  bootsel - Put Pico Into Bootsel Mode"
    echo "  load - Flash Binary To Pico"
    echo "  reboot - Reboot Pico & Run Binary"
    echo "  erase - Erase Pico Flash"
    exit 1
fi

# Get The Path Where This Shell Script Is
SCRIPT_DIR="$(dirname "$(readlink -f "$0")")"

# Source Color Definitions
source $SCRIPT_DIR/colors.sh

# List Of Valid Arguments
VALID=("cmake" "make" "info" "bootsel" "load" "reboot" "erase")

# Iterate Over All Arguments Except The Last One
for arg in "${@:1:$#-1}"; do
    found=false
    for v in "${VALID[@]}"; do
        if [ "$arg" = "$v" ]; then
            found=true
            break
        fi
    done

    if $found; then
        printf "${LIGHT_YELLOW}--=== "$arg" ===--${ENDCOLOR}\n"
        if [ "$arg" = "cmake" ]; then
            cmake ..
        elif [ "$arg" = "make" ]; then
            make -j4
        elif [ "$arg" = "info" ]; then
            picotool info -a "${@: -1}"
        elif [ "$arg" = "bootsel" ]; then
            picotool reboot -u
        elif [ "$arg" = "load" ]; then
            openocd -f interface/cmsis-dap.cfg -f target/rp2350.cfg -c "adapter speed 4000" -c "program $2.bin 0x10000000 verify reset exit"
        elif [ "$arg" = "reboot" ]; then
            openocd -f interface/cmsis-dap.cfg -f target/rp2350.cfg -c "adapter speed 4000" -c "init; reset run; exit"
        elif [ "$arg" = "erase" ]; then
            openocd -f interface/cmsis-dap.cfg -f target/rp2350.cfg -c "adapter speed 4000" -c "init; reset halt; flash erase_sector 0 0 last; reset run; exit"
        fi

        # Exit If Command Failed
        if [ $? -ne 0 ]; then
            printf "${LIGHT_RED}Command Failed. Exiting!${ENDCOLOR}\n"
            exit 1
        fi
    else
        printf "${LIGHT_RED}Invalid Argument Found "$arg". Exiting!${ENDCOLOR}\n"
        exit 1
    fi
done
exit 0