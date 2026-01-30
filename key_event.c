#include "definitions.h"
#include "widget_registrar/widget_registrar.h"
#include <X11/X.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <wchar.h>

KCSPair kcs_cache[256] = {0};
static KCSPair getKCSPair(XKeyEvent *event, XIC xic) {
    if (event->type != KeyPress) {
        return kcs_cache[event->keycode & 255];
    }
    KCSPair pair = {0, NoSymbol};
    wchar_t buf[3];
    buf[XwcLookupString(xic, event, buf, (sizeof(buf) / sizeof(wchar_t)) - 1, &pair.key_sym, NULL)] = 0;
    // Are we looking at a lead surrogate?
    if ((buf[0] & 0xFC00) == 0xD800) {
        // We can reasonably assume the next byte is a trail surrogate.
        pair.key_char = (buf[0] << 10) + buf[1] - 56613888;
    } else {
        pair.key_char = buf[0];
    }
    kcs_cache[event->keycode & 255] = pair;
    return pair;
}

int exitStatus = 1;
int keyHandler(Display* mainDisplay,
    Window mainWindow,
    GC context,
    XEvent GeneralEvent,
    unsigned int t_new,
    unsigned int t_prev,
    unsigned int t_diff,
    XIC xic
) {

    Atom WM_DELETE_WINDOW = XInternAtom(mainDisplay, "WM_DELETE_WINDOW", False);
    if (!XSetWMProtocols(mainDisplay, mainWindow, &WM_DELETE_WINDOW, 1)) {
        printf("Couldn't register WM_DELETE_WINDOW property\n");
    }

    switch (GeneralEvent.type) {

        case KeyPress:
        {
            XKeyPressedEvent *event = (XKeyPressedEvent*)&GeneralEvent;
            printf("Key Pressed: %d\n", event->keycode);
            KCSPair kcspair = getKCSPair(event, xic);
            XFilterEvent(&GeneralEvent, mainWindow);
            printf("Translated Key: %c\n", kcspair.key_char);
            registrar_key(kcspair.key_char, kcspair.key_sym);
            if (event->keycode == XKeysymToKeycode(mainDisplay, XK_Escape)) exitStatus = 0;
        } break;
        case KeyRelease:
        {
            XKeyReleasedEvent *event = (XKeyReleasedEvent*)&GeneralEvent;
            printf("Key Released: %d\n", event->keycode);
            KCSPair kcspair = getKCSPair(event, xic);
            XFilterEvent(&GeneralEvent, mainWindow);
            printf("Translated Key: %c\n", kcspair.key_char);
            //registrar_key(kcspair.key_char, kcspair.key_sym);
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
                    registrar_click(mainDisplay, mainWindow, context, GeneralEvent.xbutton.x,
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
                    registrar_release(mainDisplay, mainWindow, context, GeneralEvent.xbutton.x,
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
