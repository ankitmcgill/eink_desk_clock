# Get Scripts
# Add Scripts Folder To System Path
# 9/25/25

#!/bin/bash

# To Execute Use "source ../../../../scripts/get-scripts.sh"
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
export PATH="$PATH:$SCRIPT_DIR"
