#ifndef WIDGET_LABEL_H
#define WIDGET_LABEL_H

#include "widget.h"
#include "../widgets/label.h"
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

typedef struct labelProperties labelProperties;

Widget* initLabel(labelProperties* label);

#endif
