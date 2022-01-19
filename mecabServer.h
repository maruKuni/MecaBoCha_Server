#ifndef MY_UTIL_H
#define MY_UTIL_H
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
typedef struct CLIENT{
    struct sockaddr_in info;
    int sockfd;
}CLIENT;
void session(CLIENT *);
#endif