#ifndef UTIL_H
#define UTIL_H

#define _SVID_SOURCE
#define _POSIX_C_SOURCE 199309L
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_OF_CORES 4
#define SHMSZ (sizeof(int)*8388608)
#define MAX_PRIME 100000

void processWork(int *shm, int processNumber);
void releaseSegment(int shmid, int **shm);
int initialiseShmSegment(int **shm);
void getTime(struct timespec *time);

#endif