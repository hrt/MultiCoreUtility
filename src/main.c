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

  shmid = initialiseShmSegment(shm);

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

  accum = (stop.tv_sec - start.tv_sec)
        + (stop.tv_nsec - start.tv_nsec)
          / 1000000000.0;

  fprintf(stderr, "PARENT : Time spent : %lf\n", accum);

  releaseSegment(shmid, shm);

  return 0;
}