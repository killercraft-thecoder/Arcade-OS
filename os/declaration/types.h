// types.h - Defines custom types for the OS
#ifndef TYPES_H
#define TYPES_H

#include <cstdint>
#include <string>

typedef uint32_t uint;      // 32-bit unsigned integer
typedef std::string string; // Standard string type
typedef int64_t lint;       // 64-bit signed integer
typedef uint64_t ulint;     // 64-bit unsigned integer
typedef uint8_t byte;       // 8-bit unsigned integer
typedef int32_t number;     // A number supported by the STM32F4 at hardware level
typedef float nfloat;       // A float supported by the STM32F4's FPU

#endif // TYPES_H