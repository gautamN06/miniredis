#include <stdio.h>
#include <stdlib.h>
#include "database.h"

int main() {

    Database db; 
    //&db = memory address of db 
    init_database(&db);

    char *keys[] = {
        "epithet",
        "age",
        "allegiance", 
        "haki",
        "island"
    };

    for (int i = 0; i < 5; i++){
        int index = hash(keys[i]);

        printf("Key: %s | Bucket: %d\n", keys[i], index);
    }

    return 0;
}