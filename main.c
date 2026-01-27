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

    int isTerminated = 1;
    while (isTerminated) {
        isTerminated = keyHandler(mainDisplay, mainWindow);

        XClearWindow(mainDisplay, mainWindow);
        newBox->drawButton(mainDisplay, mainWindow, context, *newBox);
    }

    destroyButtonBox(newBox);

    return OK;
}
