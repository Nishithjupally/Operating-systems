#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/syscall.h>

int p,c,N,in,out,count,tconsumer;

void *producer(int buffer[]);
void *consumer(int buffer[]);

int main(int argc, char const *argv[])
{
	
	p = atoi(argv[1]);
	c = atoi(argv[2]);
	N = atoi(argv[3]);
	int buffer[N];
	pthread_t prod[p],cons[c];
	//printf("%d %d %d\n",p,c,N);
	for (int i = 0; i < p; ++i)
	{
		pthread_create(&prod[i],NULL,(void *)producer,buffer);
	}
	for (int i = 0; i < c; ++i)
	{
		pthread_create(&cons[i],NULL,(void *)consumer,buffer);
	}
	for (int i = 0; i < p; ++i)
	{
		pthread_join(prod[i],NULL);
	}
	 for (int i = 0; i < c; ++i)
	{
		pthread_join(cons[i],NULL);
	}
	printf("Total %d\n",tconsumer);

	return 0;
}

void *producer(int buffer[])
{
	int produced = 0;
	while(produced < 10*N)
	{
		while(count == N)
			;

		buffer[in] = syscall(SYS_gettid);
		//cout<<buffer[in]<<" ";
		printf("Produced %d \n", buffer[in]);
		// printf("Produced %d %d\n", buffer[in],count);
		in = (in + 1)%(N);
		count++;
		//printf("Produced %d %d\n", buffer[in],count);

		produced++;

	}
	//printf("%d\n",produced);
}

void *consumer(int buffer[])
{
	//while(){
	while(tconsumer < ((10*N)*p))
	{
		while(count == 0)
			;

		int consumed = buffer[out];
		tconsumer++;
		printf("Consumed  %d \n", consumed);
		out = (out + 1)%N;
		count--;
		//printf("Consumed  %d %d\n", consumed,count);
	}
	//printf("IN cons\n");
//}
}
