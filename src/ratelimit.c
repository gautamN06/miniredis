#include "ratelimit.h"
#include "util.h"

void bucket_init(TokenBucket *b, double capacity, double rate) {
    b->tokens = capacity;
    b->capacity = capacity;
    b->rate = rate;
    b->last_time = monotonic_seconds();
}

int bucket_allow(TokenBucket *b) {
    double now = monotonic_seconds();

    /* Lazy refill: add the tokens earned since the last call. No timer needed. */
    b->tokens += (now - b->last_time) * b->rate;
    if (b->tokens > b->capacity) b->tokens = b->capacity;
    b->last_time = now;

    if (b->tokens >= 1.0) {
        b->tokens -= 1.0;
        return 1;
    }
    return 0;
}