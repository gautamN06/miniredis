#ifndef DATABASE_H
#define DATABASE_H 



typedef struct Entry {
    char *key; //pointer to string representing key 
    char *value;  //pointer to string representing value 

    struct Entry *next;  //pointer to another entry 
} Entry; 


Entry *create_entry(const char *key, const char *value); 



#endif 