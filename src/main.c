#include <stdio.h>
#include "database.h"

int main() {

    Entry firstEntry; 
    firstEntry.key = "username";
    firstEntry.value = "John Doe";
    firstEntry.next = NULL; 

    printf("Key: %s\n", firstEntry.key);
    printf("Value: %s\n", firstEntry.value);


    return 0;
}