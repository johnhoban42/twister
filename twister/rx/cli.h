#include <argp.h>
#include <stdbool.h>

// Struct for RX command line args
typedef struct rx_args{
    int seq_length;
    bool verbose;
    char* out;
    int port;
} rx_args;

#define DEFAULT_RX_ARGS {.seq_length = 1000, .verbose = false, .out = "./out.txt", .port = 7777}

static error_t parse_opt(int key, char* arg, struct argp_state* state);
int parse_args(int argc, char** argv, rx_args* out);