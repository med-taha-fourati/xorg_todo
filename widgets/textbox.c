#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "textbox.h"
#include "box.h"

void drawTextBox(Display* mainDisplay, Window mainWindow, GC context, boxProperties properties) {
    XDrawRectangle(mainDisplay, mainWindow, context, properties.x, properties.y, properties.width, properties.height);
}

void drawTextWithPadding(Display* mainDisplay, Window mainWindow, GC context, textBox box) {
    if (box.properties == NULL) return;
    int startX = box.properties->x + box.paddingAllSides;
    int startY = box.properties->y + box.paddingAllSides;
    int innerW = box.properties->width - 2 * box.paddingAllSides;
    int innerH = box.properties->height - 2 * box.paddingAllSides;
    if (innerW <= 0 || innerH <= 0) return;
    XFontStruct* font = XLoadQueryFont(mainDisplay, "-misc-fixed-*-*-*-*-13-*-*-*-*-*-*-*");
    if (!font) return;
    XSetFont(mainDisplay, context, font->fid);
    int ascent = font->ascent;
    int descent = font->descent;
    int lineH = ascent + descent;
    if (lineH <= 0) lineH = NEWLINE_SPACING;
    int maxVisible = innerH / lineH;
    if (maxVisible < 1) maxVisible = 1;
    int firstVisible = box.firstVisibleLine;
    if (firstVisible < 0) firstVisible = 0;
    if (firstVisible > box.numberOfLines - maxVisible) firstVisible = box.numberOfLines > maxVisible ? box.numberOfLines - maxVisible : 0;
    int y = startY + ascent;
    for (int i = firstVisible; i < box.numberOfLines && (i - firstVisible) < maxVisible; i++) {
        char* ln = box.lines && box.lines[i] ? box.lines[i] : NULL;
        if (ln) {
            XDrawString(mainDisplay, mainWindow, context, startX, y, ln, strlen(ln));
        }
        y += lineH;
    }
    XFreeFont(mainDisplay, font);
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

static void tb_insert_new_line(textBox* box, int at) {
    if (at < 0) at = 0;
    if (at > box->numberOfLines) at = box->numberOfLines;
    char** nl = (char**)realloc(box->lines, sizeof(char*) * (box->numberOfLines + 1));
    if (!nl) return;
    box->lines = nl;
    for (int i = box->numberOfLines; i > at; i--) box->lines[i] = box->lines[i-1];
    box->lines[at] = (char*)calloc(box->maxWordLength, 1);
    if (!box->lines[at]) return;
    box->numberOfLines += 1;
}

static void tb_remove_line(textBox* box, int at) {
    if (at < 0 || at >= box->numberOfLines) return;
    if (box->lines && box->lines[at]) free(box->lines[at]);
    for (int i = at; i < box->numberOfLines - 1; i++) box->lines[i] = box->lines[i+1];
    box->numberOfLines -= 1;
    if (box->numberOfLines == 0) {
        box->lines = NULL;
        return;
    }
    char** nl = (char**)realloc(box->lines, sizeof(char*) * box->numberOfLines);
    if (nl) box->lines = nl;
}

static int tb_total_length(textBox* box) {
    int t = 0;
    for (int i = 0; i < box->numberOfLines; i++) if (box->lines[i]) t += strlen(box->lines[i]);
    return t;
}

void actUponTyping(textBox* box, char* input) {
    if (box == NULL || input == NULL) return;
    if (box->state != SELECTED) return;
    int charWidth = box->charWidth > 0 ? box->charWidth : 9;
    if (charWidth <= 0) charWidth = 9;
    int maxChars = box->calculatedWidthOfLine / charWidth;
    if (maxChars < 1) maxChars = 1;
    int innerH = box->properties ? (int)box->properties->height - 2 * box->paddingAllSides : 0;
    int lineH = box->lineHeight > 0 ? box->lineHeight : NEWLINE_SPACING;
    int maxVisible = lineH > 0 && innerH > 0 ? innerH / lineH : 1;
    if (maxVisible < 1) maxVisible = 1;
    size_t inlen = strlen(input);
    for (size_t k = 0; k < inlen; k++) {
        if (tb_total_length(box) + 1 > box->maxWordLength) break;
        char c = input[k];
        if (box->cursorPos.line < 0) box->cursorPos.line = 0;
        if (box->cursorPos.line >= box->numberOfLines) box->cursorPos.line = box->numberOfLines - 1;
        if (box->cursorPos.line < 0) {
            tb_insert_new_line(box, 0);
            box->cursorPos.line = 0;
            box->cursorPos.col = 0;
        }
        char* line = box->lines[box->cursorPos.line];
        if (!line) {
            box->lines[box->cursorPos.line] = (char*)calloc(box->maxWordLength, 1);
            line = box->lines[box->cursorPos.line];
            if (!line) return;
        }
        int linelen = strlen(line);
        if (box->cursorPos.col < 0) box->cursorPos.col = 0;
        if (box->cursorPos.col > linelen) box->cursorPos.col = linelen;
        if (c == '\n') {
            // handle newline
            tb_insert_new_line(box, box->cursorPos.line + 1);
            box->cursorPos.line += 1;
            box->cursorPos.col = 0;
        } else if (c == '\b') {
            if (box->cursorPos.col > 0) {
                memmove(line + box->cursorPos.col - 1, line + box->cursorPos.col, linelen - box->cursorPos.col + 1);
                box->cursorPos.col -= 1;
            } else if (box->cursorPos.line > 0) {
                int prev = box->cursorPos.line - 1; // previous index
                char* pl = box->lines[prev]; // previous line
                int pllen = pl ? strlen(pl) : 0;
                int curlen = line ? strlen(line) : 0;
                if (!pl) {
                    box->lines[prev] = (char*)calloc(box->maxWordLength, 1);
                    pl = box->lines[prev];
                    pllen = 0;
                }
                if (pl && line) {
                    if (pllen + curlen < box->maxWordLength) {
                        memcpy(pl + pllen, line, curlen + 1);
                        box->cursorPos.line = prev;
                        box->cursorPos.col = pllen;
                        tb_remove_line(box, prev + 1);
                        line = pl;
                        linelen = strlen(line);
                        while (linelen > maxChars) {
                            tb_insert_new_line(box, box->cursorPos.line + 1);
                            char* next = box->lines[box->cursorPos.line + 1];
                            if (!next) break;
                            char* move = line + maxChars;
                            strcpy(next, move);
                            line[maxChars] = '\0';
                            linelen = strlen(line);
                        }
                    } else {
                        box->cursorPos.line = prev;
                        box->cursorPos.col = pllen > 0 ? pllen - 1 : 0;
                        if (pllen > 0) pl[pllen - 1] = '\0';
                    }
                }
            }
            goto scroll_update;
        }
        if (c == '\n') {
            char* tail = NULL;
            if (linelen - box->cursorPos.col > 0) tail = strdup(line + box->cursorPos.col);
            line[box->cursorPos.col] = '\0';
            tb_insert_new_line(box, box->cursorPos.line + 1);
            box->cursorPos.line += 1;
            box->cursorPos.col = 0;
            if (tail) {
                strncpy(box->lines[box->cursorPos.line], tail, box->maxWordLength - 1);
                box->lines[box->cursorPos.line][box->maxWordLength - 1] = '\0';
                free(tail);
            }
            line = box->lines[box->cursorPos.line - 1];
            linelen = strlen(line);
            goto wrap_check;
        }
        if (linelen + 1 >= box->maxWordLength) continue;
        memmove(line + box->cursorPos.col + 1, line + box->cursorPos.col, linelen - box->cursorPos.col + 1);
        line[box->cursorPos.col] = c;
        box->cursorPos.col += 1;
        linelen += 1;
wrap_check:
        while (linelen > maxChars) {
            tb_insert_new_line(box, box->cursorPos.line + 1);
            char* next = box->lines[box->cursorPos.line + 1];
            if (!next) break;
            char* move = line + maxChars;
            strcpy(next, move);
            line[maxChars] = '\0';
            if (box->cursorPos.col > maxChars || (box->cursorPos.line < box->numberOfLines - 1 && box->cursorPos.col == linelen)) {
                box->cursorPos.line += 1;
                box->cursorPos.col = (int)strlen(next) - (int)strlen(move) + (box->cursorPos.col - maxChars);
                if (box->cursorPos.col < 0) box->cursorPos.col = 0;
            }
            line = next;
            linelen = strlen(line);
        }
scroll_update:
        if (box->cursorPos.line >= box->firstVisibleLine + maxVisible) box->firstVisibleLine = box->cursorPos.line - maxVisible + 1;
        if (box->cursorPos.line < box->firstVisibleLine) box->firstVisibleLine = box->cursorPos.line;
    }
    int tlen = 0;
    for (int i = 0; i < box->numberOfLines; i++) if (box->lines[i]) tlen += strlen(box->lines[i]);
    if (box->text) box->text[0] = '\0';
    if (box->text) {
        int written = 0;
        for (int i = 0; i < box->numberOfLines; i++) {
            if (!box->lines[i]) continue;
            int ll = strlen(box->lines[i]);
            if (written + ll >= box->maxWordLength) break;
            strncat(box->text, box->lines[i], box->maxWordLength - 1 - strlen(box->text));
            written += ll;
        }
    }
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
    box->text[0] = '\0';
    if (text) strncat(box->text, text, box->maxWordLength - 1);

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
    box->lines[0] = (char*)calloc(box->maxWordLength, 1);
    if (!box->lines[0]) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    if (box->text) strncat(box->lines[0], box->text, box->maxWordLength - 1);
    box->cursorPos.line = 0;
    box->cursorPos.col = strlen(box->lines[0]);
    box->firstVisibleLine = 0;
    box->maxVisibleLines = 0;
    box->lineHeight = 0;
    box->ascent = 0;
    box->descent = 0;
    box->charWidth = 9;
    box->font = NULL;
    Display* d = XOpenDisplay(NULL);
    if (d) {
        XFontStruct* f = XLoadQueryFont(d, "-misc-fixed-*-*-*-*-13-*-*-*-*-*-*-*");
        if (f) {
            box->ascent = f->ascent;
            box->descent = f->descent;
            box->lineHeight = box->ascent + box->descent;
            if (box->lineHeight <= 0) box->lineHeight = NEWLINE_SPACING;
            box->charWidth = XTextWidth(f, "M", 1);
            if (box->charWidth <= 0) box->charWidth = 9;
            XFreeFont(d, f);
        }
        XCloseDisplay(d);
    }
    box->properties->drawBox = drawTextBox;
    box->drawText = drawTextWithPadding;
    return box;
}

void freeTextBox(textBox* box) {
    if (!box) return;
    if (box->lines) {
        for (int i = 0; i < box->numberOfLines; i++) if (box->lines[i]) free(box->lines[i]);
        free(box->lines);
    }
    if (box->text) free(box->text);
    if (box->properties) free(box->properties);
    free(box);
}
