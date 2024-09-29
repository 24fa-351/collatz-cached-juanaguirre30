#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "cache.h"

CacheEntry** cache; 
int cacheSize;  

unsigned int cacheHits = 0;
unsigned int cacheMisses = 0; 

void initialize(int size) {
    cacheSize = size;
    cache = (CacheEntry**)malloc(cacheSize * sizeof(CacheEntry*)); 
    for (int ix = 0; ix < cacheSize; ix++) {
        cache[ix] = NULL;
    }
}

int lookUp(int input, enum CacheMethod method) {
    for (int ix = 0; ix < cacheSize; ix++) {
        if (cache[ix] && cache[ix]->input == input) {
            if (method == LFU) {
                cache[ix]->LFUCount++;
            } else if (method == LRU) {
                cache[ix]->lastAccessed = time(NULL); 
            }
            cacheHits++;
            return cache[ix]->steps;
        }
    }
    cacheMisses++; 
    return -1; 
}

void evict(int minIndex, CacheEntry* entry) {
    free(cache[minIndex]);
    cache[minIndex] = entry;
}

void insert(int input, unsigned int steps, enum CacheMethod method) {
    CacheEntry* entry = (CacheEntry*)malloc(sizeof(CacheEntry));
    entry->input = input;
    entry->steps = steps;
    entry->LFUCount = 1; 
    entry->lastAccessed = time(NULL); 

    for (int ix = 0; ix < cacheSize; ix++) {
        if (!cache[ix]) {
            cache[ix] = entry;
            return;
        }
    }

    if (method == LFU) {
        int minCount = INT_MAX;
        int minIndex = 0;
        for (int ix = 0; ix < cacheSize; ix++) {
            if (cache[ix]->LFUCount < minCount) {
                minCount = cache[ix]->LFUCount;
                minIndex = ix;
            }
        }
        evict(minIndex, entry);
    
    } else if (method == LRU) {
        time_t minTime = time(NULL);
        int minIndex = 0;
        for (int ix = 0; ix < cacheSize; ix++) {
            if (cache[ix]->lastAccessed < minTime) {
                minTime = cache[ix]->lastAccessed;
                minIndex = ix;
            }
        }
        evict(minIndex, entry);
    }
}

unsigned int collatz(int input, enum CacheMethod method) {
    int cached_steps = lookUp(input, method);
    if (cached_steps != -1) {
        return cached_steps; 
    }

    unsigned int steps;
    if (input == 1) { 
        steps = 0; 
    } else if (input % 2 == 0) { 
        steps = 1 + collatz(input / 2, method);
    } else {
        steps = 1 + collatz((input * 3) + 1, method);
    }

    insert(input, steps, method);
    return steps;
}

double cacheHitPercentage(unsigned int cacheHits, unsigned int cacheMisses) {
    double percentage = (double)cacheHits / (cacheHits + cacheMisses) * 100.0;
    return percentage; 
}