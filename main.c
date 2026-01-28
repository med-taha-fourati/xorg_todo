#include "definitions.h"
#include "window_attrib.h"
#include "button.h"
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char** argv) {
    Display* mainDisplay = XOpenDisplay(0);
    Window root = XDefaultRootWindow(mainDisplay);
    int defaultScreen = DefaultScreen(mainDisplay);

    Visual* windowVisual = CopyFromParent;
    int WindowClass = CopyFromParent;

    GC context = XDefaultGC(mainDisplay, defaultScreen);

    int attributeValueMask = CWBackPixel | CWEventMask;


    XSetWindowAttributes windowAttributes = {};
    windowAttributes.background_pixel = propreties.color;
    windowAttributes.event_mask =
        StructureNotifyMask |
        KeyPressMask |
        KeyReleaseMask |
        ExposureMask;


    Window mainWindow = XCreateWindow(
        mainDisplay,
        root,
        propreties.x,
        propreties.y,
        propreties.width,
        propreties.height,
        0, // Border Width
        0, // Window Depth
        WindowClass,
        windowVisual,
        attributeValueMask,
        &windowAttributes
    );

    XMapWindow(mainDisplay, mainWindow);

    XFlush(mainDisplay);

    buttonProperties* newBox = initButtonBox(mainDisplay, mainWindow, context);
    newBox->filled = UNFILLED;
    unsigned int t_prev, t_new, t_diff = 0;

    int isTerminated = 1;
    while (isTerminated) {
        XEvent GeneralEvent = {};
        XNextEvent(mainDisplay, &GeneralEvent);
        newBox->clickEvent(mainDisplay, mainWindow, context, *newBox);

        isTerminated = keyHandler(mainDisplay, mainWindow, GeneralEvent, t_new, t_prev, t_diff);

        XClearWindow(mainDisplay, mainWindow);
        newBox->drawButton(mainDisplay, mainWindow, context, *newBox);
    }

    newBox->destroyEvent(mainDisplay);
    destroyButtonBox(newBox);

    return OK;
}
