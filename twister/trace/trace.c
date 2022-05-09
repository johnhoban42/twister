#include <stdbool.h>
#include <stdio.h>
#include "../mtwister/mtwister.h"
#include "../trace/cli.h"
#include "../utils/constants.h"

int run(trace_args args){
    // Randomize
    MTRand mt = seedRand(args.seed);

    // Prepare output file
    FILE* f = fopen(args.out, "w");
    if(f == NULL){
        fprintf(stderr, "Unable to write to file %s\n", args.out);
        return 1;
    }
    printf("Writing packet trace to file '%s'...\n", args.out);

    // Generate the trace. 0 -> transmit, 1 -> drop
    while(args.length > 0){
        char* flag = genRand(&mt) < 1.0 / args.rate ? PKT_DROP : PKT_RECV;
        fputs(flag, f);
        args.length--;
    }
    fclose(f);
}

int main(int argc, char** argv){
    trace_args args = DEFAULT_TRACE_ARGS;
    parse_args(argc, argv, &args);
    run(args);
    return 0;
}