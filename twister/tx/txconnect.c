#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/constants.h"
#include "../tx/txconnect.h"
   
/*
Instantiate a UDP TX socket and wait to connect to RX.
Largely adapted from example code found at https://www.geeksforgeeks.org/udp-client-server-using-connect-c-implementation/
*/
udp_tx_conn* connect_udp_tx(long servaddr){   

    udp_tx_conn* conn = malloc(sizeof(udp_tx_conn));
      
    // clear servaddr
    memset(&(conn->servaddr), 0, sizeof((conn->servaddr)));
    conn->servaddr.sin_addr.s_addr = htonl(servaddr);
    conn->servaddr.sin_port = htons(PORT);
    conn->servaddr.sin_family = AF_INET;
      
    // create datagram socket
    conn->socket = socket(AF_INET, SOCK_DGRAM, 0);
      
    // connect to server
    if(connect(conn->socket, (struct sockaddr *)&(conn->servaddr), sizeof((conn->servaddr))) < 0)
    {
        printf("\n Error : Connect Failed \n");
        exit(0);
    }
  
    return conn;

}

/*
Send a datagram over UDP.
*/
int udp_send(void* msg, int msglen, udp_tx_conn* conn){
    return sendto(conn->socket, msg, msglen, 0, (struct sockaddr*)NULL, sizeof(conn->servaddr));
}

/*
Disconnect from a UDP TX socket.
*/
int disconnect_udp_tx(udp_tx_conn* conn){
    close(conn->socket);
    free(conn);
    return 0;
}
