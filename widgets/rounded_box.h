#ifndef ROUNDED_BOX_H
#define ROUNDED_BOX_H

#include <stdint.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <stdlib.h>

enum fillOptions {
    FILLED = 0,
    UNFILLED = 1
};

typedef struct roundedBoxPropreties {
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;
    int radius;
    long color; 
    enum fillOptions filled;
    void (*drawRoundedBox)(Display* mainDisplay, Window mainWindow, GC context, struct roundedBoxPropreties box);
} roundedBoxProperties;

/* should clean up tbh */
extern roundedBoxProperties* initRoundedBox(Display* mainDisplay, Window mainWindow, GC context);
extern void destroyRoundedBox(roundedBoxProperties* box);

#endif
