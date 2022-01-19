#ifndef MY_UTIL_H
#define MY_UTIL_H
#include <arpa/inet.h>
typedef struct CLIENT{
    struct sockaddr_in info;
    int sockfd;
}CLIENT;
void session(CLIENT *);
#endif