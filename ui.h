#ifndef UI_H
#define UI_H

#include <stdarg.h>  // For va_list
#include <stdio.h>   // For vsnprintf (optional)
#include <string.h>  // For strlen, strcpy
#include <stdlib.h>  // For itoa if needed
#include "image.cpp" // i should use a header file later.
#include "font.cpp"


typedef struct {
    Image_ SCREEN;
} ScreenImage;

/** The Screen of the Device */
inline ScreenImage* screen = nullptr;
inline int CURR_SCREEN_X = 0;

static int drawText(const char* text) {
    if (screen == nullptr) {
        return DEVICE_HARDWARE_CONFIGURATION_ERROR;
    }
    if (screen->SCREEN == nullptr) {
        return DEVICE_HARDWARE_CONFIGURATION_ERROR;
    }
    imagePrint(screen->SCREEN,text,0,CURR_SCREEN_X,1,font5)
    return DEVICE_OK;
}

int printf(const char* format, ...) {
    char buffer[80]; // Final output string
    va_list args;
    va_start(args, format);

    int len = 0;
    for (int i = 0; format[i] != '\0'; ++i) {
        if (format[i] == '%' && format[i + 1]) {
            ++i;
            switch (format[i]) {
                case 's': {
                    const char* s = va_arg(args, const char*);
                    len += snprintf(buffer + len, sizeof(buffer) - len, "%s", s);
                    break;
                }
                case 'd': {
                    int d = va_arg(args, int);
                    len += snprintf(buffer + len, sizeof(buffer) - len, "%d", d);
                    break;
                }
                case 'c': {
                    char c = (char)va_arg(args, int);
                    buffer[len++] = c;
                    break;
                }
                default:
                    buffer[len++] = '%';
                    buffer[len++] = format[i];
                    break;
            }
        } else {
            buffer[len++] = format[i];
        }

        if (len >= sizeof(buffer) - 1) break; // Prevent overflow
    }

    buffer[len] = '\0';
    va_end(args);

    return drawText(buffer);
}

#endif // UI_H