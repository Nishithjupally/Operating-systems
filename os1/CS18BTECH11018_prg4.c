 #include <stdio.h>
 #include <sys/types.h>
 #include <unistd.h>
 #include <sys/wait.h>
 #include <signal.h>
 #include <stdlib.h>
 #include <time.h>
 #include <sys/utsname.h>

 void sigint(); 

 clock_t start,end;
 double times;

 void main() 
 { 
    int child1 = fork ();
    clock_t buffer[1024]; //piping syntax used from slides
    int fd[2];
    pipe (fd);


    if (child1 < 0) 
      { 
        printf("Error\n");
        exit(1); 
      } 
  
    if (child1 == 0) 
      { 
        close (fd[0]);
        end = clock ();

        signal (SIGINT, sigint);

        end = clock () - end;
        buffer[0] = end;
      } 
  
    else 
      { 
        int j;
        close (fd[1]);
        printf("Parent is about to send the signal SIGINT\n"); 

        start = clock ();

        kill (child1,SIGINT);
        
        start = clock () - start; 
        j = read (fd[0],buffer,sizeof(buffer));
        end = buffer[0];
        times = ((double) (start - end)) / CLOCKS_PER_SEC;

        wait (NULL);
        printf ("Time taken for a switch is %f seconds\n",times/2);
        //printf ("In parent again\n");

        //OS details
        printf("\n");
        printf("System details:\n");
        FILE *pf1;
        struct utsname info; //using structure from header
        int i = uname (&info);
        if(i == 0) 
          {
            printf ("Operating System : %s\n",info.sysname);
            printf ("Operating System Version: %s\n",info.version);
            printf ("Kernel Version: %s\n",info.release);
          }
        else
          {
            printf("Error in fetching OS details\n");
          }

        printf("CPU details:\n");
        printf("Architecture of processor: %s\n",info.machine);
        printf("Other details :\n");
        pf1 = popen ("inxi -C","r"); //using inix command
        char outputcpu[1024];
        fgets (outputcpu,1024,pf1);
        printf ("%s\n",outputcpu);
      } 
 } 
  
 void sigint()  
 { 
    signal (SIGINT, sigint);
    printf ("Child has received a signal SIGINT from parent\n"); 
 } 
  