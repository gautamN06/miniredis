#include <stdio.h>
#include <stdlib.h>
#include "database.h"

int main() {

    Database db; 
    //&db = memory address of db 
    init_database(&db);

    insert_entry(&db, "epithet", "Red Hair");
    insert_entry(&db, "age", "40");
    insert_entry(&db, "allegiance", "Red Hair Pirates");
    insert_entry(&db, "haki", "Conquerors");
    insert_entry(&db, "island", "Elbaf");

    for(int i = 0; i < TABLE_SIZE; i++){
        printf("Bucket %d: ", i);

        Entry *curr = db.buckets[i];

        while(curr != NULL){
            printf("[%s:%s] -> ", curr->key, curr->value);

            curr = curr->next; 
        }

        printf("NULL\n");
    }

    return 0;
}