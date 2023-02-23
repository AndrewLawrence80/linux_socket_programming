#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8000
#define SIZE_BUFFER 1024

int main()
{
    int server_sockfd = -1;
    struct sockaddr_in server_addr, client_addr;

    // create socket
    // SOCK_DGRAM denotes UDP
    server_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
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

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    if (bind(server_sockfd, (const struct sockaddr *)&server_addr, (socklen_t)sizeof(server_addr)) < 0)
    {
        perror("error when binding server address to socket");
        exit(EXIT_FAILURE);
    }

    ssize_t val_recv = 0, val_send = 0;
    char buffer[SIZE_BUFFER] = {0};
    const char *msg = "A message from server";
    socklen_t socklen_client_addr = (socklen_t)sizeof(client_addr);
    while (true)
    {
        val_recv = recvfrom(server_sockfd, buffer, SIZE_BUFFER, 0, (struct sockaddr *)&client_addr, (socklen_t *)&socklen_client_addr);
        if (val_recv < 0)
        {
            perror("error when receiving from client");
            exit(EXIT_FAILURE);
        }
        printf("received msg from client %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        printf("%s\n", buffer);
        val_send = sendto(server_sockfd, msg, strlen(msg) * sizeof(char), 0, (const struct sockaddr *)&client_addr, socklen_client_addr);
        if (val_send < 0)
        {
            perror("error when sending to client");
            exit(EXIT_FAILURE);
        }
        printf("sending msg to client %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    }
    return 0;
}