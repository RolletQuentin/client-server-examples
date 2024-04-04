/**
 * @file main.c
 * @author Quentin Rollet (rolletquen@gmail.com)
 * @brief Server which reverse a string he receives from a client
 * @version 0.1
 * @date 2024-04-04
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/shm.h>
#include <signal.h>

#include "reverse.h"

#define INPUT_KEY 217 // SHM key
#define OUTPUT_KEY 218 // SHM key

// structure to send data to the server
typedef struct
{
    int pid;
    char string[1024];
} Data;

// pointer to shared memory
char* input_mem;
char* output_mem;

// id of shared memory
int input_shmid;
int output_shmid;

void stop_signal_handler(int signal) {

    printf("\nServer is stopping...\n");

    // Detach shared memory
    if (shmdt(input_mem) == -1)
    {
        perror("shmdt");
        exit(1);
    }

    if (shmdt(output_mem) == -1)
    {
        perror("shmdt");
        exit(1);
    }

    // Delete shared memory
    if (shmctl(input_shmid, IPC_RMID, NULL) == -1)
    {
        perror("shmctl");
        exit(1);
    }

    if (shmctl(output_shmid, IPC_RMID, NULL) == -1)
    {
        perror("shmctl");
        exit(1);
    }

    printf("Shared memory deleted\n");
    printf("Server stopped. See you next time :)\n");

    exit(0);
}

int main(int argc, char const *argv[])
{
    printf("Server is starting...\n");

    // declare server and client data
    Data server_data;
    Data client_data;

    // get the pid of the server
    server_data.pid = getpid();

    // create shared memory
    input_shmid = shmget((key_t)INPUT_KEY, 1024, IPC_CREAT | 0666);
    output_shmid = shmget((key_t)OUTPUT_KEY, 1024, IPC_CREAT | 0666);

    // attach shared memory
    input_mem = shmat(input_shmid, NULL, 0);
    output_mem = shmat(output_shmid, NULL, 0);

    // check if shared memory is attached
    if (input_mem == (char *)-1 || output_mem == (char *)-1)
    {
        perror("shmat");
        exit(1);
    }

    // register signal handler
    signal(SIGINT, stop_signal_handler);

    printf("Server started. Enjoy !\n\n");

    while (1)
    {
        // check if shared memory is empty
        if (strlen(input_mem) > 0)
        {
            // get the pid of the client
            memcpy(&client_data, input_mem, sizeof(Data));

            // print message received from client
            printf("Received message from client %d: %s\n", client_data.pid, client_data.string);

            // reverse string
            reverse_string(client_data.string);
            strcpy(server_data.string, client_data.string);

            // send the reversed string to the client
            printf("Sending reversed message to client %d: %s\n", server_data.pid, server_data.string);
            memcpy(output_mem, &server_data, sizeof(Data));

            // empty shared memory
            memset(input_mem, 0, sizeof(Data));
        }
    }

    return 0;
}
