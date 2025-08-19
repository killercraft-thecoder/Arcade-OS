// types.h - Defines custom types for the OS
#ifndef TYPES_H
#define TYPES_H

#include <cstdint>
#include <string>
#include "platform_includes.h"

typedef uint32_t uint;              // 32-bit unsigned integer
typedef std::string string;         // Standard string type
typedef uint8_t byte;               // 8-bit unsigned integer
typedef PROCESSOR_WORD_TYPE number; // A number supported by the STM32F4 at hardware level
#ifdef FPU_32
typedef float nfloat; // A float supported by the MCU's FPU
#define __FLOAT
#endif
#ifdef FPU_64
typedef double nfloat; // A float supported by the MCU's FPU
#define __FLOAT
#endif
#ifndef __FLOAT
#define nfloat float // No FPU on MCU.
#endif
nfloat typedef int32_t i32;
typedef int16_t i16;
typedef int64_t i64;
typedef int8_t i8;
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

/**
 * @class FastDouble
 * @brief A lightweight alternative to double precision, using a float and an extra 32-bit field.
 */
class FastDouble
{
public:
    nfloat base;        /**< Primary float value */
    uint32_t extraBits; /**< Additional precision */

    /**
     * @brief Default constructor. Initializes to zero.
     */
    FastDouble() : base(0.0f), extraBits(0) {}

    /**
     * @brief Constructs a FastDouble from a float and optional extra bits.
     * @param b The base float value.
     * @param e The extra bits (default is 0).
     */
    FastDouble(nfloat b, uint32_t e = 0) : base(b), extraBits(e) {}

    /**
     * @brief Copy constructor.
     * @param other The FastDouble to copy.
     */
    FastDouble(const FastDouble &other) = default;

    /**
     * @brief Adds two FastDouble values.
     * @param rhs The right-hand operand.
     * @return A new FastDouble representing the sum.
     */
    FastDouble operator+(const FastDouble &rhs) const
    {
        return FastDouble(base + rhs.base, extraBits ^ rhs.extraBits);
    }

    /**
     * @brief Subtracts one FastDouble from another.
     * @param rhs The right-hand operand.
     * @return A new FastDouble representing the difference.
     */
    FastDouble operator-(const FastDouble &rhs) const
    {
        return FastDouble(base - rhs.base, extraBits ^ rhs.extraBits);
    }

    /**
     * @brief Multiplies two FastDouble values.
     * @param rhs The right-hand operand.
     * @return A new FastDouble representing the product.
     */
    FastDouble operator*(const FastDouble &rhs) const
    {
        return FastDouble(base * rhs.base, extraBits ^ rhs.extraBits);
    }

    /**
     * @brief Divides one FastDouble by another.
     * @param rhs The right-hand operand.
     * @return A new FastDouble representing the quotient.
     */
    FastDouble operator/(const FastDouble &rhs) const
    {
        return FastDouble(base / rhs.base, extraBits ^ rhs.extraBits);
    }

    /**
     * @brief Checks equality between two FastDouble values.
     * @param rhs The right-hand operand.
     * @return True if both base and extraBits match.
     */
    bool operator==(const FastDouble &rhs) const
    {
        return base == rhs.base && extraBits == rhs.extraBits;
    }

    /**
     * @brief Checks inequality between two FastDouble values.
     * @param rhs The right-hand operand.
     * @return True if either base or extraBits differ.
     */
    bool operator!=(const FastDouble &rhs) const
    {
        return !(*this == rhs);
    }

    /**
     * @brief Compares two FastDouble values for less-than.
     * @param rhs The right-hand operand.
     * @return True if this is less than rhs.
     */
    bool operator<(const FastDouble &rhs) const
    {
        return base < rhs.base || (base == rhs.base && extraBits < rhs.extraBits);
    }

    /**
     * @brief Compares two FastDouble values for greater-than.
     * @param rhs The right-hand operand.
     * @return True if this is greater than rhs.
     */
    bool operator>(const FastDouble &rhs) const
    {
        return rhs < *this;
    }

    /**
     * @brief Converts FastDouble to a standard double.
     * @return A double approximation of the FastDouble.
     */
    double toDouble() const
    {
        return static_cast<double>(base) + static_cast<double>(extraBits) / UINT32_MAX;
    }

    /**
     * @brief Converts FastDouble to a string representation.
     * @return A string showing base and extraBits as a combined numeric value.
     */
    std::string toString() const
    {
        char buffer[48];
        snprintf(buffer, sizeof(buffer), "%.6f%u", base, extraBits);
        return std::string(buffer);
    }
};

#endif // TYPES_H