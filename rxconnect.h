#include <netinet/in.h>

/* Struct containing socket and server address info for a connection */
typedef struct udp_rx_conn{
    int socket;
    struct sockaddr_in servaddr;
} udp_rx_conn;

udp_rx_conn* connect_udp_rx();
int udp_recv(void* buf, int buflen, udp_rx_conn* conn);
int disconnect_udp_rx(udp_rx_conn* conn);
