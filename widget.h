#ifndef WIDGET_H
#define WIDGET_H

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

typedef struct Widget {
    void (*draw)(Display* display, Window window, GC gc, struct Widget* self);
    void (*on_click)(Display* display, Window window, GC gc, struct Widget* self, int x, int y);
    void (*on_release)(Display* display, Window window, GC gc, struct Widget* self, int x, int y);
    void (*on_key)(struct Widget* self, int key_char, KeySym key_sym);
    void (*destroy)(Display* display, struct Widget* self);
    void* data;
} Widget;

#endif
