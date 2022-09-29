#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 28000
#define SIZE_LISTEN_QUEUE 8
#define SIZE_BUFFER 1024
int main()
{
    struct sockaddr_in server_addr, client_addr;
    int server_sockfd = -1, client_sockfd = -1;

    // create socket
    // int socket (int __domain, int __type, int __protocol)
    // __domain AF_INET, AF_INET6, AF_LOCAL
    // __type SOCK_STREAM for tcp, SOCK_DGRAM for udp
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sockfd < 0)
    {
        perror("error when creating socket");
        exit(EXIT_FAILURE);
    }

    // optional
    // set socket option
    int optval = 1;
    if (setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (const void *)&optval, (socklen_t)sizeof(optval)) < 0)
    {
        perror("error when setting socket option");
        exit(EXIT_FAILURE);
    }

    // set server address
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = PORT;

    // bind server address to socket file descriptor
    if (bind(server_sockfd, (const struct sockaddr *)&server_addr, (socklen_t)sizeof(server_addr)) < 0)
    {
        perror("error when binding server address");
        exit(EXIT_FAILURE);
    }

    // listen connection
    if (listen(server_sockfd, SIZE_LISTEN_QUEUE) < 0)
    {
        perror("error when listening connection request");
        exit(EXIT_FAILURE);
    }

    socklen_t socklen_client_addr = (socklen_t)sizeof(client_addr);
    client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_addr, &socklen_client_addr);
    if (client_sockfd < 0)
    {
        perror("error when accepting connection");
        exit(EXIT_FAILURE);
    }
    printf("get connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohl(client_addr.sin_port));
    if (client_sockfd < 0)
    {
        perror("error when accepting connection");
        exit(EXIT_FAILURE);
    }

    char buffer[SIZE_BUFFER] = {0};
    ssize_t val_recv = recv(client_sockfd, buffer, SIZE_BUFFER, 0);
    if (val_recv < 0)
    {
        perror("error when receiving from client");
        exit(EXIT_FAILURE);
    }
    while (val_recv)
    {
        printf("%s\n", buffer);
        const char *msg = "A message from server";
        ssize_t val_send = send(client_sockfd, msg, sizeof(msg), 0);
        if (val_send < 0)
        {
            perror("loss connection to client");
            close(client_sockfd);
        }
        val_recv = recv(client_sockfd, buffer, SIZE_BUFFER, 0);
    }
    close(client_sockfd);

    return 0;
}