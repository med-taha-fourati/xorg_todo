#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "box.h"

enum textBoxState {
    NORMAL,
    SELECTED,
};

typedef struct textBox {
    boxProperties* properties;
    int maxWordLength;
    int paddingAllSides;
    enum textBoxState state;
    void (*drawText)(Display* mainDisplay, Window mainWindow, GC context, struct textBox textBox);
    void (*clickEvent)(Display* mainDisplay, Window mainWindow, GC context);
    void (*destroyEvent)(Display* mainDisplay);
    char* text;
} textBox;

extern textBox* initTextBox(Display* display, Window window, GC context, boxProperties* properties, const char* text);
extern void actUponClicking(textBox* box, int x, int y);
extern void actUponReleasing(textBox* box, int x, int y);
extern void actUponTyping(textBox* box, char* input);
extern boxProperties* createBoxPropertiesTest();

extern textBox* box;

extern void freeTextBox(textBox* box);

#endif // TEXTBOX_H
