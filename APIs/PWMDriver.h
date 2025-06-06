#ifndef PWMDRIVER_H
#define PWMDRIVER_H

#include <cstdint>

namespace PWM {
    /**
     * Initialize a PWM channel on a given pin.
     * @param pin The pin number to use for PWM output.
     * @param frequency The frequency in Hz for the PWM signal.
     * @param dutyCycle The initial duty cycle (0-100).
     * @return true if initialization succeeded, false otherwise.
     */
    bool init(uint32_t pin, uint32_t frequency, uint8_t dutyCycle);

    /**
     * Set the duty cycle for a PWM channel.
     * @param pin The pin number for the PWM output.
     * @param dutyCycle The new duty cycle (0-100).
     */
    void setDutyCycle(uint32_t pin, uint8_t dutyCycle);

    /**
     * Set the frequency for a PWM channel.
     * @param pin The pin number for the PWM output.
     * @param frequency The new frequency in Hz.
     */
    void setFrequency(uint32_t pin, uint32_t frequency);

    /**
     * Stop PWM output on a given pin.
     * @param pin The pin number to stop PWM on.
     */
    void stop(uint32_t pin);
}

#endif
