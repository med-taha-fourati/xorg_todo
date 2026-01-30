#include <stdlib.h>
#include "widget_button.h"
#include "../widgets/button.h"

static void wb_draw(Display* display, Window window, GC gc, Widget* self) {
    buttonProperties* b = (buttonProperties*)self->data;
    b->drawButton(display, window, gc, *b);
}

static void wb_click(Display* display, Window window, GC gc, Widget* self, int x, int y) {
    buttonProperties* b = (buttonProperties*)self->data;
    b->clickEvent(display, window, gc, *b);
    actUponButtonClick(b, x, y);
}

static void wb_release(Display* display, Window window, GC gc, Widget* self, int x, int y) {
    buttonProperties* b = (buttonProperties*)self->data;
    actUponButtonRelease(b, x, y);
}

static void wb_key(Widget* self, int key_char, KeySym key_sym) {
}

static void wb_destroy(Display* display, Widget* self) {
    buttonProperties* b = (buttonProperties*)self->data;
    b->destroyEvent(display);
    destroyButtonBox(b);
}

Widget* initButton(buttonProperties* initial) {
    Widget* w = (Widget*)malloc(sizeof(Widget));
    buttonProperties* b = initial ? initial : createButton(0, 0, 0);
    w->draw = wb_draw;
    w->on_click = wb_click;
    w->on_release = wb_release;
    w->on_key = wb_key;
    w->destroy = wb_destroy;
    w->data = b;
    return w;
}
