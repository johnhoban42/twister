#include <netinet/in.h>

/* Struct containing socket and server address info for a connection */
typedef struct udp_rx_conn{
    int socket;
    struct sockaddr_in servaddr;
} udp_rx_conn;

/* Struct containing RX metrics */
typedef struct rx_metrics{
    int pkt_received;
    int pkt_drops;
    float time_elapsed;
} rx_metrics;

#define INIT_RX_METRICS {.pkt_received = 0, .pkt_drops = 0, .time_elapsed = 0.0}

udp_rx_conn* connect_udp_rx(int port);
int udp_recv(void* buf, int buflen, udp_rx_conn* conn);
int disconnect_udp_rx(udp_rx_conn* conn);
