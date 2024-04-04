/**
 * @file main.c
 * @author Quentin Rollet (rolletquen@gmail.com)
 * @brief A client that sends a string to a server which will reverse it
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

#define OUTPUT_KEY 217 // SHM key
#define INPUT_KEY 218 // SHM key

// structure to send data to the server
typedef struct
{
    int pid;
    char string[1024];
} Data;

int main(int argc, char const *argv[])
{
    // check the number of arguments
    if (argc != 2)
    {
        printf("Usage: %s <string>\n", argv[0]);
        exit(1);
    }

    // declare client data
    Data client_data;
    Data server_data;

    // get the pid of the client
    client_data.pid = getpid();

    // get the string to send
    strcpy(client_data.string, argv[1]);

    // pointer to shared memory
    char* output_mem;
    char* input_mem;

    // id of shared memory
    int output_shmid;
    int input_shmid;

    // create shared memory
    output_shmid = shmget((key_t)OUTPUT_KEY, 0, 0);
    input_shmid = shmget((key_t)INPUT_KEY, 0, 0);

    // attach shared memory
    output_mem = shmat(output_shmid, NULL, 0);
    input_mem = shmat(input_shmid, NULL, 0);

    // copy data to shared memory
    memcpy(output_mem, &client_data, sizeof(Data));

    int response_received = 0;
    while (!response_received)
    {
        // check if server has written in shared memory
        if (strlen(input_mem) > 0)
        {
            memcpy(&server_data, input_mem, sizeof(Data));

            printf("Received response from server %d: %s\n", server_data.pid, server_data.string);
            response_received = 1;

            // empty shared memory
            memset(input_mem, 0, sizeof(Data));
        }
    }

    return 0;
}