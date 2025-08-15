#include "pxt.h"

namespace AnalogInPinMethods {

/**
 * Read the connector value as analog, that is, as a value comprised between 0 and 1023.
 * @param name pin to write to
 */
int analogRead(AnalogInPin name) {
    return PINOP(getAnalogValue());
}
}

namespace AnalogOutPinMethods {
void analogWrite(AnalogOutPin name, int value) __attribute__ ((weak));

/**
 * Set the connector value as analog. Value must be comprised between 0 and 1023.
 * @param name pin name to write to
 * @param value value to write to the pin between ``0`` and ``1023``. eg:1023,0
 */
void analogWrite(AnalogOutPin name, int value) {
    PINOP(setAnalogValue(value));
}
}