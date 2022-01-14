/*
 * Program: Speedup calculation of matrix multiplication with
 *          multi-processing and multi-threading.
 * Author:  changeme  changeme
 * Roll# :  changeme
 */

#include <stdlib.h>		/* for exit, atoi */
#include <stdio.h>		/* for fprintf */
#include <errno.h>		/* for error code eg. E2BIG */
#include <getopt.h>		/* for getopt */
#include <assert.h>		/* for assert */
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define z 8
/*
 * Forward declarations
 */

void usage (int argc, char *argv[]);
void input_matrix (int *mat, int nrows, int ncols);
void output_matrix (int *mat, int nrows, int ncols);
void init_matrix (int *mat, int nrows, int ncols);
unsigned long long int single_thread_mm ();
unsigned long long int multi_thread_mm ();
unsigned long long int multi_process_mm ();
void multithreadmul ();

int *A, *B, *C;
int crows, ccols;
int arows, acols, brows, bcols;
char interactive = 0;
int threaditer = 0;
sem_t mutex;


int main (int argc, char *argv[])
{
  int c;

  /* Loop through each option (and its's arguments) and populate variables */
  while (1)
    {
      int this_option_optind = optind ? optind : 1;
      int option_index = 0;
      static struct option long_options[] = {
	{"help", no_argument, 0, 'h'},
	{"ar", required_argument, 0, '1'},
	{"ac", required_argument, 0, '2'},
	{"br", required_argument, 0, '3'},
	{"bc", required_argument, 0, '4'},
	{"interactive", no_argument, 0, '5'},
	{0, 0, 0, 0}
      };

      c = getopt_long (argc, argv, "h1:2:3:4:5", long_options, &option_index);
      if (c == -1)
	break;

      switch (c)
	{
	case 0:
	  fprintf (stdout, "option %s", long_options[option_index].name);
	  if (optarg)
	    fprintf (stdout, " with arg %s", optarg);
	  fprintf (stdout, "\n");
	  break;

	case '1':
	  arows = atoi (optarg);
	  break;

	case '2':
	  acols = atoi (optarg);
	  break;

	case '3':
	  brows = atoi (optarg);
	  break;

	case '4':
	  bcols = atoi (optarg);
	  break;

	case '5':
	  interactive = 1;
	  break;

	case 'h':
	case '?':
	  usage (argc, argv);

	default:
	  fprintf (stdout, "?? getopt returned character code 0%o ??\n", c);
	  usage (argc, argv);
	}
    }

  if (optind != argc)
    {
      fprintf (stderr, "Unexpected arguments\n");
      usage (argc, argv);
    }

  unsigned long long time_single, time_multi_process, time_multi_thread;
  /* Add your code here */
  /* TODO */
  if (acols != brows)
    {
      exit (EXIT_FAILURE);
    }
  time_single = single_thread_mm ();
  time_multi_thread = multi_thread_mm ();
  time_multi_process = multi_process_mm ();

  fprintf (stdout, "Time taken for single threaded: %llu us\n", time_single);
  fprintf (stdout, "Time taken for multi process: %llu us\n",
	   time_multi_process);
  fprintf (stdout, "Time taken for multi threaded: %llu us\n",
	   time_multi_thread);
  fprintf (stdout, "Speedup for multi process : %4.2f x\n",
	   (double) time_single / time_multi_process);
  fprintf (stdout, "Speedup for multi threaded : %4.2f x\n",
	   (double) time_single / time_multi_thread);

  exit (EXIT_SUCCESS);
}

/*
 * Show usage of the program
 */
void usage (int argc, char *argv[])
{
  fprintf (stderr, "Usage:\n");
  fprintf (stderr, "%s --ar <rows_in_A>  --ac <cols_in_A>"
	   " --br <rows_in_B>  --bc <cols_in_B>" " [--interactive]", argv[0]);
  exit (EXIT_FAILURE);
}

/*
 * Input a given 2D matrix
 */
void input_matrix (int *mat, int rows, int cols)
{
  for (int i = 0; i < rows; i++)
    {
      for (int j = 0; j < cols; j++)
	{
	  fscanf (stdin, "%d", mat + (i * cols + j));
	}
    }
}

/*
 * Output a given 2D matrix
 */
void output_matrix (int *mat, int rows, int cols)
{
  for (int i = 0; i < rows; i++)
    {
      for (int j = 0; j < cols; j++)
	{
	  fprintf (stdout, "%d ", *(mat + (i * cols + j)));
	}
      fprintf (stdout, "\n");
    }
}

void init_matrix (int *mat, int nrows, int ncols)
{
  for (int i = 0; i < nrows; i++)
    {
      for (int j = 0; j < ncols; j++)
	{
	  //fscanf(stdin, "%d", mat+(i*cols+j));
	  *(mat + (i * ncols + j)) = rand ();

    }}
}

unsigned long long int single_thread_mm ()  //single thread function
{

  A = (int *) malloc (arows * acols * sizeof (int));
  B = (int *) malloc (brows * bcols * sizeof (int));
  C = (int *) malloc (arows * bcols * sizeof (int));


  if (interactive == 1)
    {
      fprintf (stdout, "Enter A:\n");
      input_matrix (A, arows, acols);
      fprintf (stdout, "Enter B:\n");
      input_matrix (B, brows, bcols);
    }
  else
    {
      init_matrix (A, arows, acols);
      init_matrix (B, brows, bcols);

    }
  //printf("HH\n");
  long long int temp = 0;

  struct timeval start;
  gettimeofday (&start, NULL);

  for (int i = 0; i < arows; ++i)
    {
      for (int j = 0; j < bcols; ++j)
	{
	  for (int k = 0; k < brows; ++k)
	    {
	      temp =
		temp + (*(A + (i * acols + k))) * (*(B + (k * bcols + j)));
	    }
	  (*(C + (i * bcols + j))) = temp;
	  temp = 0;
	}
    }

  struct timeval end;
  gettimeofday (&end, NULL);
  double totalTime;
  totalTime = (end.tv_sec - start.tv_sec) * 1e6;
  totalTime = (totalTime + (end.tv_usec - start.tv_usec)) * 1e-6;
  if (interactive == 1)
    {
      fprintf (stdout, "Result:\n");
      output_matrix (C, arows, bcols);
    }
  free (C);
  free (B);
  free (A);


  return (totalTime * 1000000);
}

unsigned long long int multi_thread_mm () //multi thread function
{
  A = (int *) malloc (arows * acols * sizeof (int));
  B = (int *) malloc (brows * bcols * sizeof (int));
  C = (int *) malloc (arows * bcols * sizeof (int));
  if (interactive == 1)
    {
      fprintf (stdout, "Enter A:\n");
      input_matrix (A, arows, acols);
      fprintf (stdout, "Enter B:\n");
      input_matrix (B, brows, bcols);
    }
  else
    {

      init_matrix (A, arows, acols);
      init_matrix (B, brows, bcols);
    }
  sem_init (&mutex, 0, 1);
  pthread_t threadmul[z];

  struct timeval start;
  gettimeofday (&start, NULL);
  for (int i = 0; i < z; ++i)
    {
      pthread_create (&threadmul[i], NULL, (void *) multithreadmul, NULL);
    }
  for (int i = 0; i < z; ++i)
    {
      pthread_join (threadmul[i], NULL);
    }

  struct timeval end;
  gettimeofday (&end, NULL);
  double totalTime;
  totalTime = (end.tv_sec - start.tv_sec) * 1e6;
  totalTime = (totalTime + (end.tv_usec - start.tv_usec)) * 1e-6;

  if (interactive == 1)
    {
      fprintf (stdout, "Result:\n");
      output_matrix (C, arows, bcols);
    }

  free (C);
  free (B);
  free (A);
  return (totalTime * 1000000);
  //}




}

void multithreadmul ()
{
  int x;
  sem_wait (&mutex);
  x = threaditer++;
  sem_post (&mutex);
  long long int temp = 0;

  for (int i = x * arows / z; i < (x + 1) * arows / z; ++i)
    {
      for (int j = 0; j < bcols; ++j)
	{
	  for (int k = 0; k < brows; ++k)
	    {
	      temp =
		temp + (*(A + (i * acols + k))) * (*(B + (k * bcols + j)));
	    }

	  (*(C + (i * bcols + j))) = temp;
	  temp = 0;
	}
    }
}

unsigned long long int multi_process_mm () //multi process function
{
  int a =
    shmget (IPC_PRIVATE, ((arows) * (acols) * (sizeof (int))),  //shared memory between processes
	    IPC_CREAT | 0666);
  int b =
    shmget (IPC_PRIVATE, ((brows) * (bcols) * (sizeof (int))),
	    IPC_CREAT | 0666);
  int c =
    shmget (IPC_PRIVATE, ((arows) * (bcols) * (sizeof (int))),
	    IPC_CREAT | 0666);

  A = (int *) shmat (a, NULL, 0);
  B = (int *) shmat (b, NULL, 0);
  C = (int *) shmat (c, NULL, 0);

  if (interactive == 1)
    {
      fprintf (stdout, "Enter A:\n");
      input_matrix (A, arows, acols);
      fprintf (stdout, "Enter B:\n");
      input_matrix (B, brows, bcols);
    }
  else
    {

      init_matrix (A, arows, acols);
      init_matrix (B, brows, bcols);
    }

  // clock_t start,end;
  // start = clock();
  fflush (stdout);
  struct timeval start;
  gettimeofday (&start, NULL);
  for (int l = 0; l < z; ++l)
    {
      int child = fork ();
      if (child == 0)
	{
	  A = (int *) shmat (a, NULL, 0);
	  B = (int *) shmat (b, NULL, 0);
	  C = (int *) shmat (c, NULL, 0);

	  long long int temp = 0;

	  for (int i = l * arows / z; i < (l + 1) * arows / z; ++i)
	    {
	      for (int j = 0; j < bcols; ++j)
		{
		  for (int k = 0; k < brows; ++k)
		    {
		      temp =
			temp +
			(*(A + (i * acols + k))) * (*(B + (k * bcols + j)));
		    }

		  (*(C + (i * bcols + j))) = temp;
		  temp = 0;
		}
	    }
	  exit (0);
	}

    }

  for (int i = 0; i < z; ++i)
    {
      wait (NULL);
    }
  struct timeval end;

  gettimeofday (&end, NULL);
  double totalTime;
  totalTime = (end.tv_sec - start.tv_sec) * 1e6;
  totalTime = (totalTime + (end.tv_usec - start.tv_usec)) * 1e-6;
  if (interactive == 1)
    {
      fprintf (stdout, "Result:\n");
      output_matrix (C, arows, bcols);
    }
  return (totalTime * 1000000);

}
