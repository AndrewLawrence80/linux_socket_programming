#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define SIZE_BUFFER 1024
int main(int argc, const char *argv[])
{
    int client_sockfd = -1;
    struct sockaddr_in server_addr;
    // if (argc != 3)
    // {
    //     perror("error when parsing server address and port ");
    //     exit(EXIT_FAILURE);
    // }
    const char *SERVER_IP = "127.0.0.1";
    const int SERVER_PORT = 8000;
    client_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_sockfd < 0)
    {
        perror("error when creating socket");
        exit(EXIT_FAILURE);
    }
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    printf("%s\n", SERVER_IP);
    printf("%d\n", SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) < 0)
    {
        perror("error when assigning server ip");
        exit(EXIT_FAILURE);
    }
    ssize_t val_recv = 0, val_send = 0;
    char buffer[SIZE_BUFFER] = {0};
    socklen_t socklen_server_addr = (socklen_t)sizeof(server_addr);
    const char *msg = "A message from client";
    while (true)
    {
        val_send = sendto(client_sockfd, msg, strlen(msg) * sizeof(char), MSG_CONFIRM, (const struct sockaddr *)&server_addr, socklen_server_addr);
        if (val_send < 0)
        {
            perror("error when sending to server");
            exit(EXIT_FAILURE);
        }
        val_recv = recvfrom(client_sockfd, buffer, SIZE_BUFFER, 0, (struct sockaddr *)&server_addr, &socklen_server_addr);
        if (val_recv < 0)
        {
            perror("error when receiving from server");
            exit(EXIT_FAILURE);
        }
        printf("%s\n",buffer);
        sleep(1);
    }
    return 0;
}