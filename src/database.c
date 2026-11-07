#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "database.h"
#include "util.h"

/* ---------- helpers ---------- */

static void free_entry(Entry *e) {
    free(e->key);
    free(e->value);
    free(e);
}

static Entry *create_entry(const char *key, const char *value, double expires_at) {
    Entry *e = calloc(1, sizeof(Entry));
    if (!e) return NULL;

    /* Copy the strings: the caller's buffers get reused (e.g. by the server). */
    e->key = strdup(key);
    e->value = strdup(value);
    if (!e->key || !e->value) {
        free_entry(e);          /* free(NULL) is safe */
        return NULL;
    }
    e->expires_at = expires_at;
    return e;
}

unsigned int hash(const char *key) {
    unsigned long h = 5381;                       /* djb2 */
    for (size_t i = 0; key[i] != '\0'; i++)
        h = h * 33 + (unsigned char)key[i];
    return (unsigned int)(h % TABLE_SIZE);
}

/* ---------- LRU list ---------- */

static void lru_unlink(Database *db, Entry *e) {
    if (e->lru_prev) e->lru_prev->lru_next = e->lru_next;
    else             db->lru_head = e->lru_next;

    if (e->lru_next) e->lru_next->lru_prev = e->lru_prev;
    else             db->lru_tail = e->lru_prev;

    e->lru_prev = e->lru_next = NULL;
}

static void lru_push_front(Database *db, Entry *e) {
    e->lru_prev = NULL;
    e->lru_next = db->lru_head;
    if (db->lru_head) db->lru_head->lru_prev = e;
    db->lru_head = e;
    if (!db->lru_tail) db->lru_tail = e;
}

static void lru_touch(Database *db, Entry *e) {
    if (db->lru_head == e) return;
    lru_unlink(db, e);
    lru_push_front(db, e);
}

/* ---------- core operations ---------- */

/* Removes e from the bucket chain AND the LRU list, then frees it. */
static void remove_entry(Database *db, Entry *e) {
    unsigned int idx = hash(e->key);
    Entry *curr = db->buckets[idx];
    Entry *prev = NULL;

    while (curr && curr != e) {
        prev = curr;
        curr = curr->next;
    }
    if (!curr) return;

    if (prev) prev->next = curr->next;
    else      db->buckets[idx] = curr->next;

    lru_unlink(db, e);
    free_entry(e);
    db->size--;
}

/* Finds a live entry. Expired entries are deleted on the spot (lazy expiration). */
static Entry *find_entry(Database *db, const char *key) {
    Entry *curr = db->buckets[hash(key)];

    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            if (curr->expires_at > 0 && monotonic_seconds() >= curr->expires_at) {
                remove_entry(db, curr);
                return NULL;
            }
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

void init_database(Database *db, size_t capacity) {
    for (int i = 0; i < TABLE_SIZE; i++) db->buckets[i] = NULL;
    db->lru_head = db->lru_tail = NULL;
    db->size = 0;
    db->capacity = capacity > 0 ? capacity : 1;
}

int insert_entry(Database *db, const char *key, const char *value, double ttl_seconds) {
    double expires_at = ttl_seconds > 0 ? monotonic_seconds() + ttl_seconds : 0;

    Entry *existing = find_entry(db, key);
    if (existing) {
        char *copy = strdup(value);
        if (!copy) return 0;
        free(existing->value);
        existing->value = copy;
        existing->expires_at = expires_at;
        lru_touch(db, existing);
        return 1;
    }

    if (db->size >= db->capacity && db->lru_tail)
        remove_entry(db, db->lru_tail);             /* evict least recently used */

    Entry *e = create_entry(key, value, expires_at);
    if (!e) return 0;

    unsigned int idx = hash(key);
    e->next = db->buckets[idx];
    db->buckets[idx] = e;
    lru_push_front(db, e);
    db->size++;
    return 1;
}

const char *get_entry(Database *db, const char *key) {
    Entry *e = find_entry(db, key);
    if (!e) return NULL;
    lru_touch(db, e);
    return e->value;
}

int delete_entry(Database *db, const char *key) {
    Entry *e = find_entry(db, key);
    if (!e) return 0;
    remove_entry(db, e);
    return 1;
}

int exists_entry(Database *db, const char *key) {
    return get_entry(db, key) != NULL;
}

size_t db_size(const Database *db) {
    return db->size;
}

void print_keys(const Database *db) {
    for (const Entry *e = db->lru_head; e; e = e->lru_next)
        printf("%s\n", e->key);
}

void flush_database(Database *db) {
    Entry *curr = db->lru_head;
    while (curr) {
        Entry *next = curr->lru_next;
        free_entry(curr);
        curr = next;
    }
    for (int i = 0; i < TABLE_SIZE; i++) db->buckets[i] = NULL;
    db->lru_head = db->lru_tail = NULL;
    db->size = 0;
}