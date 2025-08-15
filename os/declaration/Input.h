#ifndef INPUT_H
#define INPUT_H

#include <cstdint> // Ensures uint8_t is defined
#include "configkeys.h"
#include "pxt.h"
#include "Event.h"      // For emitting and handling events
#include "EventModel.h" // For registering listeners
#include "Timer.h"      // For scheduling hold/repeat events

// **High-Level Button Definitions**
// These constants represent button IDs used for input detection.
#define BTN_A 1          // A button
#define BTN_B 2          // B button
#define BTN_UP 3         // Navigation: Up
#define BTN_DOWN 4       // Navigation: Down
#define BTN_LEFT 5       // Navigation: Left
#define BTN_RIGHT 6      // Navigation: Right
#define BTN_MENU 7       // Menu Button
#define BTN_SOFT_RESET 8 // Reset Button
#define BTN_NULL_ID 0    // the ID for a NULL Button

// Declare external button instances
JADAC_Button A = JADAC_Button(BTN_A);     // Button A
JADAC_Button B = JADAC_Button(BTN_B);     // Button B
JADAC_Button UP = JADAC_Button(BTN_UP);    // Button UP
JADAC_Button DOWN = JADAC_Button(BTN_DOWN);  // Button DOWN
JADAC_Button LEFT = JADAC_Button(BTN_LEFT);  // Button LEFT
JADAC_Button RIGHT = JADAC_Button(BTN_RIGHT); // Button RIGHT
static uint16_t buttonsPressed = 0;

inline void onButtonDown(Event e)
{
    int buttonId = e.value;
    buttonsPressed |= (1 << buttonId);
}

inline void onButtonUp(Event e)
{
    int buttonId = e.value;
    buttonsPressed &= ~(1 << buttonId);
}

inline void initButtonListeners()
{
    registerWithEventBus(PXT_INTERNAL_KEY_DOWN, DEVICE_EVENT_ANY, onButtonDown);
    registerWithEventBus(PXT_INTERNAL_KEY_UP, DEVICE_EVENT_ANY, onButtonUp);
}

// Button Class for Input Handling
//  This class represents a physical button, storing its hardware ID and
//  providing an interface for checking its state.
class JADAC_Button
{
public:
    uint8_t Hardware_ID; // the ID of the PIN the Pysical Button is connected to

    /**
     * @brief Constructs a Button object, mapping logical IDs to hardware PINs.
     * @param Hardware_ID The logical ID (`BTN_*`) or direct PIN number.
     *
     * If a predefined button ID is passed (e.g., `BTN_A`), the constructor maps it
     * to the actual PIN (e.g., `PIN_BTN_A`). If an unknown ID is provided, it remains unchanged.
     */

    Button(uint8_t Hardware_ID) : Hardware_ID(Hardware_ID)
    {
        switch (id)
        {
        case BTN_A:
            Hardware_ID = CFG_PIN_BTN_A;
            break;
        case BTN_B:
            Hardware_ID = CFG_PIN_BTN_B;
            break;
        case BTN_UP:
            Hardware_ID = CFG_PIN_BTN_UP;
            break;
        case BTN_DOWN:
            Hardware_ID = CFG_PIN_BTN_DOWN;
            break;
        case BTN_LEFT:
            Hardware_ID = CFG_PIN_BTN_LEFT;
            break;
        case BTN_RIGHT:
            Hardware_ID = CFG_PIN_BTN_RIGHT;
            break;
        case BTN_MENU:
            Hardware_ID = CFG_PIN_BTN_MENU;
            break;
        case BTN_SOFT_RESET:
            Hardware_ID = PIN_BTN_SOFT_RESET;
            break;
        default:
            Hardware_ID = id;
            break;
        }
    }

    /**
     * @brief Checks if the button is currently pressed.
     * @return `true` if the button is being pressed, otherwise `false`.
     */
    bool isPressed()
    {
        return (buttonsPressed & (1 << Hardware_ID)) != 0;
    };
};

/**
 * @brief Retrieves a pointer to a dynamically allocated `Button` object based on its hardware ID.
 * @param id The unique identifier corresponding to the button.
 * @return A pointer to a `Button` instance representing the requested button.
 *         Caller is responsible for deleting the returned pointer.
 */
JADAC_Button *GetButtonFromID(uint8_t id)
{
    return new Button(id)
}

#endif /* INPUT_H */