#include "label.h"

void label_draw(Display* mainDisplay, Window mainWindow, GC gc, labelProperties* label) {
    XDrawString(mainDisplay, mainWindow, gc, label->posX, label->posY, label->text, strlen(label->text));
}

labelProperties* label_create() {
    char* text = (char*)malloc(sizeof(char) * 100);
    labelProperties* label = malloc(sizeof(labelProperties));
    label->posX = 0;
    label->posY = 0;
    label->text = text;
    label->drawText = label_draw;
    return label;
}

void label_destroy(labelProperties* label) {
    free(label->text);
    free(label);
}

void label_set_text(labelProperties* label, const char* text) {
    free(label->text);
    label->text = strdup(text);
}
