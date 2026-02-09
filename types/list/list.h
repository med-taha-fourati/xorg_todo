#ifndef LIST_DECL_H
#define LIST_DECL_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

enum doneStatus {
    DONE = 0,
    NOT_DONE = 1
};

typedef struct list {
    const char* text;
    uint8_t doneOrNotBoolean;
    struct list *nextItem;
    struct list *prevItem;
} list;

#endif
