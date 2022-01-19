#ifndef MECABSERVER_H_
#define MECABSERVER_H_
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
typedef struct CLIENT{
    int sockfd;
}CLIENT;
void session(CLIENT *);
#endif