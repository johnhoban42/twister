#include <argp.h>
#include <stdlib.h>
#include "../rx/cli.h"


/* Program description */
static char doc[] = "Receive data over a UDP connection and generate connection performance statistics.";

/* Command line options */
static struct argp_option opts[] = {
    {"seq-length",      'l', "<int>",   0, "Number of packets received before verifying data via LCS. Default is 1000.",     0},
    {"verbose",         'v', 0,         0, "Enable verbose output.",                                                         0},
    {0}
};

/* Parse options with argp */
static error_t parse_opt(int key, char* arg, struct argp_state* state){

    error_t status = 0;
    rx_args* args = (rx_args*)state->input;

    switch(key){
        case 'l':
            args->seq_length = atoi(arg);
            break;
        case 'v':
            args->verbose = true;
            break;
        default:
            status = ARGP_ERR_UNKNOWN;
            break;
    }

    return status;

}

/* Arg parser */
static struct argp argparser = {
    .options        = opts,
    .parser         = parse_opt,
    .doc            = doc,
    .children       = 0,
    .help_filter    = 0,
    .argp_domain    = 0
};

/* Wrapper to parse all command line arguments. */
int parse_args(int argc, char** argv, rx_args* out){
    return argp_parse(&argparser, argc, argv, 0, 0, out);
}