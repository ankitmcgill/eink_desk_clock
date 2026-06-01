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
    echo "  size - Compile Bin Size / Usage Report"
    exit 1
fi

# Get The Path Where This Shell Script Is
SCRIPT_DIR="$(dirname "$(readlink -f "$0")")"

# Source Color Definitions
source $SCRIPT_DIR/colors.sh

# List Of Valid Arguments
VALID=("cmake" "make" "info" "bootsel" "load" "reboot" "erase" "size")

# Find Name Of Elf File
ELF_FILE=$(ls *.elf | head -1) 

# Iterate Over All Arguments
for arg in "${@:1:$#0}"; do
    found=false
    for v in "${VALID[@]}"; do
        if [ "$arg" = "$v" ]; then
            found=true
            break
        fi
    done

    if $found; then
        printf "${LIGHT_YELLOW}--=== "$arg" ===--${ENDCOLOR}\n"
        printf "${LIGHT_GREEN}Found Elf File : $ELF_FILE${ENDCOLOR}\n"
        if [ "$arg" = "cmake" ]; then
            cmake ..
        elif [ "$arg" = "make" ]; then
            make -j4
        elif [ "$arg" = "info" ]; then
            picotool info -a "${@: -1}"
        elif [ "$arg" = "bootsel" ]; then
            picotool reboot -u
        elif [ "$arg" = "load" ]; then
            openocd -f interface/cmsis-dap.cfg -f target/rp2350.cfg -c "adapter speed 4000" -c "init; reset halt; program $ELF_FILE verify reset; reset; shutdown"
        elif [ "$arg" = "reboot" ]; then
            openocd -f interface/cmsis-dap.cfg -f target/rp2350.cfg -c "adapter speed 4000" -c "init; reset run; shutdown"
        elif [ "$arg" = "erase" ]; then
            openocd -f interface/cmsis-dap.cfg -f target/rp2350.cfg -c "adapter speed 4000" -c "init; reset halt; flash erase_sector 0 0 last; shutdown"
        elif [ "$arg" = "size" ]; then
            arm-none-eabi-size $ELF_FILE
            arm-none-eabi-size $ELF_FILE | awk -v fs=2097152 -v rs=532480 '
                NR == 2 {
                    flash = $1 + $2
                    ram   = $3 + $2
                    printf "Flash: %d / %d bytes (%.1f%%)\n", flash, fs, flash / fs * 100
                    printf "RAM:   %d / %d bytes (%.1f%%)\n", ram, rs, ram / rs * 100
                }'
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