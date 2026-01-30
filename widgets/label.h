#ifndef LABEL_H
#define LABEL_H

#include "../widget_registrar/widget_registrar.h"
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

typedef struct labelProperties {
    int posX;
    int posY;
    char* text;
    void (*drawText)(Display* mainDisplay, Window window, GC gc, struct labelProperties* label);
} labelProperties;

labelProperties* label_create();
void label_destroy(labelProperties* label);
void label_set_text(labelProperties* label, const char* text);

#endif
