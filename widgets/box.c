#include "box.h"

/*need to fix segfault here as it is not returning anything here*/
void drawBox(Display* mainDisplay, Window mainWindow, GC context, boxProperties box) {
    XDrawRectangle(
        mainDisplay,
        mainWindow,
        context,
        box.x,
        box.y,
        box.width,
        box.height
    );
    if (box.filled == FILLED) {
        XFillRectangle(
            mainDisplay,
            mainWindow,
            context,
            box.x,
            box.y,
            box.width,
            box.height
        );
    }
}

boxProperties* initBox(Display* mainDisplay, Window mainWindow, GC context) {
    boxProperties* box = (boxProperties*)malloc(sizeof(boxProperties));
    
    box->x = 30;
    box->y = 20;
    box->width = 10;
    box->height = 10;
    box->color = 0x00000000;

    box->drawBox = drawBox;

    return box;
}

void destroyBox(boxProperties* box) {
    free(box);
}
