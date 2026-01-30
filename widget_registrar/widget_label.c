#include "widget_label.h"

static void wl_draw_label(Display* display, Window window, GC context, Widget* widget) {
    labelProperties* props = (labelProperties*)widget->data;
    // Draw the label text
    props->drawText(display, window, context, props);
}

static void wl_destroy_label(Display* display, Widget* widget) {
    labelProperties* props = (labelProperties*)widget->data;
    label_destroy(props);
    free(widget);
}

Widget* initLabel(labelProperties* label) {
    if (!label)
        return NULL;
    Widget* widget = (Widget*)malloc(sizeof(Widget));
    labelProperties* props = label ? label : (labelProperties*)malloc(sizeof(labelProperties));

    widget->data = props;
    widget->draw = wl_draw_label;//props->drawText;

    widget->on_click = NULL;
    widget->on_key = NULL;
    widget->on_release = NULL;

    widget->destroy = wl_destroy_label;  //props->destroy;
    return widget;
}
