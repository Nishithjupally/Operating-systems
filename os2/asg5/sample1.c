#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int global = 433;
int main()
{
	int local = 544;
	char ch;
	sleep(10);
	printf("&global=%p &local=%p &main=%p\n", &global, &local, &main);
	scanf("%c", &ch);
}