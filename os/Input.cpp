#include "Input.h"
#include "pins.h"
#include "configkeys.h"
#include <cstdint>

// Helper: Check if a pin is HIGH
static bool isHigh(int key)
{
    DigitalInOutPin pin = pins::pinByCfg(key);
    return pin && pin->getDigitalValue() == 1;
}

// Implementation of Button::isPressed
bool Button::isPressed() const
{
    return isHigh(Hardware_ID);
}

// Implementation of isPressed for button ID
bool isPressed(uint8_t id)
{
    return Button(id).isPressed();
}

// Wait until a button is pressed
bool waitUntilBTNPressed(uint8_t id)
{
    while (!isPressed(id))
    {
    }
    return true;
}

// Return the first button found pressed, or BTN_NULL_ID if none
uint8_t WhatButtonPressed()
{
    for (uint8_t id = BTN_A; id <= BTN_SOFT_RESET; ++id)
    {
        if (isPressed(id))
            return id;
    }
    return BTN_NULL_ID;
}

// Get a Button object from an ID (returns a pointer, caller must delete)
Button* GetButtonFromID(uint8_t id)
{
    return new Button(id);
}

// Convert a Button to a VirtualButton (returns a pointer, caller must delete)
VirtualButton* MakeVirtual(Button* button)
{
    VirtualButton* temporary = new VirtualButton(button->Hardware_ID);
    if (button->isPressed())
    {
        temporary->press();
    }
    return temporary;
}