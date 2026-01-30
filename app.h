#ifndef APP_H
#define APP_H

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

int app_init();
int app_run();
void app_destroy();

Display* app_display();
Window app_window();
GC app_gc();
XIC app_xic();

#endif
