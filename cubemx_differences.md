CubeMX2 repository layout differences
======================================

Starting with the STM32C5 family, STMicroelectronics restructured their
STM32Cube repositories. This document describes how the two layouts differ
and how each maps into this stm32lib repository.

Two scripts handle the import:

- `fetch_from_upstream.sh` -- original STM32Cube layout
- `fetch_from_cubemx2.sh` -- CubeMX2 layout

Repository structure
--------------------

### Original STM32Cube (e.g. STM32CubeWB)

A single repository contains everything under a `Drivers/` directory:

```
STM32CubeWB/
  Drivers/
    CMSIS/
      Device/ST/STM32WBxx/
        Include/            <-- device headers
        Source/
          Templates/        <-- system_stm32wbxx.c, startup .s files
    STM32WBxx_HAL_Driver/
      Inc/                  <-- HAL + LL headers, templates
      Src/                  <-- HAL + LL sources
  Middlewares/
  Projects/
  ...
```

### CubeMX2 (e.g. STM32CubeC5)

The monolithic `Drivers/` tree is gone. CMSIS device files and HAL/LL
drivers live in separate Git repositories, included as submodules:

```
STM32CubeC5/
  stm32c5xx_dfp/            <-- submodule: github.com/STMicroelectronics/stm32c5xx-dfp
    Include/                <-- device headers
    Source/
      startup_*.c           <-- startup files (now .c, not .s)
      Templates/            <-- system_stm32c5xx.c
  stm32c5xx_drivers/        <-- submodule: github.com/STMicroelectronics/stm32c5xx-drivers
    hal/                    <-- HAL .c and .h files mixed together
    ll/                     <-- LL headers only (inline implementations)
    templates/
      common/
        stm32_assert.h      <-- was stm32_assert_template.h
        stm32c5xx_hal_conf.h  <-- was stm32c5xx_hal_conf_template.h
        stm32_ll.h          <-- new, not present in original layout
  arch/
  middleware/
  ...
```

Note: submodule directory names use underscores (`stm32c5xx_dfp`) but the
Git repository names use hyphens (`stm32c5xx-dfp`).

Key differences
---------------

| Aspect | Original STM32Cube | CubeMX2 |
|--------|-------------------|---------|
| CMSIS device files | `Drivers/CMSIS/Device/ST/STM32WBxx/` | `stm32c5xx_dfp/` (separate repo) |
| HAL/LL drivers | `Drivers/STM32WBxx_HAL_Driver/` | `stm32c5xx_drivers/` (separate repo) |
| HAL headers vs sources | Separated into `Inc/` and `Src/` | Mixed together in `hal/` |
| LL drivers | Headers in `Inc/`, sources in `Src/` | Headers only in `ll/` (all inline) |
| Template headers | In `Inc/` with `_template` suffix | In `templates/common/` without suffix |
| Startup files | Assembly (`.s`) in `Source/Templates/` | C (`.c`) directly in `Source/` |
| `stm32_ll.h` | Not present | New file in `templates/common/` |

File mapping
------------

### CMSIS device files

| Source (upstream) | Destination (stm32lib) |
|-------------------|----------------------|
| **Original:** `Drivers/CMSIS/Device/ST/STM32WBxx/Include/` | `CMSIS/STM32WBxx/Include/` |
| **Original:** `Drivers/CMSIS/Device/ST/STM32WBxx/Source/` | `CMSIS/STM32WBxx/Source/` |
| **CubeMX2:** `stm32c5xx_dfp/Include/` | `CMSIS/STM32C5xx/Include/` |
| **CubeMX2:** `stm32c5xx_dfp/Source/` | `CMSIS/STM32C5xx/Source/` |

### HAL/LL drivers

| Source (upstream) | Destination (stm32lib) |
|-------------------|----------------------|
| **Original:** `Drivers/STM32WBxx_HAL_Driver/Inc/*.h` | `STM32WBxx_HAL_Driver/Inc/` |
| **Original:** `Drivers/STM32WBxx_HAL_Driver/Src/*.c` | `STM32WBxx_HAL_Driver/Src/` |
| **CubeMX2:** `stm32c5xx_drivers/hal/*.h` | `STM32C5xx_HAL_Driver/Inc/` |
| **CubeMX2:** `stm32c5xx_drivers/hal/*.c` | `STM32C5xx_HAL_Driver/Src/` |
| **CubeMX2:** `stm32c5xx_drivers/ll/*.h` | `STM32C5xx_HAL_Driver/Inc/` |

### Template headers

| Source (upstream) | Destination (stm32lib) |
|-------------------|----------------------|
| **Original:** `Inc/stm32_assert_template.h` | `Inc/stm32_assert_template.h` |
| **Original:** `Inc/stm32wbxx_hal_conf_template.h` | `Inc/stm32wbxx_hal_conf_template.h` |
| **CubeMX2:** `templates/common/stm32_assert.h` | `Inc/stm32_assert_template.h` (renamed) |
| **CubeMX2:** `templates/common/stm32c5xx_hal_conf.h` | `Inc/stm32c5xx_hal_conf_template.h` (renamed) |
| **CubeMX2:** `templates/common/stm32_ll.h` | `Inc/stm32_ll.h` (new) |
