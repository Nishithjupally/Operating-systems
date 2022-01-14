#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
sem_t mutex;
void* thread(void* arg) { //function which act like thread
   sem_wait(&mutex); //wait state
   printf("\nEntered into the Critical Section..\n");
   sleep(3); //critical section
   printf("\nCompleted...\n"); //comming out from Critical section
   sem_post(&mutex);
}
main() {
   sem_init(&mutex, 0, 1);
   pthread_t th1,th2;
   pthread_create(&th1,NULL,thread,NULL);
   sleep(2);
   pthread_create(&th2,NULL,thread,NULL);
   //Join threads with the main thread
   pthread_join(th1,NULL);
   pthread_join(th2,NULL);
   sem_destroy(&mutex);
}