#ifndef DATABASE_H
#define DATABASE_H 

#define TABLE_SIZE 10 

typedef struct Entry {
    char *key; //pointer to string representing key 
    char *value;  //pointer to string representing value 

    struct Entry *next;  //pointer to another entry 
} Entry; 


typedef struct Database {
    Entry *buckets[TABLE_SIZE];
} Database; 

Entry *create_entry(const char *key, const char *value); 

void init_database(Database *db); 

int hash(const char *key); 

int insert_entry(Database *db, const char *key, const char *value); 

const char *get_entry(Database *db, const char *key);


int delete_entry(Database *db, const char *key); 

int exists_entry(Database *db, const char *key); 

void print_keys(Database *db);

void flush_database(Database *db); 

#endif 