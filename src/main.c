#include "util.h"

/*
 * Here is the sources which was used to put it together : (credits goes to them)
 * https://gist.github.com/gaocegege/ff591e7aaa1298285bd5
 * http://www.cs.cf.ac.uk/Dave/C/node27.html
 */

int main()
{
  int shmid;
  int *shm;

  struct timespec start, stop;
  double accum;

  getTime(&start);

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
  if ((shm = shmat(shmid, NULL, 0)) == (int *) -1)
  {
    perror("shmat");
    exit(EXIT_FAILURE);
  }


  /*
   * Forking processes with different threadNumber arguments
   */
  pid_t pids[NUM_OF_CORES];
  for (int i = 0; i < NUM_OF_CORES; i++)
  {
    if (!(pids[i] = fork()))
    {
      processWork(shm, i);
    }
    if (pids[i] < 0)
    {
      perror("Fork");
      exit(EXIT_FAILURE);
    }
  }

  /*
   *  Waiting for all processes to return
   */
  for (int i = 0; i < NUM_OF_CORES; ++i)
  {
    waitpid(pids[i], NULL, 0);
  }


  for (int i = 0; i < NUM_OF_CORES; i++)
  {

    fprintf(stderr, "PARENT : received %d from child %d\n", shm[i], i);

    /*
     * Checking solutions against processNumber * 7
     */
    if (shm[i] != i * 7)
      fprintf(stderr, "ERROR : Multi threading / Shared memory error\n");
  }

  getTime(&stop);

  accum = ( stop.tv_sec - start.tv_sec )
        + ( stop.tv_nsec - start.tv_nsec )
          / 1000000000.0;

  fprintf(stderr, "PARENT : Time spent : %lf\n", accum);


  if (shmdt(shm))
  {
    perror("detatch failed");
  }

  shmctl(shmid, IPC_RMID, NULL);


  return 0;
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
  if( clock_gettime( CLOCK_MONOTONIC, time) == -1 ) {
    perror( "ERROR : Clock error.." );
    exit(EXIT_FAILURE);
  }
}