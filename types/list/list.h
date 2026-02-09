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

list* createList(const char* startingText, enum doneStatus done);
void destroyList(list** existingList);
void addNextItemAtEnd(list** existingList, const char* text, enum doneStatus done);
void addPrevItemAtBeginning(list** existingList, const char* text, enum doneStatus done);
void removeItem(list** existingList, list* listElementToRemove);
list* elementLookupByText(list* originalList, const char* text);

#endif
