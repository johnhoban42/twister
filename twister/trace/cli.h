#include <argp.h>
#include <stdbool.h>
#include "../utils/constants.h"

// Struct for trace command line args
typedef struct trace_args{
    int rate;
    long length;
    char* out;
    unsigned long seed;
} trace_args;

#define DEFAULT_TRACE_ARGS {.rate = 10, .length = 100000, .out = "./trace.txt", .seed = TWISTER_SEED}

static error_t parse_opt(int key, char* arg, struct argp_state* state);
int parse_args(int argc, char** argv, trace_args* out);