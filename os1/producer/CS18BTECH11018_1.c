#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/syscall.h>

int p, c, N, in, out, count, tconsumer;

void
producer (int buffer[])
{
  int produced = 0;
  while (produced < 10 * N)	//stopping condition for producer
    {
      while (count == N)	//busy waiting when full
	;

      buffer[in] = syscall (SYS_gettid);	//getting thread id
      printf ("Produced by %d \n", buffer[in]);
      in = (in + 1) % (N);
      count++;
      produced++;

    }
}

void
consumer (int buffer[])
{
  while (tconsumer < ((10 * N) * p))	//stopping condition for consumers
    {
      while (count == 0)	//busy waiting when buffer is empty
	;

      int consumed = buffer[out];
      tconsumer++;
      printf ("Consumed  %d \n", consumed);
      out = (out + 1) % N;
      count--;
    }
}

int
main (int argc, char const *argv[])
{

  p = atoi (argv[1]);
  c = atoi (argv[2]);
  N = atoi (argv[3]);
  int buffer[N];
  pthread_t prod[p], cons[c];
  //printf("%d %d %d\n",p,c,N);
  for (int i = 0; i < p; ++i)
    {
      pthread_create (&prod[i], NULL, (void *) producer, buffer);
    }
  for (int j = 0; j < c; ++j)
    {
      pthread_create (&cons[j], NULL, (void *) consumer, buffer);
    }
  for (int k = 0; k < p; ++k)
    {
      pthread_join (prod[k], NULL);
    }
  for (int l = 0; l < c; ++l)
    {
      pthread_join (cons[l], NULL);
    }
  printf ("Total consumed = %d\n", tconsumer);

  return 0;
}
