#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "box.h"

enum textBoxState {
    NORMAL,
    SELECTED,
};

#define NEWLINE_SPACING 9 // just right now for later i will calculate font pixel size

typedef struct textBox {
    boxProperties* properties;
    int maxWordLength;
    int paddingAllSides;
    enum textBoxState state;
    int numberOfLines;
    int calculatedWidthOfLine;
    char** lines;
    void (*drawText)(Display* mainDisplay, Window mainWindow, GC context, struct textBox textBox);
    void (*clickEvent)(Display* mainDisplay, Window mainWindow, GC context);
    void (*destroyEvent)(Display* mainDisplay);
    char* text;
} textBox;

extern textBox* createTextBox(boxProperties* properties, const char* text);
extern void actUponClicking(textBox* box, int x, int y);
extern void actUponReleasing(textBox* box, int x, int y);
extern void actUponTyping(textBox* box, char* input);
extern boxProperties* createBoxPropertiesTest();
extern void freeTextBox(textBox* box);

#endif // TEXTBOX_H
