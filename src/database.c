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

void init_database(Database *db){
    for (int i = 0; i < TABLE_SIZE; i++){
        db->buckets[i] = NULL; 
    }
}

int hash(const char *key){
    int sum = 0;

    for (int i = 0; key[i] != '\0'; i++)
    {
        sum += key[i];
    }

    return sum % TABLE_SIZE; 
}