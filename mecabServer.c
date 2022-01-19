#include "mecabServer.h"
int main(int argc, char *argv[]){
    int sock_fd, new_sockfd;
    struct sockaddr_in server, clnt;
    socklen_t sin_siz;
    int port;
    CLIENT *client = NULL;
    pthread_t tid;

    if(argc != 3){
        printf("./server_main host port\n");
        exit(1);
    }

    if((sock_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket");
        exit(1);
    }
    
    server.sin_family = PF_INET;
    port = strtol(argv[2], NULL, 10);
    server.sin_port = htons(port);
    inet_aton(argv[1], &server.sin_addr);
    sin_siz = sizeof(struct sockaddr_in);
    if (bind(sock_fd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("bind");
        exit(1);
    }

    if (listen(sock_fd, SOMAXCONN) < 0)
    {
        perror("listen");
        exit(1);
    }
    while(1){
        if((new_sockfd = accept(sock_fd,(struct sockaddr *)&clnt, &sin_siz)) < 0){
            perror("accept");
            exit(1);
        }
        client = (CLIENT *)malloc(sizeof(CLIENT));
        client->sockfd = new_sockfd;
        pthread_create(&tid, NULL, (void *)session, client);
        pthread_detach(tid);
        client = NULL;
    }
}
void session(CLIENT *client){
    int PtoC[2];// 親から子へ
    int CtoP[2];// 子から親へ
    char fromClient[BUFSIZ];
    char fromMeCab[BUFSIZ];
    long msglenFromClient;
    long msglenFromMecab;
    pid_t pid;
    if(pipe(PtoC) < 0){
        perror("pipe:PtoC");
        exit(1);
    }
    if(pipe(CtoP) < 0){
        perror("pipe:CtoP");
        exit(1);
    }
    pid = fork();
    if(pid == 0){
        // 子プロセス
        printf("mecab\n");
        close(client->sockfd);
        close(CtoP[0]);
        close(PtoC[1]);
        close(STDIN_FILENO);
        dup(PtoC[0]);
        close(STDOUT_FILENO);
        dup(CtoP[1]);
        execlp("mecab", " ", NULL);
    }else if(pid > 0){
        close(PtoC[0]);
        close(CtoP[1]);
        while(1){
            msglenFromClient = recv(client->sockfd, fromClient, BUFSIZ, 0);
            printf("recv:%ld\n", msglenFromClient);
            if(msglenFromClient == 0){
                break;
            }
            msglenFromClient =  write(PtoC[1], fromClient, msglenFromClient);
            printf("write:%ld", msglenFromClient);
            msglenFromMecab =  read(CtoP[0], fromMeCab, BUFSIZ);
            send(client->sockfd, fromMeCab, msglenFromMecab, 0);
            printf("send\n");
        }
        close(PtoC[1]);
        close(CtoP[0]);
        close(client->sockfd);
        free(client);
        kill(pid, SIGKILL);

    }else{
        perror("fork");
        exit(1);
    }
}