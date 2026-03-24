#include "list.h"

list* createList(const char* startingText, enum doneStatus done) {
    list* newList = (list*)malloc(sizeof(list));

    newList->doneOrNotBoolean = done;
    newList->nextItem = NULL;
    newList->prevItem = NULL;
    newList->text = strdup(startingText ? startingText : "");

    return newList;
}

void destroyList(list** existingList) {
    // to not leave memory dangling, we will nullify each item one by one
    list* current = *existingList;
    while (current != NULL) {
        list* next = current->nextItem;
        free(current);
        current = next;
    }
    *existingList = NULL;
    //free(current->text); dangling pointer
}

void addNextItemAtEnd(list** existingList, const char* text, enum doneStatus done) {
    list* newElement = createList(text, done);
    if (*existingList == NULL) {
        *existingList = newElement;
        return;
    }
    list* current = *existingList;
    while (current->nextItem != NULL) {
        current = current->nextItem;
    }
    current->nextItem = newElement;
    newElement->prevItem = current;
}

void addPrevItemAtBeginning(list** existingList, const char* text, enum doneStatus done) {
    list* newElement = createList(text, done);
    if (*existingList != NULL) {
        newElement->nextItem = *existingList;
        (*existingList)->prevItem = newElement;
    }
    *existingList = newElement;
}

void removeItem(list** head, list* node) {
    if (!head || !*head || !node) return;
    if (node->prevItem) {
        node->prevItem->nextItem = node->nextItem;
    } else {
        *head = node->nextItem; // removing head
    }
    if (node->nextItem) {
        node->nextItem->prevItem = node->prevItem;
    }
    free(node);
}

/*
 * @brief returns the first instance found of that text
 * @params original doubly linked list, text to look for
 */
list* elementLookupByText(list* originalList, const char* text) {
    list* looking = originalList;
    while (looking->nextItem != NULL && !strcmp(looking->text, text)) {
        looking = looking->nextItem;
    }
    return looking;
}
