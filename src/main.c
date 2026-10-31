#include <stdio.h>
#include <stdlib.h>
#include "database.h"

int main() {

    Entry *firstEntry = create_entry("username", "John Doe"); 
    
    if (firstEntry == NULL){
        printf("Failed to create memory for this entry.\n");
        return 1;
    }


    printf("Key: %s\n", firstEntry->key);
    printf("Value: %s\n", firstEntry->value);

    free(firstEntry); 

    return 0;
}