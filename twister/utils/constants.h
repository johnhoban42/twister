#ifndef CONSTANTS_H
#define CONSTANTS_H

/* Mersenne Twister constants */
#define TWISTER_SEED 0x836d6f21

/* Networking constants */
#define PACKET_SIZE 1200
#define BLOCKS_PER_PACKET PACKET_SIZE/sizeof(unsigned long)

/* Trace constants */
#define PKT_RECV "0\n"
#define PKT_DROP "1\n"

#endif