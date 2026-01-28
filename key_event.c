#include "definitions.h"
#include "button.h"
#include <stdio.h>

int exitStatus = 1;
int keyHandler(Display* mainDisplay, Window mainWindow, XEvent GeneralEvent, unsigned int t_new, unsigned int t_prev, unsigned int t_diff) {
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
        case ButtonPress:{
            switch (GeneralEvent.xbutton.button) {
                case 1:
                    printf("Left Click\n");
                    t_prev=t_new;
                    printf("Click Occured      : [%d, %d]\n",
                           GeneralEvent.xbutton.x_root,
                           GeneralEvent.xbutton.y_root);
                    // inject button handler here
                    break;
                case 2:
                    printf("Grabed\n");
                    printf("Middle Click\n");
                    break;
                case 3:
                    printf("Right Click\n");
                    break;
                case 4:
                    printf("Grabed\n");
                    printf("Scroll UP\n");
                    break;
                case 5:
                    printf("Scroll Down\n");
                    break;
            }
            break;
        }
    }
    return exitStatus;
}
