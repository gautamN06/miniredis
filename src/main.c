#include <stdio.h>
#include <stdlib.h>
#include "database.h"

int main() {

    Database db; 
    //&db = memory address of db 
    init_database(&db);

    printf("MiniRedis database is initialized.\n");

    for (int i =0; i < TABLE_SIZE; i++){
        //%p = format specifier for printing pointer's address 
        printf("Bucket %d: %p\n", i, (void *)db.buckets[i]);
    }

    return 0;
}