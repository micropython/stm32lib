#!/bin/bash
#
# Fetch new code for an MCU series from the CubeMX2-era upstream STM repository.
#
# The CubeMX2 repos (starting with STM32C5) have a different layout:
#   - CMSIS device files are in the stm32<mcu>xx_dfp submodule
#   - HAL/LL drivers are in the stm32<mcu>xx_drivers submodule
#   - HAL .c and .h files are mixed together (no Inc/Src split)
#   - LL drivers are header-only in a separate ll/ directory

if [ $# -ne 1 ]; then
    echo "usage: $0 <mcu-suffix>"
    echo ""
    echo "eg: $0 C5"
    echo "Clones the DFP and drivers repos from STMicroelectronics and extracts CMSIS + HAL files"
    exit 1
fi

set -euo pipefail

suffix=$1
mcu=STM32${suffix}
mcu_lower=$(echo "stm32${suffix}" | tr '[:upper:]' '[:lower:]')

cmsis=CMSIS/${mcu}xx
hal=${mcu}xx_HAL_Driver

# Clone into a temporary directory, fetching only the two submodules we need.
tmpdir=$(mktemp -d)
trap "rm -rf $tmpdir" EXIT

dfp_repo="https://github.com/STMicroelectronics/${mcu_lower}xx-dfp.git"
drivers_repo="https://github.com/STMicroelectronics/${mcu_lower}xx-drivers.git"

echo "Cloning $dfp_repo into $tmpdir/dfp ..."
git clone --depth 1 "$dfp_repo" "$tmpdir/dfp"

echo "Cloning $drivers_repo into $tmpdir/drivers ..."
git clone --depth 1 "$drivers_repo" "$tmpdir/drivers"

dfp="$tmpdir/dfp"
drivers="$tmpdir/drivers"

if [ ! -d "$dfp/Include" ]; then
    echo "ERROR: DFP submodule not found at $dfp"
    echo "Check the repository layout matches the expected CubeMX2 structure."
    exit 1
fi

if [ ! -d "$drivers/hal" ]; then
    echo "ERROR: Drivers submodule not found at $drivers"
    echo "Check the repository layout matches the expected CubeMX2 structure."
    exit 1
fi

# Create target directories if they don't exist.
if [ ! -d "$cmsis" ]; then
    echo "WARNING: $cmsis is not an existing directory, assuming a new MCU series"
    mkdir -p "$cmsis"
fi

if [ ! -d "$hal" ]; then
    echo "WARNING: $hal is not an existing directory, assuming a new MCU series"
    mkdir -p "$hal"
fi

# CMSIS: remove any old files and copy across the new ones.
echo "Fetching CMSIS to $cmsis"
rm -rf "$cmsis/Include"
rm -rf "$cmsis/Source"
cp -r "$dfp/Include" "$cmsis/"
cp -r "$dfp/Source" "$cmsis/"
rm -rf "$cmsis"/Source/Templates/*/linker

# HAL: remove any old files and copy across the new ones.
# The new layout has .c and .h mixed in hal/, so we split them into Inc/ and Src/.
echo "Fetching HAL to $hal"
rm -rf "$hal/Inc"
rm -rf "$hal/Src"
mkdir -p "$hal/Inc"
mkdir -p "$hal/Src"

# Copy HAL headers and sources from the mixed hal/ directory.
find "$drivers/hal" -maxdepth 1 -name "*.h" -exec cp {} "$hal/Inc/" \;
find "$drivers/hal" -maxdepth 1 -name "*.c" -exec cp {} "$hal/Src/" \;

# Copy the Legacy directory if it exists (some families have it).
if [ -d "$drivers/hal/Legacy" ]; then
    cp -r "$drivers/hal/Legacy" "$hal/Inc/"
fi

# Copy LL headers (header-only, inline implementations).
find "$drivers/ll" -maxdepth 1 -name "*.h" -exec cp {} "$hal/Inc/" \;

# Copy template headers, renaming to match the convention used by other families.
if [ -d "$drivers/templates/common" ]; then
    for f in "$drivers/templates/common/"*.h; do
        base=$(basename "$f")
        case "$base" in
            stm32_assert.h)
                cp "$f" "$hal/Inc/stm32_assert_template.h" ;;
            ${mcu_lower}xx_hal_conf.h)
                cp "$f" "$hal/Inc/${mcu_lower}xx_hal_conf_template.h" ;;
            *)
                cp "$f" "$hal/Inc/$base" ;;
        esac
    done
fi

# Make sure tr and expand don't expect UTF-8, because these files aren't
export LC_CTYPE=C

for dir in $cmsis $hal; do
    # Process the new source code to:
    # - remove trailing white-space
    # - convert to unix line-endings
    # - expand tabs with 4 spaces
    # - convert non-ascii chars to ascii equivalent (should only be in comments)
    echo "Processing source code in $dir"
    for file in $(find "$dir" -name "*.[chs]"); do
        chmod 644 "$file"
        cat "$file" | awk "{sub(/[\t ]*$/,\"\")}1" | expand -t 4 | tr \\200\\205\\211\\221\\222\\223\\224\\226\\231\\244\\261\\265\\302\\327\\342 \'??\'\'\"\"\\-\'??u?x\' > "tmp$$"
        /bin/mv "tmp$$" "$file"
    done
done

echo ""
echo "Done. Files imported to:"
echo "  $cmsis/Include"
echo "  $cmsis/Source"
echo "  $hal/Inc"
echo "  $hal/Src"
