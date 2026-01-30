#ifndef WIDGET_REGISTRAR_H
#define WIDGET_REGISTRAR_H

#include "widget.h"

void registrar_init();
void registrar_add(Widget* w);
void registrar_remove(Widget* w);
void registrar_draw_all(Display* display, Window window, GC gc);
void registrar_click(Display* display, Window window, GC gc, int x, int y);
void registrar_release(Display* display, Window window, GC gc, int x, int y);
void registrar_key(int key_char, KeySym key_sym);
void registrar_destroy_all(Display* display);

#endif
