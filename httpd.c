
/*
    http server

    COMPATIBILITY NOTES
        Linux w/ glibc:         "it works on my machine"
        Linux w/ other libcs:   probably
        Other Unixes:           likely
        Win32/mingw:            hell no

    COMPILING
        Build with
            cc -std=c99 -Wall -Wextra -Wpedantic -Werror httpd.c

    TODOS
        TODO FEAT [1] Build response automatically and read from index.html
        TODO FEAT [2] use getopt to get port and listen address

    made with ♡ by heliumhydride
*/

#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define ANSI_RED   "\033[1;31m"
#define ANSI_GREEN "\033[1;32m"
#define ANSI_BLUE  "\033[1;34m"
#define ANSI_NORM  "\033[0m"

#define MAX_MSG_SIZE 4096
#define FILENAME "additional_files/httpd/index.html"
#define PORT 80
#define LISTEN_ADDR "127.0.0.1"

static char* program;
void perror2(const char* s)
{
    printf(ANSI_RED "%s: ", program);
    perror(s);
    printf(ANSI_NORM);
}

static int running = true;
void ctrl_c(int unused)
{
    (void)unused;
    running = false;
}

int main(int argc, char* argv[])
{
    program = argv[0];
    (void)argc; // [2]
 
    setbuf(stderr, NULL);
    setbuf(stdout, NULL);
    signal(SIGINT, ctrl_c);
    signal(SIGTERM, ctrl_c);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1) {
        perror2("socket");
        return 1;
    }
  
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(LISTEN_ADDR);
  
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if(bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror2("bind");
        return 1;
    }
  
    if(listen(sockfd, 1) < 0) {
        perror2("listen");
        close(sockfd);
        return 1;
    }

    puts(ANSI_GREEN "heliumhydride's http server" ANSI_NORM);
    printf(ANSI_BLUE "listening on address %s on port %d\n" ANSI_NORM, LISTEN_ADDR, PORT);

    int excode = 0;
    struct sockaddr_in clientaddr;
    while(running) {
        uint32_t clientaddr_size = sizeof(clientaddr);
        int clientsock = accept(sockfd, (struct sockaddr*)&clientaddr, &clientaddr_size);
        if(clientsock < 0) {
            perror2("accept");
            close(clientsock);
            excode = 1;
            break;
        }

        printf(ANSI_BLUE "%s: connected\n" ANSI_NORM, inet_ntoa(clientaddr.sin_addr));

        char client_msg[MAX_MSG_SIZE] = {0};
        if(recv(clientsock, client_msg, MAX_MSG_SIZE, 0) == -1) {
            perror2("recv");
            close(clientsock);
            excode = 1;
            break;
        }

        printf(ANSI_BLUE "%s: received\n" ANSI_NORM, inet_ntoa(clientaddr.sin_addr));
        printf("%s", client_msg);

        // [1]
        char server_msg[MAX_MSG_SIZE] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 1\r\nConnection: close\r\n\r\nX";
        if(send(clientsock, server_msg, MAX_MSG_SIZE, 0) == -1) {
            perror2("send");
            close(clientsock);
            excode = 1;
            break;
        }

        close(clientsock);
    }
  
    puts(ANSI_BLUE "kthxbye" ANSI_NORM);
    close(sockfd);
    puts(ANSI_BLUE "closed socket" ANSI_NORM);
    return excode;
}
