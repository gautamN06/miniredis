#ifndef DATABASE_H
#define DATABASE_H 

#include <stddef.h>

#define TABLE_SIZE 1024

typedef struct Entry {
    char *key; //pointer to string representing key 
    char *value;  //pointer to string representing value 
    double expires_at;
    struct Entry *next;  //pointer to another entry 

    struct Entry *lru_prev;
    struct Entry *lru_next; 

} Entry; 


typedef struct Database {
    Entry *buckets[TABLE_SIZE];
    Entry *lru_head;       //most recently used 
    Entry *lru_tail;    //least recently used (first to be removed) 
    size_t size;    
    size_t capacity;    //max entries before LRU eviction 

} Database; 


/* capacity must be > 0. */
void init_database(Database *db, size_t capacity);

unsigned int hash(const char *key);

/* SET. Copies key and value. ttl_seconds <= 0 means no expiration.
 * Evicts the least recently used entry if the database is full.
 * Returns 1 on success, 0 on allocation failure. */
int insert_entry(Database *db, const char *key, const char *value, double ttl_seconds);

/* GET. Returns NULL if missing or expired. The returned pointer is valid
 * until the next call that modifies the database. */
const char *get_entry(Database *db, const char *key);

/* DEL. Returns 1 if the key existed, 0 otherwise. */
int delete_entry(Database *db, const char *key);

/* EXISTS. Returns 1 or 0. */
int exists_entry(Database *db, const char *key);

size_t db_size(const Database *db);

/* Prints keys from most to least recently used. */
void print_keys(const Database *db);

/* Frees everything and resets the database to empty. */
void flush_database(Database *db);

#endif