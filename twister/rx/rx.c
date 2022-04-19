#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../rx/rxconnect.h"
#include "../rx/cli.h"
#include "../mtwister/mtwister.h"
#include "../utils/constants.h"
#include "../utils/utils.h"


/* Receive a single datagram. Each should be a random sequence of unsigned longs. */
unsigned long* recv_datagram(udp_rx_conn* conn){
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

/* Run the channel test. */
int run(int sl){

    // Establish connection and random seed
    udp_rx_conn* conn = connect_udp_rx();
    MTRand mt = seedRand(TWISTER_SEED);

    // Receive packets and measure receive rate and bit errors
    unsigned long** data = calloc(1000, PACKET_SIZE);
    unsigned long** mseq = calloc(1000, PACKET_SIZE);
    int packets = 0;
    int seq_length = 0;
    int pkt_drops = 0;
    int bit_errors = 0;
    long start;

    while(1){

        // Receive packet data and generate comparison sequence
        data[seq_length] = recv_datagram(conn);
        mseq[seq_length] = generate_payload(&mt);
        seq_length++;

        // Start timing upon receiving the first packet
        if(packets == 0){
            start = get_timestamp();
        }
        //bit_errors += count_bit_errors(data, &mt);
        packets++;

        // Track RX data in intervals
        // Every 1000 packets, check for packet drops
        // If all packets have been received, then check for bit errors among them
        if(seq_length == sl){
            double dt = (double)(get_timestamp() - start) / 1000000;
            int drops_in_seq = sl - packet_lcs(data, mseq, sl, sl);
            pkt_drops += drops_in_seq;
            // Since we essentially "skipped" drops_in_seq packets from the Mersenne sequence,
            // generate that number of dummy payloads to offset the missing packets
            for(int i = 0; i < drops_in_seq; i++){
                generate_payload(&mt);
            }
            seq_length = 0;
            printf("Packets Received: %d Packet Drops Detected: %d Time Elapsed: %.5f\n",
                packets, pkt_drops, dt);
        }

    }
}

int main(int argc, char** argv){
    rx_args args = DEFAULT_RX_ARGS;
    parse_args(argc, argv, &args);
    run(args.seq_length);
    return 0;
}