#include "definitions.h"
#include <stdio.h>

int exitStatus = 1;

int keyHandler(Display* mainDisplay, Window mainWindow) {
    XEvent GeneralEvent = {};
    XNextEvent(mainDisplay, &GeneralEvent);

    Atom WM_DELETE_WINDOW = XInternAtom(mainDisplay, "WM_DELETE_WINDOW", False);
    if (!XSetWMProtocols(mainDisplay, mainWindow, &WM_DELETE_WINDOW, 1)) {
        printf("Couldn't register WM_DELETE_WINDOW property\n");
    }

    switch (GeneralEvent.type) {
        case KeyPress:
        case KeyRelease:
        {
            XKeyPressedEvent *event = (XKeyPressedEvent*)&GeneralEvent;

            if (event->keycode == XKeysymToKeycode(mainDisplay, XK_Escape)) exitStatus = 0;
        } break;
        case ClientMessage:
        {
            XClientMessageEvent *client_event = (XClientMessageEvent*) &GeneralEvent;
            if ((Atom)client_event->data.l[0] == WM_DELETE_WINDOW) {
                XDestroyWindow(mainDisplay, mainWindow);
                exitStatus = 0;
            }
        } break;
    }
    return exitStatus;
}
