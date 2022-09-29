#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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
    const char *SERVER_IP = "localhost";
    const int SERVER_PORT = 28000;
    client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
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
    if (connect(client_sockfd, (const struct sockaddr *)&server_addr, (socklen_t)sizeof(server_addr)) < 0)
    {
        perror("error when connecting to server");
        exit(EXIT_FAILURE);
    }
    char buffer[SIZE_BUFFER] = {0};
    const char *msg = "A message from client";
    int val_send = send(client_sockfd, msg, sizeof(msg), 0);
    if (val_send < 0)
    {
        perror("error when sending to server");
        exit(EXIT_FAILURE);
    }
    while (val_send)
    {
        int val_recv = recv(client_sockfd, buffer, SIZE_BUFFER, 0);
        if (val_recv < 0)
        {
            perror("loss connection to server");
            exit(EXIT_FAILURE);
        }
        printf("%s\n", buffer);
        sleep(1);
        val_send = send(client_sockfd, msg, sizeof(msg), 0);
    }
    close(client_sockfd);
    return 0;
}