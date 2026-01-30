#include "button.h"
#include <X11/X.h>
#include <X11/Xlib.h>

void drawButtonBox(Display* mainDisplay, Window mainWindow, GC context, buttonProperties box) {
    int x = box.x;
    int y = box.y;
    int w = box.width;
    int h = box.height;
    int r = box.radius;

    XFontStruct* font = XLoadQueryFont(mainDisplay, "-misc-fixed-*-*-*-*-13-*-*-*-*-*-*-*");
    XSetFont(mainDisplay, context, font->fid);
    int widthOfPhrase = XTextWidth(font, box.text, strlen(box.text));
    printf("Length: %d\n", widthOfPhrase);
    int ratio = (w - widthOfPhrase + (2*strlen(box.text)));
    printf("Ratio: %d\n", ratio);
    int startXText = (x+ratio) / 2;
    int startYText = y+(9+h) /2;

    if (box.filled == FILLED) {
        XFillArc(mainDisplay, mainWindow, context, x, y, 2 * r, 2 * r, 90 * 64, 90 * 64);
        XFillArc(mainDisplay, mainWindow, context, x + w - 2 * r, y, 2 * r, 2 * r, 0 * 64, 90 * 64);
        XFillArc(mainDisplay, mainWindow, context, x + w - 2 * r, y + h - 2 * r, 2 * r, 2 * r, 270 * 64, 90 * 64);
        XFillArc(mainDisplay, mainWindow, context, x, y + h - 2 * r, 2 * r, 2 * r, 180 * 64, 90 * 64);

    // Top line (from arc end to next arc start)
        XDrawLine(mainDisplay, mainWindow, context, x + r, y, x + w - r, y);

        // Bottom line
        XDrawLine(mainDisplay, mainWindow, context, x + r, y + h, x + w - r, y + h);

        // Left line
        XDrawLine(mainDisplay, mainWindow, context, x, y + r, x, y + h - r);

        // Right line
        XDrawLine(mainDisplay, mainWindow, context, x + w, y + r, x + w, y + h - r);

        // Fill polygon
        XPoint points[] = {
            {x + r, y + h - r},
            {x + w - r, y + h - r},
            {x + w - r, y + h},
            {x + r, y + h}
        };
        XPoint points2[] = {
            {x + r, y + r},
            {x + w - r, y + r},
            {x + w - r, y},
            {x + r, y}
        };
        XPoint points3[] = {
            {x , y + r},
            {x , y + h - r},
            {x + r, y + h - r},
            {x + r, y + r}
        };
        XPoint points4[] = {
            {x + w - r, y + r},
            {x + w - r, y + h - r},
            {x + w, y + h - r},
            {x + w, y + r}
        };
        XFillPolygon(mainDisplay, mainWindow, context, points, 4, Convex, CoordModeOrigin);
        XFillPolygon(mainDisplay, mainWindow, context, points2, 4, Convex, CoordModeOrigin);
        XFillPolygon(mainDisplay, mainWindow, context, points3, 4, Convex, CoordModeOrigin);
        XFillPolygon(mainDisplay, mainWindow, context, points4, 4, Convex, CoordModeOrigin);
    } else {

        // Top-Left Corner (start angle 90 degrees, sweep 90 degrees)
        // Bounding box for the arc is (x, y) to (x + 2*r, y + 2*r)
        XDrawArc(mainDisplay, mainWindow, context, x, y, 2 * r, 2 * r, 90 * 64, 90 * 64);

        // Top-Right Corner (start angle 0 degrees, sweep 90 degrees)
        // Bounding box is (x + w - 2*r, y) to (x + w, y + 2*r)
        XDrawArc(mainDisplay, mainWindow, context, x + w - 2 * r, y, 2 * r, 2 * r, 0 * 64, 90 * 64);

        // Bottom-Right Corner (start angle 270 degrees, sweep 90 degrees)
        // Bounding box is (x + w - 2*r, y + h - 2*r) to (x + w, y + h)
        XDrawArc(mainDisplay, mainWindow, context, x + w - 2 * r, y + h - 2 * r, 2 * r, 2 * r, 270 * 64, 90 * 64);

        // Bottom-Left Corner (start angle 180 degrees, sweep 90 degrees)
        // Bounding box is (x, y + h - 2*r) to (x + 2*r, y + h)
        XDrawArc(mainDisplay, mainWindow, context, x, y + h - 2 * r, 2 * r, 2 * r, 180 * 64, 90 * 64);
        XDrawLine(mainDisplay, mainWindow, context, x + r, y, x + w - r, y);

        // Bottom line
        XDrawLine(mainDisplay, mainWindow, context, x + r, y + h, x + w - r, y + h);

        // Left line
        XDrawLine(mainDisplay, mainWindow, context, x, y + r, x, y + h - r);

        // Right line
        XDrawLine(mainDisplay, mainWindow, context, x + w, y + r, x + w, y + h - r);


    }
    // 2. Draw the four connecting straight lines

    XDrawString(
        mainDisplay, mainWindow, context, startXText, startYText, box.text, strlen(box.text)
    );
}

static void btn_clientEvent(Display* mainDisplay, Window mainWindow, GC context, buttonProperties box) {
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

static void btn_destroyEvent(Display* mainDisplay) {
    XUngrabPointer(mainDisplay, CurrentTime);
}

buttonProperties* createButton(Display* mainDisplay, Window mainWindow, GC context) {
    buttonProperties* b = (buttonProperties*)malloc(sizeof(buttonProperties));

    b->x = 30;
    b->y = 20;
    b->width = 100;
    b->height = 40;
    b->color = 0x00000000;
    b->radius = 10;
    b->filled = UNFILLED;

    b->drawButton = drawButtonBox;
    b->text = "Press me!!";

    b->clickEvent = btn_clientEvent;

    b->destroyEvent = btn_destroyEvent;
    return b;
}

void actUponButtonClick(buttonProperties* box, int x, int y) {
    if (box == NULL) {
        printf("Box is null\n");
        return;
    };
    if (x >= box->x && x < (box->x + box->width) && y >= box->y && y < (box->y + box->height)) box->filled = FILLED;
    else box->filled = UNFILLED;
}

void actUponButtonRelease(buttonProperties* box, int x, int y) {
    if (box == NULL) {
        printf("Box is null\n");
        return;
    };
    if (x >= box->x && x < (box->x + box->width) && y >= box->y && y < (box->y + box->height)) box->filled = UNFILLED;
    else box->filled = UNFILLED;
}

void destroyButtonBox(buttonProperties* box) {
    free(box);
}
