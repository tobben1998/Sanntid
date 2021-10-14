
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>



pthread_mutex_t mtx1, mtx2, mtx3, mtx4, mtx5;

/* void* task1(void* args){
    while(1){
            pthread_mutex_lock(&mtx1); 
    if(&mtx5==1){
        usleep(100);
        pthread_mutex_lock(&mtx5); 
        printf("TASK1 eaating");
        pthread_mutex_unlock(&mtx5);  
    }
    pthread_mutex_unlock(&mtx1);  
    }


} */
void* task1(void* args){
    while(1){
    pthread_mutex_lock(&mtx1); 
    usleep(101);
    pthread_mutex_lock(&mtx5); 
    printf("TASK1 eating\n");
    pthread_mutex_unlock(&mtx1);
    pthread_mutex_unlock(&mtx5);  
    usleep(1000000);
    }  

}
void* task2(void* args){
    while (1){
    pthread_mutex_lock(&mtx2); 
    usleep(101);
    pthread_mutex_lock(&mtx1); 
    printf("TASK2 eating\n");
    pthread_mutex_unlock(&mtx2);
    pthread_mutex_unlock(&mtx1);  
    usleep(3000000);
    }
    
  

}
void* task3(void* args){
    while (1){
    pthread_mutex_lock(&mtx3);
    usleep(101);
    pthread_mutex_lock(&mtx2); 
    printf("TASK3 eating\n");
    pthread_mutex_unlock(&mtx3);
    pthread_mutex_unlock(&mtx2);  
    usleep(2027000);
    }
    
  

}
void* task4(void* args){
    while (1){
    pthread_mutex_lock(&mtx4); 
    usleep(100);
    pthread_mutex_lock(&mtx3); 
    printf("TASK4 eating\n");
    pthread_mutex_unlock(&mtx4);
    pthread_mutex_unlock(&mtx3);
    usleep(2008470);
    }
    
    

}
void* task5(void* args){
    while (1) {
    pthread_mutex_lock(&mtx5); 
    usleep(100);
    pthread_mutex_lock(&mtx4); 
    printf("TASK5 eating\n");
    pthread_mutex_unlock(&mtx5);
    pthread_mutex_unlock(&mtx4);  
    usleep(2003500);
    }
    
  

}



int main(){

pthread_mutex_init(&mtx1, NULL);
pthread_mutex_init(&mtx2, NULL);
pthread_mutex_init(&mtx3, NULL);
pthread_mutex_init(&mtx4, NULL);
pthread_mutex_init(&mtx5, NULL);



pthread_t thread1, thread2, thread3, thread4, thread5;

pthread_create(&thread1, NULL,task1,NULL);
pthread_create(&thread2, NULL,task2,NULL);
pthread_create(&thread3, NULL,task3,NULL);
pthread_create(&thread4, NULL,task4,NULL);
pthread_create(&thread5, NULL,task5,NULL);

pthread_join(thread1, NULL);
pthread_join(thread2, NULL);
pthread_join(thread3, NULL);
pthread_join(thread4, NULL);
pthread_join(thread5, NULL);



pthread_mutex_destroy(&mtx1);
pthread_mutex_destroy(&mtx2);
pthread_mutex_destroy(&mtx3);
pthread_mutex_destroy(&mtx4);
pthread_mutex_destroy(&mtx5);

    return 0;
}