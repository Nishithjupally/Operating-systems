#include <stdio.h>
#include <sys/types.h>
#include<stdlib.h>
/* Primitive System Data Types */
int main(int argc, char **argv)
{
char *myName = argv[0];
int cpid = fork();
if (cpid == 0) { //Child
//sleep(5); //sleeps for 5 secs
printf("1My pid: %d I am a child of %s My parentpid: %d\n", getpid(), myName, getppid());
exit(0);
} else { //Parent
printf("2My pid: %d My child’s pid: %d\n",getpid(),cpid);
exit(0);
}

}