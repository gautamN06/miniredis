#include <stdlib.h>
#include "database.h"

Entry *create_entry(const char *key, const char *value){
    Entry *newEntry = malloc(sizeof(Entry));

    if (newEntry == NULL){
        return NULL; 
    }

    newEntry->key = (char *)key;
    newEntry->value = (char *)value; 
    newEntry->next = NULL; 

    return newEntry; 
}