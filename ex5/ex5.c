#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "io.h"
#include <sched.h>

int set_cpu(int cpu_number){
    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(cpu_number, &cpu);
    return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}


void timespec_add(struct timespec *t, long us){
    t->tv_nsec += us*1000;
    if(t->tv_nsec > 1000000000){
    t->tv_nsec = t->tv_nsec - 1000000000;
    t->tv_sec += 1;
    }
}

long period = 1000;

void *taskA(){
    set_cpu(1);
    struct timespec waketime;
    clock_gettime(CLOCK_REALTIME, &waketime);


    
    while(1){
        if(io_read(1) == 0){
            io_write(1,0);
            usleep(5);
            io_write(1,1);
        }
        timespec_add(&waketime, period);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &waketime, NULL);
    };
}

void *taskB(){
    set_cpu(1);
    struct timespec waketime;
    clock_gettime(CLOCK_REALTIME, &waketime);


    while(1){
        if(io_read(2) == 0){
            io_write(2,0);
            usleep(5);
            io_write(2,1);
        }
        timespec_add(&waketime, period);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &waketime, NULL);
    };
}

void *taskC(){
    set_cpu(1);
    struct timespec waketime;
    clock_gettime(CLOCK_REALTIME, &waketime);

    while(1){
        if(io_read(3) == 0){
            io_write(3,0);
            usleep(5);
            io_write(3,1);
        }
        timespec_add(&waketime, period);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &waketime, NULL);
    };
}

void *test(){
    struct timespec waketime;
    clock_gettime(CLOCK_REALTIME, &waketime);
    int i = 0;

    
    struct timespec period = {.tv_sec = 0, .tv_nsec = 500*1000};


    while(1){
        printf("Hello %d", i);
        fflush(stdout);
        i++;


        timespec_add(&waketime, period.tv_nsec);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &waketime, NULL);
    }
    
}

void *disturbance(){
    set_cpu(1);
    while(1){
        asm volatile("" ::: "memory");
    };
}

int main(){
    io_init();

    pthread_t threadA, threadB, threadC,d1,d2,d3,d4,d5,d6,d7,d8,d9,d10;

    pthread_create(&threadA, NULL,taskA,NULL);
    pthread_create(&threadB, NULL,taskB,NULL);
    pthread_create(&threadC, NULL,taskC,NULL);
    pthread_create(&d1,NULL,disturbance,NULL);
    pthread_create(&d2,NULL,disturbance,NULL);
    pthread_create(&d3,NULL,disturbance,NULL);
    pthread_create(&d4,NULL,disturbance,NULL);
    pthread_create(&d5,NULL,disturbance,NULL);
    pthread_create(&d6,NULL,disturbance,NULL);
    pthread_create(&d7,NULL,disturbance,NULL);
    pthread_create(&d8,NULL,disturbance,NULL);
    pthread_create(&d9,NULL,disturbance,NULL);
    pthread_create(&d10,NULL,disturbance,NULL);

    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);
    pthread_join(threadC, NULL);
    pthread_join(d1,NULL);
    pthread_join(d2,NULL);
    pthread_join(d3,NULL);
    pthread_join(d4,NULL);
    pthread_join(d5,NULL);
    pthread_join(d6,NULL);
    pthread_join(d7,NULL);
    pthread_join(d8,NULL);
    pthread_join(d9,NULL);
    pthread_join(d10,NULL);

    return 0;
}
