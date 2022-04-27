#include <argp.h>
#include <stdlib.h>
#include "../tx/cli.h"


/* Program description */
static char doc[] = "Transmit pseudorandom test data at a fixed rate over a UDP connection.";

/* Command line options */
static struct argp_option opts[] = {
    {"address",     'a', "<address>",   0, "Server address in hex format. Default is 0x64400001 (127.0.0.1).",  0},
    {"port",        'p', "<port>",      0, "Transmit to RX via this port. Default is 7777.",                    0},
    {"rate",        'r', "<float>",     0, "Packet transmission rate in Mb/s. Default is 10.0.",                0},
    {0}
};

/* Parse options with argp */
static error_t parse_opt(int key, char* arg, struct argp_state* state){

    error_t status = 0;
    tx_args* args = (tx_args*)state->input;

    switch(key){
        case 'a':
            args->servaddr = strtol(arg, NULL, 16);
            break;
        case 'p':
            args->port = atoi(arg);
            break;
        case 'r':
            args->rate = atof(arg);
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
int parse_args(int argc, char** argv, tx_args* out){
    return argp_parse(&argparser, argc, argv, 0, 0, out);
}