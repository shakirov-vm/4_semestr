#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

void client_init(int num_threads);

int main(int argc, char **argv) {

    int num_threads = atoi(argv[1]);

    if (num_threads > 0 && errorState == 0) {
        while (1) client_init(num_threads);
    }
    else printf("Incorrect num_threads\n");
    return 0;
}

void client_init(int noThreads) {

    int err = 0;

    struct sockaddr_in serv_addr;
    struct sockaddr_in peer_addr;
    struct sockaddr_in sock_addr;

    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(&peer_addr, '0', sizeof(peer_addr));
    memset(&sock_addr, '0', sizeof(sock_addr));

    socklen_t peer_addrLen = sizeof(peer_addr);

    sock_addr.sin_family      = AF_INET;
    sock_addr.sin_port        = htons(CL_PORT);
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* Initialization */

    int bcsk = socket(AF_INET, SOCK_DGRAM, 0);

    int nonZero = 1;

    err = setsockopt(bcsk, SOL_SOCKET, SO_REUSEADDR, &nonZero, sizeof(nonZero));
    if (err != 0) perror("setsockopt bsck");

    err = bind(bcsk, (struct sockaddr *) &sock_addr, sizeof(sock_addr));
    if (err < 0) perror("bind bsck");

    int servTcpPort = 0;

    printf("Start recieve\n");
    printf("Waiting for server\n");

    recvfrom(bcsk, &servTcpPort, sizeof (int), 0, (struct sockaddr *) &peer_addr, &peer_addrLen);
    
    if (servTcpPort != SERV_PORT) return;

    close(bcsk);

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port   = htons(SERV_PORT);
    serv_addr.sin_addr   = peer_addr.sin_addr;

    socklen_t serv_addrLen = sizeof(serv_addr);

    printf("serv addr = %s\n", inet_ntoa(peer_addr.sin_addr));

    int sockFd = socket(AF_INET, SOCK_STREAM, 0);
}
