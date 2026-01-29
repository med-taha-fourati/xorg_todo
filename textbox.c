#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "textbox.h"
#include "box.h"

textBox* box = NULL;

void drawTextBox(Display* mainDisplay, Window mainWindow, GC context, boxProperties properties) {
    // regular box
    XDrawRectangle(mainDisplay, mainWindow, context, properties.x, properties.y, properties.width, properties.height);
}

void drawTextWithPadding(Display* mainDisplay, Window mainWindow, GC context, textBox box) {
    // draw text with padding
    int startX = box.properties->x + box.paddingAllSides;
    int startY = box.properties->y + box.paddingAllSides;
    int endX = startX + box.properties->width - 2 * box.paddingAllSides;
    int endY = startY + box.properties->height - 2 * box.paddingAllSides;
    XDrawString(mainDisplay, mainWindow, context, startX, startY, box.text, strlen(box.text));
    //XmbDrawText(mainDisplay, mainWindow, context, startX, startY, NULL, 0);
}

boxProperties* createBoxPropertiesTest() {
    boxProperties* properties = (boxProperties*)malloc(sizeof(boxProperties));
    if (!properties) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    properties->x = 40;
    properties->y = 40;
    properties->width = 100;
    properties->height = 120;

    return properties;
}

void actUponClicking(textBox* box,int x, int y) {
    if (box == NULL) {
            printf("Box is null\n");
            return;
        };
        if (x >= box->properties->x && x < (box->properties->x + box->properties->width) && y >= box->properties->y && y < (box->properties->y + box->properties->height)) box->state = SELECTED;
        else box->state = NORMAL;
}

void clientEvent(Display* mainDisplay, Window mainWindow, GC context) {
    XGrabPointer(
        mainDisplay,
        mainWindow,
        True,
        PointerMotionMask | ButtonPressMask | ButtonReleaseMask,
        GrabModeAsync,
        GrabModeAsync,
        None,
        None,
        CurrentTime
    );


    XAllowEvents(mainDisplay, AsyncPointer, CurrentTime);
}

void destroyEvent(Display* mainDisplay) {
    XUngrabPointer(mainDisplay, CurrentTime);
}

void actUponReleasing(textBox* box,int x, int y) {
    // if (box == NULL) {
    //             printf("Box is null\n");
    //             return;
    //         };
    //         if (x >= box->properties->x && x < (box->properties->x + box->properties->width) && y >= box->properties->y && y < (box->properties->y + box->properties->height)) box->state = SELECTED;
    //         else box->state = NORMAL;
    // not sure what to do....
}

void actUponTyping(textBox* box, char* input) {
    if (box->state != SELECTED) {
        return;
    }
    int len = strlen(box->text);
    if (len + strlen(input) > box->maxWordLength) {
        fprintf(stderr, "Input exceeds maximum word length\n");
        return;
    }
    strcat(box->text, input);
}

textBox* initTextBox(Display* display, Window window, GC context, boxProperties* properties, const char* text) {
    textBox* box = (textBox*)malloc(sizeof(textBox));
    if (!box) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    box->properties = properties;
    box->maxWordLength = 2000;
    box->text = (char*)malloc(box->maxWordLength * sizeof(char));
    if (!box->text) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy(box->text, text);

    box->paddingAllSides = 10;
    box->clickEvent = clientEvent;
    box->destroyEvent = destroyEvent;

    box->properties->drawBox = drawTextBox;
    box->drawText = drawTextWithPadding;
    return box;
}

void freeTextBox(textBox* box) {
    free(box->text);
    free(box->properties);
    free(box);
}
