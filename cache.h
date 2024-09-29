#ifndef CACHE_H
#define CACHE_H

#include <time.h>

enum CacheMethod { LRU, LFU }; 

typedef struct CacheEntry {
    int input;
    unsigned int steps;
    int LFUCount;
    time_t lastAccessed; 
} CacheEntry;

extern CacheEntry** cache; 
extern int cacheSize;  
extern unsigned int cacheHits; 
extern unsigned int cacheMisses;

void initialize(int size);
int lookUp(int input, enum CacheMethod method);
void evict(int minIndex, CacheEntry* entry);
void insert(int input, unsigned int steps, enum CacheMethod method);
unsigned int collatz(int input, enum CacheMethod method);
double cacheHitPercentage(unsigned int cacheHits, unsigned int cacheMisses); 

#endif