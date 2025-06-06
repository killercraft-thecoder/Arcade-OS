#include "pxt.h"

namespace pxt {
static DevicePin **pinPtrs;         // Pointer to an array of DevicePin objects
static uint8_t numPinPtrs;          // Number of DevicePin objects
static uint8_t pinPos[DEV_NUM_PINS]; // Position map for pins

//%
DevicePin *getPin(int id) {
    // Ensure the pin ID is valid
    id &= CFG_PIN_NAME_MSK;

    if (id >= DEV_NUM_PINS)
        soft_panic(PANIC_NO_SUCH_PIN);

    int ptr = pinPos[id];
    if (ptr == 0) {
        // Allocate memory for a new pin
        pinPtrs = (DevicePin **)realloc(pinPtrs, (numPinPtrs + 1) * sizeof(void *));
        bool isAnalog = IS_ANALOG_PIN(id); // Check if the pin supports analog
        pinPtrs[numPinPtrs++] =
            new DevicePin(DEVICE_ID_IO_P0 + id, (PinName)id,
                          isAnalog ? PIN_CAPABILITY_AD : PIN_CAPABILITY_DIGITAL);
        ptr = numPinPtrs;
        pinPos[id] = ptr;
    }
    return pinPtrs[ptr - 1];
}

//%
DevicePin *getPinCfg(int key) {
    int p = getConfig(key, -1);
    if (p == -1)
        DMESG("No pin configuration for key: %d", key);
    return getPin(p);
}

// Links one pin to another
void linkPin(int from, int to) {
    if (from < 0 || from >= DEV_NUM_PINS)
        soft_panic(PANIC_NO_SUCH_PIN);
    getPin(to); // Ensure the 'to' pin exists
    pinPos[from] = pinPos[to]; // Link positions
}

//%
DevicePin *lookupPin(int pinName) {
    if (pinName < 0 || pinName == 0xff) // Invalid pin
        return NULL;
    pinName &= CFG_PIN_NAME_MSK;
    return getPin(pinName);
}

//%
DevicePin *lookupPinCfg(int key) {
    return lookupPin(getConfig(key));
}

// Looks up a CodalComponent by ID
CodalComponent *lookupComponent(int id) {
    for (int i = 0; i < DEVICE_COMPONENT_COUNT; ++i) {
        if (CodalComponent::components[i] && CodalComponent::components[i]->id == id)
            return CodalComponent::components[i];
    }
    return NULL;
}

} // namespace pxt

namespace pins {
/**
 * Get a pin by configuration ID (e.g., DAL.CFG_PIN...)
 */
//%
DigitalInOutPin pinByCfg(int key) {
    return pxt::lookupPinCfg(key);
}

/**
 * Create a new zero-initialized buffer.
 * @param size Number of bytes in the buffer
 */
//%
Buffer createBuffer(int size) {
    return mkBuffer(NULL, size);
}

/**
 * Get the duration of the last pulse in microseconds.
 * This should be used in an `onPulsed` handler.
 */
//% help=pins/pulse-duration blockGap=8
//% blockId=pins_pulse_duration block="pulse duration (µs)"
//% weight=19
int pulseDuration() {
    return pxt::lastEvent.timestamp; // Return timestamp of the last event
}
} // namespace pins
