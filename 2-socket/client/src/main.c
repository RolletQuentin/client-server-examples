/**
 * @file main.c
 * @author Quentin Rollet (rolletquen@gmail.com)
 * @brief Client to send request to server
 * @version 0.1
 * @date 2024-04-05
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

int main(int argc, char const *argv[])
{
    // create socket
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // set server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0)
    {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }

    // connect to server
    if (connect(client_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    // send request
    char buffer[1024] = {0};
    char *request = "Can you give me the result of -3 * 7 ?";
    send(client_fd, request, strlen(request), 0);
    printf("Request sent: %s\n", request);

    // receive response
    read(client_fd, buffer, 1024);
    printf("Response received: %s\n", buffer);

    // close socket
    close(client_fd);

    return 0;
}
