#include "util.h"

int initialiseShmSegment(int **shm)
{
  int shmid;
  /*
   * Create the shared memory segment.
   */
  if ((shmid = shmget(IPC_PRIVATE, SHMSZ, IPC_CREAT | 0666)) < 0)
  {
    perror("shmget");
    exit(EXIT_FAILURE);
  }

  /*
   * Now we attach the segment to our data space.
   */
  if ((*shm = shmat(shmid, NULL, 0)) == (int *) -1)
  {
    perror("shmat");
    exit(EXIT_FAILURE);
  }

  return shmid;
}

void releaseSegment(int shmid, int **shm)
{
  if (shmdt(*shm))
  {
    perror("detatch failed");
  }

  shmctl(shmid, IPC_RMID, NULL);
}

/*
 * This function is called by each process with possibly different arguments
 * At the moment, this function simply calculates prime numbers and then return the processNumber * 7
 */
void processWork(int *shm, int processNumber)
{
  clock_t begin, end;
  double time_spent;
  begin = clock();
  unsigned long i, num = 0;
  int primes = 0;
  for (num = 1; num <= MAX_PRIME; ++num) {
    for (i = 2; (i <= num) && (num % i != 0); ++i);
    if (i == num)
      ++primes;
  }
  fprintf(stderr, "CHILD%d : %d primes calculated!\n", processNumber, primes);
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  fprintf(stderr, "CHILD%d : Time spent : %lf\n", processNumber, time_spent);

  int r = processNumber * 7;

  fprintf(stderr, "CHILD%d : Returning integer %d to parent..\n", processNumber, r);

  shm[processNumber] = r;

  exit(EXIT_SUCCESS);
}

/*
 * Attempts to get the current time
 */
void getTime(struct timespec *time)
{
  if(clock_gettime(CLOCK_MONOTONIC, time) == -1) {
    perror("ERROR : Clock error..");
    exit(EXIT_FAILURE);
  }
}