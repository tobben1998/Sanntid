#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
#include <time.h>
#include <unistd.h>
#include <x86intrin.h>


void rdtsc1(){
    for (int i = 0; i < 10*1000*1000; i++){
        __rdtsc();
    }
}

void clockGettime1(){
    for (int i = 0; i < 10*1000*1000; i++){
        struct timespec now;
	    clock_gettime(CLOCK_MONOTONIC, &now);
    }
}

void times1(){
    struct tms now;
    for (int i = 0; i < 10*1000*1000; i++){
        times(&now);
    }
}

void rdtsc2(){
    int cpuFreqMHz = 1.4; //my pc
    const int ns_max = 50;
    int histogram[ns_max];
    memset(histogram, 0, sizeof(int)*ns_max);

    for(int i = 0; i < 10*1000*1000; i++){
        int t1 = __rdtsc();
        int t2 = __rdtsc();

        int ns = (t2 - t1)/cpuFreqMHz;

        if(ns >= 0 && ns < ns_max){
            histogram[ns]++;
        }
    }

    for(int i = 0; i < ns_max; i++){
        printf("%d\n", histogram[i]);
    }
}


///////////////////////////////

struct timespec timespec_normalized(time_t sec, long nsec){
    while(nsec >= 1000000000){
        nsec -= 1000000000;
        ++sec;
    }
    while(nsec < 0){
        nsec += 1000000000;
        --sec;
    }
    return (struct timespec){sec, nsec};
}

struct timespec timespec_sub(struct timespec lhs, struct timespec rhs){
    return timespec_normalized(lhs.tv_sec - rhs.tv_sec, lhs.tv_nsec - rhs.tv_nsec);
}

///////////////////////////////////////

void clockGettime2(){
    const int ns_max = 50;
    int histogram[ns_max];
    memset(histogram, 0, sizeof(int)*ns_max);
    struct timespec t1, t2;
    for(int i = 0; i < 10*1000*1000; i++){
        clock_gettime(CLOCK_MONOTONIC, &t1);
        clock_gettime(CLOCK_MONOTONIC, &t2);
        int ns = timespec_sub(t2,t1).tv_nsec;

        if(ns >= 0 && ns < ns_max){
            histogram[ns]++;
        }
    }

    for(int i = 0; i < ns_max; i++){
        printf("%d\n", histogram[i]);
    }
}



void times2(){
    const int ns_max = 50;
    int histogram[ns_max];
    memset(histogram, 0, sizeof(int)*ns_max);
    struct tms now;
    int ticPerSec=sysconf(_SC_CLK_TCK);
    for(int i = 0; i < 10*1000*1000; i++){
        int t1=times(&now);
        int t2=times(&now);

        int ns = (t2 - t1)*1000000000/ticPerSec;

        if(ns >= 0 && ns < ns_max){
            histogram[ns]++;
        }
    }

    for(int i = 0; i < ns_max; i++){
        printf("%d\n", histogram[i]);
    }
}



int main(){
    //used time to measure time used and divide by 10^7

    //rdtsc1(); // acess latency: 1.000*10^⁻8
    //clockGettime1(); // acess latency: 2.080*10^-8
    //times1(); // acess latency: 1.400s*10^-7

    rdtsc2();
    //clockGettime2();
    //times2();


}

