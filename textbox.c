#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "textbox.h"
#include "box.h"

int currentIndex = 0;
int widthOfPhrase = 0;

void drawTextBox(Display* mainDisplay, Window mainWindow, GC context, boxProperties properties) {
    XDrawRectangle(mainDisplay, mainWindow, context, properties.x, properties.y, properties.width, properties.height);
}

void drawTextWithPadding(Display* mainDisplay, Window mainWindow, GC context, textBox box) {
    int startX = box.properties->x + box.paddingAllSides;
    int startY = box.properties->y + box.paddingAllSides;
    int endX = startX + box.properties->width - 2 * box.paddingAllSides;
    int endY = startY + box.properties->height - 2 * box.paddingAllSides;
    XFontStruct* font = XLoadQueryFont(mainDisplay, "-misc-fixed-*-*-*-*-13-*-*-*-*-*-*-*");
    XSetFont(mainDisplay, context, font->fid);
    //widthOfPhrase = XTextWidth(font, box.text, strlen(box.text));
    // rather than make this dynamic,, lets test out something fixed
    widthOfPhrase = strlen(box.lines[currentIndex]) * 9;
    printf("Current width of the phrase: %d | sizeof(*box.lines[currentIndex]): %lu", widthOfPhrase, strlen(box.lines[currentIndex]));
    XDrawString(mainDisplay, mainWindow, context, startX, startY, box.lines[currentIndex], strlen(box.lines[currentIndex]));
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

static void tb_clientEvent(Display* mainDisplay, Window mainWindow, GC context) {
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

static void tb_destroyEvent(Display* mainDisplay) {
    XUngrabPointer(mainDisplay, CurrentTime);
}

void actUponReleasing(textBox* box,int x, int y) {
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
    if (widthOfPhrase > box->calculatedWidthOfLine) {
        box->properties->y += NEWLINE_SPACING;
        widthOfPhrase = 0;
        box->properties->x = box->properties->x + box->paddingAllSides;
        currentIndex++;
        box->numberOfLines++;
        box->lines = realloc(box->lines,
                              sizeof(char*) * box->numberOfLines);
        box->lines[currentIndex] = calloc(box->maxWordLength, 1);
    }
    strcat(box->text, input);
    strcpy(box->lines[currentIndex], box->text);
}

textBox* createTextBox(boxProperties* properties, const char* text) {

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
    box->clickEvent = tb_clientEvent;
    box->destroyEvent = tb_destroyEvent;
    box->numberOfLines = 1;
    box->calculatedWidthOfLine = box->properties->width - 2 * box->paddingAllSides;
    printf("Calculated width of line: %d", box->calculatedWidthOfLine);
    box->lines = (char**)malloc(sizeof(char*) * box->numberOfLines);
    if (!box->lines) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    box->lines[currentIndex] = strdup(box->text);
    box->cursorPos.x = 0;
    box->cursorPos.y = 0;
    box->properties->drawBox = drawTextBox;
    box->drawText = drawTextWithPadding;
    return box;
}

void freeTextBox(textBox* box) {
    free(box->text);
    free(box->properties);
    free(box);
}
