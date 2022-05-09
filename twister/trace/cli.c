#include <argp.h>
#include <stdlib.h>
#include "../trace/cli.h"


/* Program description */
static char doc[] = "Generate a randomized transmit/drop trace for mahimahi.";

/* Command line options */
static struct argp_option opts[] = {
    {"length",          'l', "<long>",      0, "Trace length. Default is 100,000.",                                     0},
    {"out",             'o', "<filename>",  0, "Output filename. Default is './trace.txt'.",                            0},
    {"rate",            'r', "<int>",       0, "Drop 1/r packets nondeterministically. Default is 10.",                 0},
    {"seed",            's', "<long>",      0, "Random seed to generate the trace. Must be 32-bit int in hex (0x...)",  0},
    {0}
};

/* Parse options with argp */
static error_t parse_opt(int key, char* arg, struct argp_state* state){

    error_t status = 0;
    trace_args* args = (trace_args*)state->input;

    switch(key){
        case 'l':
            args->length = atol(arg);
            break;
        case 'o':
            args->out = arg;
            break;
        case 'r':
            args->rate = atoi(arg);
            break;
        case 's':
            args->seed = strtol(arg, NULL, 16);
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
int parse_args(int argc, char** argv, trace_args* out){
    return argp_parse(&argparser, argc, argv, 0, 0, out);
}