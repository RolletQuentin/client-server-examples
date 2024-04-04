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

#define KEY 217 // SHM key

/**
 * @brief Reverse a string
 * 
 * @param str 
 * @return char* 
 */
char *reverse_string(char *str)
{
    int i = 0;
    int j = 0;
    char tmp;

    while (str[j] != '\0')
        j++;
    j--;

    while (i < j)
    {
        tmp = str[i];
        str[i] = str[j];
        str[j] = tmp;
        i++;
        j--;
    }

    return str;
}

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

    // check if shared memory is attached
    if (mem == (char *)-1)
    {
        perror("shmat");
        exit(1);
    }

    printf("Server is running...\n");

    // detach shared memory
    shmdt(mem);

    // delete shared memory
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
