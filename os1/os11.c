 #include <stdio.h>
 #include <sys/types.h>
 #include <unistd.h>
 #include <sys/wait.h>
 #include <signal.h>
 #include <stdlib.h>

 int main()
 {
	 int child1,child2;
	 child1 = fork ();

	 if (child1 < 0) //if fork fails
	   {
	   	 printf("Error\n");
	   }

	 if (child1 == 0) //child1
	   {
	 	 while(1)
		 {
			printf ("I am Child1 with process id %d\n",getpid());
			sleep (1);
		 }
	   }

	 else if(child1 != 0) //parent
	   {
	 	  child2 = fork();

		  if (child2 < 0) //fork failed
				printf("Error\n");

		  if (child2 == 0) //child2
			{
				sleep (10);
				printf ("I am Child2 with process id %d,slept for 10 seconds and about to kill child1\n",getpid());

				kill (child1,SIGKILL);

				printf ("I am Child2 with process id %d and killed child1 and about to sleep for 10 seconds\n",getpid());
				sleep (10);
				exit (0);
			}

		  else if(child2 != 0) //parent
			{
				wait (NULL);
				wait (NULL);
				printf ("Parent process with process id %d about to exit\n",getpid());
				exit (0);
			}
		 }
  }