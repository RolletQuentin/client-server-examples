/**
 * @file main.c
 * @author Quentin Rollet (rolletquen@gmail.com)
 * @brief Server to make operation on two numbers
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
#include <asm-generic/socket.h>

#include "functions.h"


#define PORT 8080

int main(int argc, char const *argv[])
{
    printf("Server is starting...\n");

    // create socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // set socket options
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // bind socket to port
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // listen
    if (listen(server_fd, 3) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Connection established\n");

    while (1) {
        // accept
        int addrlen = sizeof(address);
        int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (new_socket < 0)
        {
            perror("accept failed");
            continue;
        }

        // read
        char buffer[1024] = {0};
        read(new_socket, buffer, 1024);
        printf("Client: %s\n", buffer);

        char response[1024] = {0};

        // extract operation
        char* operation = extract_operation(buffer);
        double result;
        if (operation == NULL)
        {
            sprintf(response, "Invalid operation");
        } else {
            // calculate
            result = calculate(operation);

            // make response
            sprintf(response, "%f", result);
        }

        // send
        send(new_socket, response, strlen(response), 0);

        // close the new socket
        close(new_socket);
    }

    return 0;
}

