#include "definitions.h"
#include "textbox.h"
#include <X11/X.h>
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
            printf("Key Pressed: %d\n", event->keycode);
            //actUponTyping(box, (char*)event->keycode);
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
                           GeneralEvent.xbutton.x,
                           GeneralEvent.xbutton.y);
                    // inject button handler here
                    actUponClicking(box, GeneralEvent.xbutton.x,
                        GeneralEvent.xbutton.y);
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
        case ButtonRelease: {
            switch (GeneralEvent.xbutton.button) {
                case 1:
                    printf("Left Click Released\n");
                    actUponReleasing(box,GeneralEvent.xbutton.x,
                        GeneralEvent.xbutton.y);
                    break;
                case 2:
                    printf("Middle Click Released\n");
                    break;
                case 3:
                    printf("Right Click Released\n");
                    break;
                case 4:
                    printf("Scroll UP Released\n");
                    break;
                case 5:
                    printf("Scroll Down Released\n");
                    break;
            }
            break;
        }
    }
    return exitStatus;
}
