// cmsis.h — custom wrapper for CMSIS headers.
#ifndef CMSIS_H
#define CMSIS_H

// Cortex-M4 core peripherals and intrinsic macros
#include "core_cm4.h"

// Compiler abstraction macros (like __STATIC_INLINE)
#include "cmsis_compiler.h"

// GCC-specific attribute and intrinsic definitions
#include "cmsis_gcc.h"

// You can add more includes here if needed:
// #include "system_stm32f4xx.h"  // Optional system config
#include "stm32f4xx.h"         // Device-specific defines

#endif // CMSIS_H