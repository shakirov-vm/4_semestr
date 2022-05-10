#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <netinet/ip.h>
#include <errno.h>

int main(int argc, char** argv) {

	int err;

	int sk = socket(PF_UNIX, SOCK_STREAM, 0);
	if (sk == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

    memset(sendBuff, '0', sizeof(sendBuff));

    struct sockaddr_in serv_addr;
    memset(&serv_addr, '0', sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // ?
    
    err = bind(sk, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	if (err == -1) {
		perror("bind");
		exit(EXIT_FAILURE);
	}

    err = listen(sk, 255);
	if (err == -1) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	int connect = 0;
    char sendBuff[256];
    sprintf(sendBuff, "Hello client!\n");

    while(1) {

        connect = accept(sk, (struct sockaddr*)NULL, NULL);

        write(connect, sendBuff, strlen(sendBuff));

        close(connect);
        sleep(1);
    }
}