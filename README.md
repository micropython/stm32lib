STM32 Cube library - CMSIS and HAL for F0, F4, F7, G0, G4, H5, H7, L0, L1, L4, N6, U5, WB and WL microcontrollers
=================================================================================================================

This repository contains a copy of STMicroelectronic's STM32 Cube firmware
library, with CMSIS-device and HAL code for STM32F0xx, STM32F4xx, STM32F7xx,
STM32G0xx, STM32G4xx, STM32H5xx, STM32H7xx, STM32L0xx, STM32L1xx, STM32L4xx,
STM32N6xx, STM32U5xx, STM32WBxx and STM32WLxx microcontrollers.  Minor changes
are applied to the HAL source to support MicroPython's port to the STM32.

The "vendor" branch contains the original sources and is updated from
time-to-time when new versions become available.  The upstream sources are
provided as separate releases (and in separate archives) for each
microcontroller class (eg F4, F7, L4).  These are merged together here in
the "vendor" branch, because the idea of the HAL code is that it provides a
uniform interface to all microcontroller classes.  The commits in the "vendor"
branch are tagged as appropriate with a tag that describes all the available
versions at that commit, for example F4-1.5.0+F7-1.1.0+L4-1.3.0.

There are then working branches that branch at a given vendor tag and apply
minor patches to the vendor code, for example work-F4-1.5.0+F7-1.1.0+L4-1.3.0.
The patches/commits that form a given working branch are reapplied (with
conflict resolution) to newer vendor tags to create the next working branch.

Original sources
================

The sources are obtained from ``http://st.com``, and also from the git
repositories found at ``https://github.com/STMicroelectronics/``.

For all .c and .h files the following processing has been done to the original
files before committing them here:
- lines endings changed to unix style
- trailing spaces removed
- tabs expanded to 4 spaces
- non-ASCII chars converted to their ASCII equivalent

Directories from the original sources are mapped into this repository according
to the following:
```
Drivers/STM32F4xx_HAL_Driver/Inc ->          STM32F4xx_HAL_Driver/Inc
Drivers/STM32F4xx_HAL_Driver/Src ->          STM32F4xx_HAL_Driver/Src
Drivers/CMSIS/Device/ST/STM32F4xx/Include -> CMSIS/STM32F4xx/Include
Drivers/CMSIS/Device/ST/STM32F4xx/Source  -> CMSIS/STM32F4xx/Source
```
And similar for the other microcontroller classes.

The included `fetch_from_upstream.sh` script can automatically copy and process
new source code from an STM git repository.

Updating to a new vendor version
=================================

To update a specific MCU series (e.g. STM32WB) to a new vendor version, follow
these steps:

1. **Clone or fetch the upstream STM32Cube repository**

   For example, to update STM32WB:
   ```bash
   cd /tmp
   git clone https://github.com/STMicroelectronics/STM32CubeWB.git
   cd STM32CubeWB
   git checkout v1.23.0  # or desired version
   ```

2. **Identify the current work branch before updating vendor**

   In stm32lib folder identify the current released tag:
   ```bash
   cd path/to/stm32lib
   git describe --tags --exact-match origin/vendor
   ```

   This shows the current version tag, for example:
   ```
   F0-1.9.0+F4-1.16.0+F7-1.7.0+G0-1.5.1+G4-1.3.0+H5-1.0.0+H7-1.11.0+L0-1.11.2+L1-1.10.3+L4-1.17.0+N6-1.1.0+WB-1.10.0+WL-1.1.0
   ```
   The convention is to use a similar tag name prepended with `work-` as the working branch when submitting an update.

   Now checkout vendor to begin the update:
   ```bash
   git checkout vendor
   ```

3. **Import and commit the vendor update**

   ```bash
   ./fetch_from_upstream.sh STM32WB /tmp/STM32CubeWB
   git add -A
   git commit -m "Import STM32CubeWB v1.23.0 on DD-Mon-YYYY."
   ```

   The `fetch_from_upstream.sh` script will copy and process the new CMSIS and HAL
   source files. The maintainer will create the version tag after merging.

4. **Create new work branch and rebase patches**

   Create a new work branch name by updating only the MCU series you're upgrading.
   For example, updating WB from 1.10.0 to 1.23.0:
   ```bash
   # Old: origin/work-...-WB-1.10.0-...
   # New: work-...-WB-1.23.0-...
   git checkout -b work-F0-1.9.0+F4-1.16.0+F7-1.7.0+G0-1.5.1+G4-1.3.0+H5-1.0.0+H7-1.11.0+L0-1.11.2+L1-1.10.3+L4-1.17.0+N6-1.1.0+WB-1.23.0+WL-1.1.0 \
       origin/work-F0-1.9.0+F4-1.16.0+F7-1.7.0+G0-1.5.1+G4-1.3.0+H5-1.0.0+H7-1.11.0+L0-1.11.2+L1-1.10.3+L4-1.17.0+N6-1.1.0+WB-1.10.0+WL-1.1.0
   ```

   Rebase all patches onto the updated vendor branch:
   ```bash
   git rebase vendor
   ```

   Resolve any conflicts if they arise. The rebase applies all MicroPython-specific
   patches from the previous work branch onto the new vendor code.

5. **Push and create pull request**

   Push only the work branch (not vendor - the vendor commit will be included in the PR):
   ```bash
   git push -u fork work-F0-1.9.0+...+WB-1.23.0+...
   gh pr create --repo micropython/stm32lib --base vendor \
                --title "stm32lib: Update STM32WB from v1.10.0 to v1.23.0."
   ```
