#include <netinet/in.h>

/* Struct containing socket and server address info for a connection */
typedef struct udp_tx_conn{
    int socket;
    struct sockaddr_in servaddr;
} udp_tx_conn;

udp_tx_conn* connect_udp_tx(long servaddr, int port);
int udp_send(void* msg, int msglen, udp_tx_conn* conn);
int disconnect_udp_tx(udp_tx_conn* conn);
