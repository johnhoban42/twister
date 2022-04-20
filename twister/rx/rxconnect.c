#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/constants.h"
#include "../rx/rxconnect.h"

udp_rx_conn* connect_udp_rx(){   
    
    udp_rx_conn* conn = malloc(sizeof(udp_rx_conn));

    // We can largely ignore the servaddr socket since we won't be sending data back to TX
    memset(&(conn->servaddr), 0, sizeof((conn->servaddr)));
  
    // Create a UDP Socket
    conn->socket = socket(AF_INET, SOCK_DGRAM, 0);        
    conn->servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    conn->servaddr.sin_port = htons(PORT);
    conn->servaddr.sin_family = AF_INET; 
   
    // bind server address to socket descriptor
    bind(conn->socket, (struct sockaddr*)&(conn->servaddr), sizeof(conn->servaddr));

    return conn;

}

/*
Receive a datagram over UDP. For now, ignore the sender address.
*/
int udp_recv(void* buf, int buflen, udp_rx_conn* conn){
    return recv(conn->socket, buf, buflen, 0);
}
