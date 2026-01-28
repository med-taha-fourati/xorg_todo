#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdint.h>

#define OK 0

extern Display* mainDisplay;
extern Window root;
extern GC context;

typedef struct propreties {
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;
    long color;
} windowProperties;

extern int keyHandler(Display* mainDisplay, Window mainWindow, XEvent GeneralEvent, unsigned int t_new, unsigned int t_prev, unsigned int t_diff);
extern void initX11(Display** displayPtr, Window* windowPtr, GC* gcPtr);

#endif // DEFINITIONS_H
