#ifndef BOX_H
#define BOX_H

#include <stdint.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <stdlib.h>

enum fillOptions {
    FILLED = 0,
    UNFILLED = 1
};

typedef struct boxPropreties {
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;
    long color;
    enum fillOptions filled;
    void (*drawBox)(Display* mainDisplay, Window mainWindow, GC context, struct boxPropreties box);
    int depth;
} boxProperties;

/* should clean up tbh */
extern boxProperties* initBox(Display* mainDisplay, Window mainWindow, GC context);
extern void destroyBox(boxProperties* box);

#endif
