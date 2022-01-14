#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/syscall.h>

int head = 0,tail = 0,totalConsumed=0,tp = 0;
int p,c,n;
sem_t mutex,empty,full;

void producerf(int array[])
{
	int totalProduced = 0;
	while(totalProduced < (10*n))
	{
		sem_wait(&empty);
		sem_wait(&mutex);
		array[head%n] = syscall(SYS_gettid);
		printf("Produced by %d\n",array[head%n]);
		head++;
		tp++;
		totalProduced++;
		sem_post(&mutex);
		sem_post(&full);
	}
}

void consumerf(int array[])
{
	while(totalConsumed < ((10*n)*p))
	{
		sem_wait(&full);
		sem_wait(&mutex);
		int data = array[tail%n];
		tail++;
		totalConsumed++;
		printf("Consumed %d\n",data);
		printf("totalConsumed %d\n",totalConsumed);
		sem_post(&mutex);
		sem_post(&empty);
	}
}

int main(int argc, char const *argv[])
{
	p = atoi(argv[1]);
	c = atoi(argv[2]);
	n = atoi(argv[3]);
	pthread_t producer[p],consumer[c];
	int buffer[n];
	sem_init(&mutex,0,1);
	sem_init(&full,0,0);
	sem_init(&empty,0,n);

	for (int i = 0; i < p; ++i)
	{
		pthread_create(&producer[i],NULL,(void*)producerf,buffer);
	}

	for (int j = 0; j < c; ++j)
	{
		pthread_create(&consumer[j],NULL,(void*)consumerf,buffer);
	}
	for (int k = 0; k < p; ++k)
	{
		pthread_join(producer[k],NULL);
	}
	for (int l = 0; l < c; ++l)
	{
		pthread_join(consumer[l],NULL);
	}
	printf("totalProduced %d\n",tp);
	printf("totalConsumed %d\n",totalConsumed);
	return 0;
}