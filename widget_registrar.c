#include <stdlib.h>
#include "widget_registrar.h"

static Widget** list = 0;
static int count = 0;
static int cap = 0;

void registrar_init() {
    count = 0;
    cap = 8;
    list = (Widget**)malloc(sizeof(Widget*) * cap);
}

void registrar_add(Widget* w) {
    if (count >= cap) {
        cap = cap * 2;
        list = (Widget**)realloc(list, sizeof(Widget*) * cap);
    }
    list[count++] = w;
}

void registrar_remove(Widget* w) {
    for (int i = 0; i < count; ++i) {
        if (list[i] == w) {
            list[i] = list[count - 1];
            count -= 1;
            break;
        }
    }
}

void registrar_draw_all(Display* display, Window window, GC gc) {
    for (int i = 0; i < count; ++i) {
        if (list[i]->draw) list[i]->draw(display, window, gc, list[i]);
    }
}

void registrar_click(Display* display, Window window, GC gc, int x, int y) {
    for (int i = 0; i < count; ++i) {
        if (list[i]->on_click) list[i]->on_click(display, window, gc, list[i], x, y);
    }
}

void registrar_release(Display* display, Window window, GC gc, int x, int y) {
    for (int i = 0; i < count; ++i) {
        if (list[i]->on_release) list[i]->on_release(display, window, gc, list[i], x, y);
    }
}

void registrar_key(int key_char, KeySym key_sym) {
    for (int i = 0; i < count; ++i) {
        if (list[i]->on_key) list[i]->on_key(list[i], key_char, key_sym);
    }
}

void registrar_destroy_all(Display* display) {
    for (int i = 0; i < count; ++i) {
        if (list[i]) {
            if (list[i]->destroy) list[i]->destroy(display, list[i]);
            free(list[i]);
        }
    }
    free(list);
    list = 0;
    count = 0;
    cap = 0;
}
