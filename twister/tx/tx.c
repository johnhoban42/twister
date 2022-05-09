#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "../tx/txconnect.h"
#include "../tx/cli.h"
#include "../mtwister/mtwister.h"
#include "../utils/constants.h"
#include "../utils/utils.h"


/* Send a burst of n packets containing random payloads. */
int send_burst(int n, udp_tx_conn* conn, MTRand* mt){
    for(int i = 0; i < n; i++){
        unsigned long* data = generate_payload(mt);
        int u = udp_send(data, PACKET_SIZE, conn);
    }
    return 0;
}

/* Run the channel test. Rate parameter controls send rate in packets per millisecond. */
int run(tx_args args){
    // Establish connection and random seed
    printf("Creating socket binding for RX...\n");
    udp_tx_conn* conn = connect_udp_tx(args.servaddr, args.port);
    printf("Transmitting data to RX...\n");
    MTRand mt = seedRand(TWISTER_SEED);
    
    long start = get_timestamp();
    long send_time_elapsed = 0;
    double pkt_remainder = 0.0;

    // 1 Mb/s = 125/PACKET_SIZE pkt/ms
    // For PACKET_SIZE = 1600 B, 1 Mb/s = 0.078125 pkt/ms
    double pkt_rate = args.rate * 125.0 / PACKET_SIZE;
    
    // Control send rate in bursts - send approx "rate" packets every millisecond

    // First pass - send rate/2 packets, then sleep
    int first_burst_size = (int)floor(pkt_rate / 2);
    send_burst(first_burst_size / 2, conn, &mt);
    int packets = first_burst_size;

    while(1){
        long t1 = get_timestamp();
        int sleep_time = 500 - send_time_elapsed > 0 ? 500 - send_time_elapsed : 0;
        usleep(sleep_time);
        long t2 = get_timestamp();
        
        // Set burst size proportional to the actual amount of time slept
        // Since this results in a fraction, accumulate "remainders" across bursts until they equal a full packet
        double burst_size_int;
        double burst_size_dbl = (pkt_rate / 2)*(t2 - t1 + send_time_elapsed)/500;
        pkt_remainder += modf(burst_size_dbl, &burst_size_int);
        int burst_size = (int)burst_size_int;
        if(pkt_remainder >= 1){
            burst_size++;
            pkt_remainder -= 1;
        }

        // Send the burst, then check how much time since t2 has passed and subtract it from the next sleep time
        send_burst(burst_size, conn, &mt);
        packets += burst_size;
        send_time_elapsed = get_timestamp() - t2;
    }
    disconnect_udp_tx(conn);
}

/* Execute run(), parametrizable by Mb/s and hex send address. */
int main(int argc, char** argv){
    tx_args args = DEFAULT_TX_ARGS;
    parse_args(argc, argv, &args);
    run(args);
    return 0;
}