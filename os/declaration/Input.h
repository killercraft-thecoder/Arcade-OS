#ifndef I_H
#define I_H

#include <cstdint> // Ensures uint8_t is defined
#include "configkeys.h"
// 🎮 **High-Level Button Definitions**
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
extern Button A;  // Button A
extern Button B;  // Button B
extern Button UP; // Button UP
extern Button DOWN; // Button DOWN
extern Button LEFT; // Button LEFT
extern Button RIGHT; // Button RIGHT

// Button Class for Input Handling
//  This class represents a physical button, storing its hardware ID and
//  providing an interface for checking its state.
class Button
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
        switch (Hardware_ID)
        {
        case BTN_A:
            Hardware_ID = PIN_BTN_A;
            break;
        case BTN_B:
            Hardware_ID = PIN_BTN_B;
            break;
        case BTN_UP:
            Hardware_ID = PIN_BTN_UP;
            break;
        case BTN_DOWN:
            Hardware_ID = PIN_BTN_DOWN;
            break;
        case BTN_LEFT:
            Hardware_ID = PIN_BTN_LEFT;
            break;
        case BTN_RIGHT:
            Hardware_ID = PIN_BTN_RIGHT;
            break;
        case BTN_MENU:
            Hardware_ID = PIN_BTN_MENU;
            break;
        case BTN_SOFT_RESET:
            Hardware_ID = PIN_BTN_SOFT_RESET;
            break;
        }
    }

    /**
     * @brief Checks if the button is currently pressed.
     * @return `true` if the button is being pressed, otherwise `false`.
     */
    bool isPressed() const;
};

/**
 * @brief Represents a software-controlled button.
 *
 * The `VirtualButton` class allows buttons to be simulated in software without requiring physical hardware.
 * This is useful for cases with no Pysical Buttons or testing
 */
class VirtualButton
{
public:
    uint8_t ID;   // Unique virtual button ID
    bool pressed; // Tracks whether the button is currently pressed

    /**
     * @brief Constructs a VirtualButton object.
     * @param id The unique identifier for this virtual button.
     */
    VirtualButton(uint8_t id) : ID(id), pressed(false)
    {
        switch (ID)
        {
        case BTN_A:
            ID = PIN_BTN_A;
            break;
        case BTN_B:
            ID = PIN_BTN_B;
            break;
        case BTN_UP:
            ID = PIN_BTN_UP;
            break;
        case BTN_DOWN:
            ID = PIN_BTN_DOWN;
            break;
        case BTN_LEFT:
            ID = PIN_BTN_LEFT;
            break;
        case BTN_RIGHT:
            ID = PIN_BTN_RIGHT;
            break;
        case BTN_MENU:
            ID = PIN_BTN_MENU;
            break;
        case BTN_SOFT_RESET:
            ID = PIN_BTN_SOFT_RESET;
            break;
        }
    }

    /**
     * @brief Simulates pressing the button.
     * Sets the button state to "pressed."
     */
    void press() { pressed = true; }

    /**
     * @brief Simulates releasing the button.
     * Sets the button state to "not pressed."
     */
    void release() { pressed = false; }

    /**
     * @brief Checks if the virtual button is currently pressed.
     * @return `true` if the button is pressed, otherwise `false`.
     */
    bool isPressed() const { return pressed; }
};

// 🔍 **Button Detection Functions**
/**
 * @brief Checks if a specific button is currently pressed.
 * @param id Button ID to check.
 * @return `true` if the button is pressed, otherwise `false`.
 */
bool isPressed(uint8_t id);

/**
 * @brief Waits until the specified button is pressed before proceeding.
 * @param id Button ID to wait for.
 * @return `true` once the button is detected.
 */
bool waitUntilBTNPressed(uint8_t id);

/**
 * @brief Determines which button was last pressed.
 * @return Button ID of the most recently pressed button, or `0` if none.
 */
uint8_t WhatButtonPressed();

/**
 * @brief Retrieves a pointer to a dynamically allocated `Button` object based on its hardware ID.
 * @param id The unique identifier corresponding to the button.
 * @return A pointer to a `Button` instance representing the requested button.
 *         Caller is responsible for deleting the returned pointer.
 */
Button* GetButtonFromID(uint8_t id);

/**
 * @brief Converts a physical Button pointer into a dynamically allocated VirtualButton pointer.
 *
 * This function takes a pointer to a `Button` object and creates a corresponding `VirtualButton`
 * with the same button ID. If the original `Button` was pressed at the time of conversion,
 * the `VirtualButton` will be initialized as pressed.
 *
 * @param button Pointer to the physical Button to convert.
 * @return A pointer to a new VirtualButton instance with the same ID and matching press state.
 *         Caller is responsible for deleting the returned pointer.
 */
VirtualButton* MakeVirtual(Button* button);

#endif /* I_H */