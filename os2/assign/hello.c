#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	if (argc == 1)
	{
		fprintf(stdout,"Hello World\n");
	}

	else if(argc == 2)
	{
		fprintf(stdout,"Hello %s\n",argv[1]);
	}

	else 
	{
		fprintf(stderr,"Syntax error\n");
		return 1;
	}
	return 0;
}