#include <strings.h>
#include "mtwister/mtwister.h"

long get_timestamp();
unsigned long* generate_payload(MTRand* mt);
int count_set_bits(unsigned long ul);
int packet_lcs(unsigned long** seq1, unsigned long** seq2, int len1, int len2);