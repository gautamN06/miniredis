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

    

    if(exists_entry(&db, "epithet")){
        printf("Eipthet exists\n");
    }

    if(!exists_entry(&db, "sword")){
        printf("Sword does not exist.\n");
    }

    return 0;
}