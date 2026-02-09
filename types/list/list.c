#include "list.h"

list* createList(const char* startingText, enum doneStatus done) {
    list* newList = (list*)malloc(sizeof(list));

    newList->doneOrNotBoolean = done;
    newList->nextItem = NULL;
    newList->prevItem = NULL;
    newList->text = startingText == NULL ? "" : startingText;

    return newList;
}

void destroyList(list** existingList) {
    // to not leave memory dangling, we will nullify each item one by one
    list* itemToDelete = *existingList;
    list* copy = NULL;
    while (itemToDelete->nextItem != NULL) {
        copy = itemToDelete->nextItem;
        copy = NULL;
        free(copy);
        itemToDelete = copy;
    }
    free(itemToDelete);
}

void addNextItemAtEnd(list** existingList, const char* text, enum doneStatus done) {
    list* copy = *existingList;
    list* newElement = createList(text, done);
    copy->nextItem = newElement;
    newElement->prevItem = copy;
}

void addPrevItemAtBeginning(list** existingList, const char* text, enum doneStatus done) {
    list* copy = *existingList;
    list* newElement = createList(text, done);
    copy->prevItem = newElement;
    newElement->nextItem = copy;
}

void removeItem(list** existingList, list* listElementToRemove) {
    list* copy = *existingList;
    list* copy2 = *existingList;
    list* found = NULL;
    while (copy->nextItem != NULL && copy != listElementToRemove) {
        found = copy->nextItem;
    }
    if (found->nextItem == NULL) {
        copy2->nextItem = NULL;
    } else if (found->prevItem == NULL) {
        copy2->prevItem = NULL;
    } else {
        copy2->nextItem = found->nextItem;
        found->nextItem->prevItem = copy2;
        copy2->prevItem = found->prevItem;
        found->prevItem->prevItem = copy2;
    }
    free(found);
}

/*
 * @brief returns the first instance found of that text
 * @params original doubly linked list, text to look for
 */
list* elementLookupByText(list* originalList, const char* text) {
    list* looking = originalList;
    while (looking->nextItem != NULL && looking->text == text) {
        looking = originalList->nextItem;
    }
    return looking;
}
