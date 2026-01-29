#include "definitions.h"
#include "window_attrib.h"
#include <X11/Xlib.h>
#include "textbox.h"
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

    XIM xim = XOpenIM(mainDisplay, NULL, NULL, NULL);
        if (!xim) {
            return 3;
        }
        XIC xic = XCreateIC(
            xim,
            XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
            XNClientWindow, mainWindow,
            XNFocusWindow, mainWindow,
            NULL
        );
        if (!xic) {
            return 4;
        }

    XFlush(mainDisplay);

    unsigned int t_prev, t_new, t_diff = 0;
    box = initTextBox(mainDisplay, mainWindow, context, createBoxPropertiesTest(), "llll");

    int isTerminated = 1;
    while (isTerminated) {
        XEvent GeneralEvent = {};
        XNextEvent(mainDisplay, &GeneralEvent);

        isTerminated = keyHandler(mainDisplay, mainWindow, GeneralEvent, t_new, t_prev, t_diff);

        XClearWindow(mainDisplay, mainWindow);

        box->properties->drawBox(mainDisplay, mainWindow, context, *box->properties);
        box->drawText(mainDisplay, mainWindow, context, *box);
        box->clickEvent(mainDisplay, mainWindow, context);
    }

    box->destroyEvent(mainDisplay);
    freeTextBox(box);

    return OK;
}
