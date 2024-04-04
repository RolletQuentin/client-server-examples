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

#define KEY 217 // SHM key

int main(int argc, char const *argv[])
{
    // pointer to shared memory
    char* mem;

    // id of shared memory
    int shmid;

    // create shared memory
    shmid = shmget((key_t)KEY, 0, 0);

    // attach shared memory
    mem = shmat(shmid, NULL, 0);

    // copy string to shared memory
    strcpy(mem, "Hello World!");

    return 0;
}