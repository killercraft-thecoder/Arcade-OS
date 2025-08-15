#ifndef UI_H
#define UI_H

#include <stdarg.h>  // For va_list
#include <stdio.h>   // For vsnprintf (optional)
#include <string.h>  // For strlen, strcpy
#include <stdlib.h>  // For itoa if needed

#define UI_OKAY 0
#define UI_NOT_OKAY 1

// Replace this with your actual screen rendering function
static void drawText(const char* text); // You must implement this elsewhere

int printf(const char* format, ...) {
    char buffer[512]; // Final output string
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

    drawText(buffer); // Send to your screen renderer
    return UI_OKAY;
}

#endif // UI_H