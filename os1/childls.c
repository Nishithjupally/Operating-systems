#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	pid_t pid;

	pid = fork();

	if (pid < 0)
	{
		printf("error\n");
		return 1;
	}

	else if(pid == 0) //child
	{
		FILE *pf;
		char *command;
		strcpy(command,"ls -1 |wc -l");
		pf = popen(command,"r");
		printf("%c\n",fgetc(pf) );
	}

	else
	{
		wait(NULL);
	}
	return 0;


}