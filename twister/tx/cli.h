#include <argp.h>

// Struct for TX command line args
typedef struct tx_args{
    float rate;
    long servaddr;
    int port;
} tx_args;

#define DEFAULT_TX_ARGS {.rate = 10.0, .servaddr = 0x64400001, .port = 7777}

static error_t parse_opt(int key, char* arg, struct argp_state* state);
int parse_args(int argc, char** argv, tx_args* out);