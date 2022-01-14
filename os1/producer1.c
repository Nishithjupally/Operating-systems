#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/syscall.h>


int p,c,N,in=0,out=0,count;
long long int tconsumer=0;
//sem_t mutex,empty,full;
sem_t mutex;
sem_t empty;
sem_t full;

//sem_init(&mutex,0,1);
// sem_init(&mutex, 0, 1);
// sem_init(&full, 0, 0);

void producer(long long int buffer[]);
void consumer(long long int buffer[]);

// void p1(int *s)
// {
// 	while(s<=0)
// 		;
// 	*s =((*s)--)%N;
// }

// void s1(int *s)
// {
// 	*s =((*s)++)%N;
// }

int main(int argc, char *argv[])
{
	
	p = atoi(argv[1]);
	c = atoi(argv[2]);
	N = atoi(argv[3]);
	long long int buffer[N];
	//empty = N;
	sem_init(&mutex, 0, 1);
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, N);
	pthread_t prod[p],cons[c];
	//printf("%d %d %d\n",p,c,N);
	for (int i = 0; i < p; ++i)
	{
		pthread_create(&prod[i],NULL,(void *)producer,buffer);
	}
	for (int j = 0; j < c; ++j)
	{
		pthread_create(&cons[j],NULL,(void *)consumer,buffer);
	}
	for (int k = 0; k < p; ++k)
	{
		pthread_join(prod[k],NULL);
	}
	 for (int l = 0; l < c; ++l)
	{
		pthread_join(cons[l],NULL);
	}
	printf("Total %lld\n",tconsumer);

	// return 0;
}

void producer(long long int buffer[])
{
	// p1(&empty);
	// p1(&mutex);
	// sem_wait(&empty);
	// sem_wait(&mutex);
	static int produced = 0;
	while(produced < 10*N)
	{
		// while(count == N)
		// 	;
		// p1(&empty);
	 //    p1(&mutex);
	//	printf("Paina p\n");
		sem_wait(&empty);
	//	printf("emptyp\n");
	    sem_wait(&mutex);
	 //   printf("mutexp\n");
		buffer[in%N] = syscall(SYS_gettid);
		//cout<<buffer[in]<<" ";
		printf("Produced %lld \n", buffer[in%N]);
		// printf("Produced %d %d\n", buffer[in],count);
		//in = (in + 1)%(N);
		in=(in+1)%N;
		// 	count++;
		//printf("hip\n");
		//printf("Produced %d %d\n", buffer[in],count);

		
		sem_post(&mutex);
		sem_post(&full);
		produced++;
		//printf("HIP\n");

	}
	// s1(&mutex);
	// s1(&full);
	//printf("%d\n",produced);
}

void consumer(long long int buffer[])
{
	//while(){
	// p1(&full);
	// p1(&mutex);
	while(tconsumer < ((10*N)*p))
	{
		// while(count == 0)
		// 	;
		// printf("tconsumer %lld\n",tconsumer);
		//printf("Paina c\n");
		sem_wait(&full);
		//printf("full\n");
		sem_wait(&mutex);
		//printf("mutex c\n");
		long long int consumed = buffer[out%N];
		
		// printf("tconsumer %lld\n",tconsumer);
		printf("Consumed  %lld\n", consumed);
		out=(out+1)%N;
		tconsumer++;
		// count--;
		//printf("hi\n");
		sem_post(&mutex);
		sem_post(&empty);
		//printf("HI\n");
		//printf("Consumed  %d %d\n", consumed,count);
	}
	// s1(&mutex);
	// s1(&empty);
	//printf("IN cons\n");
//}
}
