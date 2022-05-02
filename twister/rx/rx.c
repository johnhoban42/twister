#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../rx/rxconnect.h"
#include "../rx/cli.h"
#include "../mtwister/mtwister.h"
#include "../utils/constants.h"
#include "../utils/utils.h"


rx_metrics rxm = INIT_RX_METRICS;

/* Receive a single datagram. Each should be a random sequence of unsigned longs. */
void* recv_datagram(udp_rx_conn* conn){
    char* msg = malloc(PACKET_SIZE);
    udp_recv(msg, PACKET_SIZE, conn);
    return msg;
}

/* Count the total bit errors in a packet by comparing received data with the correct pseudorandom sequence. */
int count_bit_errors(unsigned long* data, MTRand* mt){
    int errors = 0;
    unsigned long* correct_data = generate_payload(mt);
    for(int i = 0; i < BLOCKS_PER_PACKET; i++){
        unsigned long err_str = data[i] ^ correct_data[i];
        errors += count_set_bits(err_str);
    }
    return errors;
}

/* Report RX metrics after termination. */
void report_metrics(){
    int total_packets = rxm.pkt_received + rxm.pkt_drops;
    float pct_received = 100 * (float)rxm.pkt_received / total_packets;
    float receive_rate = rxm.pkt_received * PACKET_SIZE / (125000 * rxm.time_elapsed);
    float send_rate = total_packets * PACKET_SIZE / (125000 * rxm.time_elapsed);

    printf("\n");
    printf("-------------------------\n");
    printf("Twister RX Metrics Report\n");
    printf("-------------------------\n");
    printf("Packets received: %d\n", rxm.pkt_received);
    printf("Packets dropped: %d\n", rxm.pkt_drops);
    printf("Loss rate: %.5f%%\n", 100 - pct_received);
    printf("Perceived total packets sent: %d\n", total_packets);
    printf("Time elapsed: %.5f\n", rxm.time_elapsed);
    printf("Receive rate: %.5f Mb/s\n", receive_rate);
    printf("Perceived send rate: %.5f Mb/s\n", send_rate);
    printf("-------------------------\n");

    exit(0);
}

/* Run the channel test. */
int run(rx_args args){

    // Establish connection and random seed
    printf("Waiting for TX client...\n");
    udp_rx_conn* conn = connect_udp_rx(args.port);
    MTRand mt = seedRand(TWISTER_SEED);

    // Prepare output file
    FILE* f = fopen(args.out, "w");
    if(f == NULL){
        fprintf(stderr, "Unable to write to file %s\n", args.out);
        return 1;
    }

    // Metrics
    int sl = args.seq_length;
    long start;
    unsigned long* tx = calloc(BLOCKS_PER_PACKET, sizeof(unsigned long));
    unsigned long* rx = calloc(BLOCKS_PER_PACKET, sizeof(unsigned long));

    while(1){

        // Receive packet data and generate comparison sequence
        tx = recv_datagram(conn);
        rx = generate_payload(&mt);

        // Start timing upon receiving the first packet
        if(rxm.pkt_received == 0){
            printf("Receiving data from TX...\n");
            printf("Writing packet trace to file '%s'...\n", args.out);
            start = get_timestamp();
        }
        rxm.pkt_received++;
        rxm.time_elapsed = (double)(get_timestamp() - start) / 1000000;

        while(memcmp(tx, rx, PACKET_SIZE) != 0){
            rxm.pkt_drops++;
            rx = generate_payload(&mt);
            fputs(PKT_DROP, f);
        }
        fputs(PKT_RECV, f);

        if(args.verbose && rxm.pkt_received % sl == 0){
            printf("Packets Received: %d\t\tPacket Drops Detected: %d\t\tTime Elapsed: %.5f\n",
                rxm.pkt_received, rxm.pkt_drops, rxm.time_elapsed);
        }

    }
}

int main(int argc, char** argv){
    rx_args args = DEFAULT_RX_ARGS;
    parse_args(argc, argv, &args);
    signal(SIGINT, report_metrics);
    run(args);
    return 0;
}