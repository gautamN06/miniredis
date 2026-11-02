#include <stdlib.h>
#include "database.h"
#include <string.h> 

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

//the SET from redis 
int insert_entry(Database *db, const char *key, const char *value){

    //find its bucket
    int index = hash(key);


    Entry *curr = db->buckets[index];

    while (curr != NULL){

        if(strcmp(curr->key, key) == 0){
            curr->value = (char *) value; 
            return 1; 
        }

        curr = curr->next; 
    }

    //key don't exist, create new entry
    Entry *newEntry = create_entry(key, value);

    if(newEntry == NULL) {
        return 0; 
    }

    //connecting the new entry to the exisiting linked list 
    newEntry->next = db->buckets[index];


    //make the new entry the first entry in bucket 
    db->buckets[index] = newEntry;

    return 1; 
}

//the GET from redis 
const char *get_entry(Database *db, const char *key){

    //finding the bucket 
    int index = hash(key);

    Entry *curr = db->buckets[index];

    while(curr != NULL){

        //check whehter this is the entry or not 
        if(strcmp(curr->key, key) == 0){
            return curr->value;
        }

        curr = curr->next; 

    }

    return NULL; 

}

//the DEL from redis 

int delete_entry(Database *db, const char *key){

    int index = hash(key);

    Entry *curr = db->buckets[index];
    Entry *prev = NULL; 

    while (curr != NULL){

        if(strcmp(curr->key, key) == 0){
            if (prev == NULL){
                db->buckets[index] = curr->next; 
            }else{
                prev->next = curr->next; 
            }

            free(curr);

            return 1; 
        }

        prev = curr; 
        curr = curr->next; 

    }
    return 0; 
}