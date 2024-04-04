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
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/shm.h>
#include <signal.h>

#include "reverse.h"

#define KEY 217 // SHM key

// pointer to shared memory
char* mem;

// id of shared memory
int shmid;

void stop_signal_handler(int signal) {

    printf("\nServer is stopping...\n");

    // Detach shared memory
    if (shmdt(mem) == -1)
    {
        perror("shmdt");
        exit(1);
    }

    // Delete shared memory
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
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

    // create shared memory
    shmid = shmget((key_t)KEY, 1024, IPC_CREAT | 0666);

    // attach shared memory
    mem = shmat(shmid, NULL, 0);

    // check if shared memory is attached
    if (mem == (char *)-1)
    {
        perror("shmat");
        exit(1);
    }

    // register signal handler
    signal(SIGINT, stop_signal_handler);

    printf("Server started. Enjoy !\n");

    while (1)
    {
        // check if shared memory is empty
        if (strlen(mem) > 0)
        {
            // reverse string
            reverse_string(mem);

            // print reversed string
            printf("Reversed string: %s\n", mem);

            // empty shared memory
            memset(mem, 0, 1024);
        }
    }

    return 0;
}
