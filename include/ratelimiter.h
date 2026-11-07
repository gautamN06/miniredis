#ifndef RATELIMIT_H
#define RATELIMIT_H 

//token bucketing 

typedef struct {
    double tokens;
    double capacity;
    double rate;
    double last_time; 
} TokenBucket; 

void bucket_init(TokenBucket *b, double capacity, double rate);

//returns 1 if the reuqest is allowed and spends a token and returns 0 if rejected 
int bucket_allow(TokenBucket *b); 

#endif 