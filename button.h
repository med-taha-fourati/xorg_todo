#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

enum fillOptions {
    FILLED = 0,
    UNFILLED = 1
};

typedef struct buttonPropreties {
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;
    int radius;
    long color;
    enum fillOptions filled;
    const char* text;
    void (*drawButton)(Display* mainDisplay, Window mainWindow, GC context, struct buttonPropreties box);
    void (*clickEvent)();
} buttonProperties;

/* should clean up tbh */
extern buttonProperties* initButtonBox(Display* mainDisplay, Window mainWindow, GC context);
extern void destroyButtonBox(buttonProperties* box);

#endif
