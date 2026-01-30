#include <stdlib.h>
#include <string.h>
#include "widget_textbox.h"
#include "textbox.h"

static void wt_draw(Display* display, Window window, GC gc, Widget* self) {
    textBox* tb = (textBox*)self->data;
    tb->properties->drawBox(display, window, gc, *tb->properties);
    tb->drawText(display, window, gc, *tb);
}

static void wt_click(Display* display, Window window, GC gc, Widget* self, int x, int y) {
    textBox* tb = (textBox*)self->data;
    tb->clickEvent(display, window, gc);
    actUponClicking(tb, x, y);
}

static void wt_release(Display* display, Window window, GC gc, Widget* self, int x, int y) {
    textBox* tb = (textBox*)self->data;
    actUponReleasing(tb, x, y);
}

static void wt_key(Widget* self, int key_char, KeySym key_sym) {
    textBox* tb = (textBox*)self->data;
    if (key_char <= 0) return;
    if (key_char < 128) {
        char s[2];
        s[0] = (char)key_char;
        s[1] = 0;
        actUponTyping(tb, s);
    }
}

static void wt_destroy(Display* display, Widget* self) {
    textBox* tb = (textBox*)self->data;
    tb->destroyEvent(display);
    freeTextBox(tb);
}

Widget* initTextBox(boxProperties* properties, const char* text) {
    Widget* w = (Widget*)malloc(sizeof(Widget));
    textBox* tb = createTextBox(properties, text);
    w->draw = wt_draw;
    w->on_click = wt_click;
    w->on_release = wt_release;
    w->on_key = wt_key;
    w->destroy = wt_destroy;
    w->data = tb;
    return w;
}
