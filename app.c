#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include "definitions.h"
#include "window_attrib.h"
#include "widget_registrar.h"
#include "widget_textbox.h"
#include "widget_button.h"
#include "textbox.h"
#include "app.h"

static Display* g_display = 0;
static Window g_window = 0;
static GC g_gc = 0;
static XIC g_xic = 0;
static XIM g_xim = 0;

Display* app_display() { return g_display; }
Window app_window() { return g_window; }
GC app_gc() { return g_gc; }
XIC app_xic() { return g_xic; }

int app_init() {
    g_display = XOpenDisplay(0);
    if (!g_display) return 1;
    Window root = XDefaultRootWindow(g_display);
    int defaultScreen = DefaultScreen(g_display);

    Visual* windowVisual = CopyFromParent;
    int WindowClass = CopyFromParent;

    g_gc = XDefaultGC(g_display, defaultScreen);

    int attributeValueMask = CWBackPixel | CWEventMask;

    XSetWindowAttributes windowAttributes = {};
    windowAttributes.background_pixel = propreties.color;
    windowAttributes.event_mask =
        StructureNotifyMask |
        KeyPressMask |
        KeyReleaseMask |
        ExposureMask |
        ButtonPressMask |
        ButtonReleaseMask;

    g_window = XCreateWindow(
        g_display,
        root,
        propreties.x,
        propreties.y,
        propreties.width,
        propreties.height,
        0,
        0,
        WindowClass,
        windowVisual,
        attributeValueMask,
        &windowAttributes
    );

    XMapWindow(g_display, g_window);

    g_xim = XOpenIM(g_display, NULL, NULL, NULL);
    if (!g_xim) return 3;
    g_xic = XCreateIC(
        g_xim,
        XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
        XNClientWindow, g_window,
        XNFocusWindow, g_window,
        NULL
    );
    if (!g_xic) return 4;

    XFlush(g_display);

    registrar_init();

    boxProperties* textBoxProperties = (boxProperties*)malloc(sizeof(boxProperties));
    textBoxProperties->x = 100;
    textBoxProperties->y = 100;
    textBoxProperties->width = 200;
    textBoxProperties->height = 30;

    Widget* w_tb = initTextBox(textBoxProperties, "");
    registrar_add(w_tb);

    Widget* w_btn = initButton(NULL);
    registrar_add(w_btn);

    return 0;
}

int app_run() {
    unsigned int t_prev = 0, t_new = 0, t_diff = 0;
    int cont = 1;
    while (cont) {
        XEvent GeneralEvent = {};
        XNextEvent(g_display, &GeneralEvent);
        cont = keyHandler(g_display, g_window, g_gc, GeneralEvent, t_new, t_prev, t_diff, g_xic);
        XClearWindow(g_display, g_window);
        registrar_draw_all(g_display, g_window, g_gc);
    }
    return 0;
}

void app_destroy() {
    registrar_destroy_all(g_display);
    if (g_xic) XDestroyIC(g_xic);
    if (g_xim) XCloseIM(g_xim);
    if (g_window) XDestroyWindow(g_display, g_window);
    if (g_display) XCloseDisplay(g_display);
    g_display = 0;
    g_window = 0;
    g_gc = 0;
    g_xic = 0;
    g_xim = 0;
}
