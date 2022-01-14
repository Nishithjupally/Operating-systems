#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 64000
//#incl

int a3[SIZE];
int *ptr;

int main()
{
	int pgsize,pgbits,wait;
	pgsize = sysconf(_SC_PAGESIZE);

	for (int i = pgsize;pgbits=0; i > 1)
	{
		pgbits++;
		i>>=1;
	}
	printf("Page size %d Page bits %d\n",pgsize,pgbits);
	int a1[SIZE];

	ptr = (int*)malloc(SIZE*sizeof(int));

	printf("Stack Array : %p\n",a1);
	printf("Heap Array : %p\n",ptr);
	printf("Data Seg Array : %p\n",a3);
	//intwait
	scanf("%d",&wait);

	return 0;

}