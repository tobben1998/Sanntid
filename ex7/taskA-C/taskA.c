#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

long g=0;
sem_t mutex;

void* task(void* args){
    long l=0;
    for(int i=0; i<50000000; i++){
        l++;
        sem_wait(&mutex);
        g++;
        sem_post(&mutex);
        
    }
   
    printf("global: %ld \t local: %ld \n", g, l);
}



int main(){
sem_init(&mutex, 0, 1);


pthread_t threadG, threadL;

pthread_create(&threadG, NULL,task,NULL);
pthread_create(&threadL, NULL,task,NULL);
 

pthread_join(threadG, NULL);
pthread_join(threadL, NULL);


sem_destroy(&mutex);
    return 0;
}