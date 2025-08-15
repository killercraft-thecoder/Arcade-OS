#ifndef __PXT_PINS_H
#define __PXT_PINS_H

// Button configuration macros
#define BUTTON_ACTIVE_HIGH_PULL_DOWN (ACTIVE_HIGH | 0x10)
#define BUTTON_ACTIVE_HIGH_PULL_UP (ACTIVE_HIGH | 0x20)
#define BUTTON_ACTIVE_HIGH_PULL_NONE (ACTIVE_HIGH | 0x30)
#define BUTTON_ACTIVE_LOW_PULL_DOWN (ACTIVE_LOW | 0x10)
#define BUTTON_ACTIVE_LOW_PULL_UP (ACTIVE_LOW | 0x20)
#define BUTTON_ACTIVE_LOW_PULL_NONE (ACTIVE_LOW | 0x30)

// Pin definition macros
#define PIN(name) ((PinName)pxt::getConfig(CFG_PIN_##name, -1))
#define LOOKUP_PIN(name) pxt::lookupPin(PIN(name))

// Default Codal peripheral definitions, can be overridden in platform.h
#ifndef CODAL_PIN
#define CODAL_PIN CODAL_MBED::Pin // Default to Mbed-based Pin
#endif

#ifndef CODAL_TIMER
#define CODAL_TIMER CODAL_MBED::Timer // Default to Mbed-based Timer
#endif

#ifndef CODAL_SPI
#define CODAL_SPI CODAL_MBED::SPI // Default to Mbed-based SPI
#endif

#ifndef CODAL_SERIAL
#define CODAL_SERIAL CODAL_MBED::Serial // Default to Mbed-based Serial
#endif

// Check if a pin is analog-compatible, based on DEV_ANALOG_PINS
#ifndef IS_ANALOG_PIN
#define IS_ANALOG_PIN(id) ((DEV_ANALOG_PINS >> (id)) & 1)
#endif

// Define device pin types
typedef CODAL_PIN DevicePin;

typedef DevicePin *DigitalInOutPin; // For general digital input/output pins
typedef DevicePin *AnalogInOutPin;  // For pins with both analog input/output capabilities
typedef DevicePin *AnalogInPin;     // For pins with analog input capabilities only
typedef DevicePin *AnalogOutPin;    // For pins with analog output capabilities only
typedef DevicePin *PwmPin;          // For PWM-enabled pins
typedef DevicePin *PwmOnlyPin;      // For pins restricted to PWM-only usage
typedef Button *Button_;            // For representing buttons

namespace pxt
{
    // Functions for accessing and configuring pins
    DevicePin *getPin(int id);
    DevicePin *getPinCfg(int key);
    DevicePin *lookupPin(int pinName);
    DevicePin *lookupPinCfg(int key);
    void linkPin(int from, int to);
    CodalComponent *lookupComponent(int id);
}

// Macro for pin operations
#define PINOP(op) name->op

#endif
