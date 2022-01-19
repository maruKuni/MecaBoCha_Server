#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[]){
    int sockfd, len;
    char buf[BUFSIZ];
    struct sockaddr_in serv;
    int port;
    if(argc != 3){
        printf("Usage ./prog host port\n");
        exit(1);
    }
    if((sockfd = socket(PF_INET, SOCK_STREAM,0)) < 0){
        perror("socket");
        exit(1);
    }

    serv.sin_family = PF_INET;// サーバの
    port = strtol(argv[2], NULL, 10);
    serv.sin_port = htons(port);
    inet_aton(argv[1], &serv.sin_addr);

    if(connect(sockfd, (struct sockaddr *) &serv, sizeof(serv)) < 0){
        perror("connect");
        exit(1);
    }

    while(strncasecmp(buf, "exit\n", 5) != 0){
        printf("==> ");
        if((fgets(buf, BUFSIZ, stdin)) <0){
            perror("fgets");
            exit(1);
        }
        len = send(sockfd, buf, strlen(buf), 0);
        len = recv(sockfd, buf, len, 0);
        while (len > 1)
        {
            
        }
        
    }
    close(sockfd);
    return 0;
}