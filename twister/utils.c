#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include "mtwister/mtwister.h"
#include "constants.h"

/* Get a timestamp of "now", in microseconds. */
long get_timestamp(){
    struct timeval* tv = malloc(sizeof(struct timeval));
    gettimeofday(tv, NULL);
    return (long)(1000000*tv->tv_sec + tv->tv_usec);
}

/* Generate a random payload by concatenating k pseudorandom 32-bit strings. */
unsigned long* generate_payload(MTRand* mt){
    unsigned long* data = malloc(PACKET_SIZE);
    bzero(data, PACKET_SIZE);
    for(int j = 0; j < BLOCKS_PER_PACKET; j++){
        data[j] = genRandLong(mt);
    }
    return data;
}

/* Count 1 bits in a bit string according to Brian Kernighan's algorithm. */
int count_set_bits(unsigned long ul){
    int n = 0;
    while(ul != 0){
        ul = ul & (ul-1);
        n++;
    }
    return n;
}

/* Max of two elements. */
int max(int x, int y){
    return (x > y) ? x : y;
}

/* Return the longest common subsequence of two sequences of packets. */
int _packet_lcs(unsigned long** seq1, unsigned long** seq2, int len1, int len2, int cache[len1][len2]){
    if(len1 == 0 || len2 == 0){
        return 0;
    }
    // Cache hit
    if(cache[len1][len2] != -1){
        return cache[len1][len2];
    }
    // Cache miss
    if(memcmp(seq1[len1-1], seq2[len2-1], PACKET_SIZE) == 0){
        cache[len1][len2] = 1 + _packet_lcs(seq1, seq2, len1-1, len2-1, cache);
    }else{
        cache[len1][len2] = max(_packet_lcs(seq1, seq2, len1, len2-1, cache),
                                _packet_lcs(seq1, seq2, len1-1, len2, cache));
    }
    return cache[len1][len2];
}

int packet_lcs(unsigned long** seq1, unsigned long** seq2, int len1, int len2){
    int cache[len1+1][len2+1];
    for(int i = 0; i < len1+1; i++){
        for(int j = 0; j < len2+1; j++){
            cache[len1][len2] = -1;
        }
    }
    int lcs = _packet_lcs(seq1, seq2, len1, len2, cache);
    return lcs;
}