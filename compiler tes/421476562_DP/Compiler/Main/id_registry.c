#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "id_registry.h"

typedef struct Entry {
    char* lexeme;
    int id;
    struct Entry* next;
} Entry;

static Entry* head = NULL;
static int counter = 1; // Start IDs at 1

int getOrCreateID(const char* lexeme) {
    Entry* current = head;
    /* Search existing */
    while (current != NULL) {
        if (strcmp(current->lexeme, lexeme) == 0) {
            return current->id;
        }
        current = current->next;
    }

    /* Create new */
    Entry* newEntry = (Entry*)malloc(sizeof(Entry));
    newEntry->lexeme = strdup(lexeme);
    newEntry->id = counter++;
    newEntry->next = head;
    head = newEntry;

    return newEntry->id;
}