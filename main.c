#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cache.h"

int main(int argc, char* argv[]) {
    if (argc != 6) {
        printf("Error: Expected 5 arguments - N, MIN, MAX, CACHE_METHOD (LFU or LRU), and CACHE_SIZE.\n");
        return 1; 
    }

    int N = atoi(argv[1]); 
    int MIN = atoi(argv[2]);
    int MAX = atoi(argv[3]); 
    enum CacheMethod method = (strcmp(argv[4], "LFU") == 0) ? LFU : LRU;
    int userCacheSize = atoi(argv[5]); 

    if (userCacheSize <= 0) {
        printf("Error: Cache size must be a positive integer.\n");
        return 1;
    }

    srand(time(NULL));
    initialize(userCacheSize);

    for (int ix = 0; ix < N; ix++) {
        int random_number = MIN + rand() % (MAX - MIN + 1);
        int result = collatz(random_number, method); 
        printf("Number of steps for %d: %d\n", random_number, result); 
    }

    double hitPercentage = cacheHitPercentage(cacheHits, cacheMisses); 

    printf("Cache hit percentage: %.2f%%\n", hitPercentage);
    
    for (int ix = 0; ix < cacheSize; ix++) {
        if (cache[ix]) {
            free(cache[ix]);
        }
    }
    free(cache);

    return 0; 
}
