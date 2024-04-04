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
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/shm.h>

#define OUTPUT_KEY 217 // SHM key
#define INPUT_KEY 218 // SHM key

int main(int argc, char const *argv[])
{
    // check the number of arguments
    if (argc != 2)
    {
        printf("Usage: %s <string>\n", argv[0]);
        exit(1);
    }

    // get the string to send
    char* string = (char*)argv[1];

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

    // copy string to shared memory
    strcpy(output_mem, string);

    int response_received = 0;
    while (!response_received)
    {
        // check if server has written in shared memory
        if (strlen(input_mem) > 0)
        {
            printf("Server response: %s\n", input_mem);
            response_received = 1;

            // empty shared memory
            memset(input_mem, 0, strlen(input_mem));
        }
    }

    return 0;
}