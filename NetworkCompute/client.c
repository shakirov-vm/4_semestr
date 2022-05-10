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

int main(int argc, char *argv[]) {

    int sk = socket(AF_INET, SOCK_STREAM, 0);
   	if (sk == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

    struct sockaddr_in serv_addr; 
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);
 
    inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);

 	connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    char recvBuff[1024];
    memset(recvBuff, '0',sizeof(recvBuff));

    while (1) {

    	n = read(sockfd, recvBuff, sizeof(recvBuff) - 1);
    	if (n < 0) break;

        recvBuff[n] = 0;

        printf("that - <%s>\n", recvBuff);
    }

  	if (n < 0) printf("\n Read error \n");

  	return 0;
}