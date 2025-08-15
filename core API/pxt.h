#ifndef __PXT_H
#define __PXT_H

#include "pxtbase.h"

#include "CodalConfig.h"
#include "CodalHeapAllocator.h"
#include "CodalDevice.h"
#include "CodalDmesg.h"
#include "ErrorNo.h"
#include "Timer.h"
#include "Matrix4.h"
#include "CodalCompat.h"
#include "CodalComponent.h"
#include "ManagedType.h"
#include "Event.h"
#include "NotifyEvents.h"
#include "Button.h"
#include "CodalFiber.h"
#include "MessageBus.h"
#include "MultiButton.h"

using namespace codal;

// codal::ManagedString compatibility macros
#define MSTR(s) codal::ManagedString((s)->data, (s)->length)
#define PSTR(s) mkString((s).toCharArray(), (s).length())

#include "pins.h"

// i COmmented Out Parts that Aint exist,  maybe the JOYSTICK exists? , or not.
#if CONFIG_ENABLED(DEVICE_USB)
#include "hf2.h"
#include "hf2dbg.h"
#if CONFIG_ENABLED(DEVICE_MOUSE)
#include "HIDMouse.h"
#endif
#if CONFIG_ENABLED(DEVICE_KEYBOARD)
#include "HIDKeyboard.h"
#endif
#if CONFIG_ENABLED(DEVICE_JOYSTICK)
#include "HIDJoystick.h"
#endif
#endif

#define PXT_COMM_BASE 0x20002000 // Adjusted for 8k on F4 architecture

// Backward compatibility with older Codal definitions
#ifndef REAL_TIME_FUNC
#define REAL_TIME_FUNC /* */
#endif

namespace pxt
{

#if CONFIG_ENABLED(DEVICE_USB)
    extern CodalUSB usb; // USB interface
    extern HF2 hf2;      // HF2 flashing and communication
#if CONFIG_ENABLED(DEVICE_MOUSE)
    extern USBHIDMouse mouse; // USB mouse
#endif
#if CONFIG_ENABLED(DEVICE_KEYBOARD)
    extern USBHIDKeyboard keyboard; // USB keyboard
#endif
#if CONFIG_ENABLED(DEVICE_JOYSTICK)
    extern USBHIDJoystick joystick; // USB joystick
#endif
#endif

    // Core utility functions and hardware drivers
    extern Event lastEvent;           // Tracks the last system event
    extern CODAL_TIMER devTimer;      // Core timer for task scheduling
    extern MessageBus devMessageBus;  // Message bus for events
    extern codal::CodalDevice device; // Core Codal device interface

    void set_usb_strings(const char *uf2_info);      // Set USB identification strings
    extern void (*logJDFrame)(const uint8_t *data);  // Log JacDac frames
    extern void (*sendJDFrame)(const uint8_t *data); // Send JacDac frames

    static inline void raiseEvent(int src, int val)
    {
        Event(src, val);
    }

} // namespace pxt

namespace pins
{
    class CodalSPIProxy; // Proxy for SPI
    class CodalI2CProxy; // Proxy for I2C
} // namespace pins

typedef pins::CodalI2CProxy *I2C_; // Type alias for I2C
typedef pins::CodalSPIProxy *SPI_; // Type alias for SPI

namespace pxt
{
    codal::LowLevelTimer *allocateTimer(); // Allocate a low-level timer

#ifdef CODAL_I2C
    CODAL_I2C *getI2C(DigitalInOutPin sda, DigitalInOutPin scl); // Get I2C instance
#endif
    CODAL_SPI *getSPI(DigitalInOutPin mosi, DigitalInOutPin miso, DigitalInOutPin sck); // Get SPI instance
#ifdef CODAL_JACDAC_WIRE_SERIAL
    LowLevelTimer *getJACDACTimer(); // JacDac timer
#endif
    class PressureButton;                     // Custom pressure button class
    uint32_t readButtonMultiplexer(int bits); // Read from button multiplexer
    void disableButtonMultiplexer();          // Disable button multiplexer
}

namespace serial
{
    class CodalSerialDeviceProxy; // Proxy for serial devices
}

typedef serial::CodalSerialDeviceProxy *SerialDevice; // Type alias for SerialDevice

namespace jacdac
{
    class JDProxyDriver; // JacDac driver proxy
} // namespace jacdac

typedef jacdac::JDProxyDriver *JacDacDriverStatus; // Type alias for JacDac driver status

// Device-specific definitions
#define DEVICE_ID_BUTTON_SLIDE 3000
#define DEVICE_ID_MICROPHONE 3001
#define DEVICE_ID_FIRST_BUTTON 4000
#define DEVICE_ID_FIRST_TOUCHBUTTON 4100

// Internal event IDs
#define PXT_INTERNAL_KEY_UP 2050
#define PXT_INTERNAL_KEY_DOWN 2051

#endif
