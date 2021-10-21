

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include <signal.h>
//#include <alchemy/task.h>

#include "io.h"
#include <native/task.h>
#include <native/types.h>
#include <native/timer.h>
//#include <nucleus/timer.h>
//#include <nucleus/pod.h>
#include <asm-generic/xenomai/timeconv.h>
#include <rtdk.h>
#include <sys/mman.h>


	

int set_cpu(int cpu_number){
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number,&cpu);
	return pthread_setaffinity_np(pthread_self(),sizeof(cpu_set_t),&cpu);
}

int period_ns = 500000; //0,5 ms

void funcA(void *arg){
  unsigned long duration = 50000000000; //50 sec
  unsigned long endTime = rt_timer_read() + duration;
  
  rt_task_set_periodic(NULL, TM_NOW, period_ns);
  while(1){
    
    if(io_read(1)==0){
      io_write(1,0);
      usleep(5);
      io_write(1,1);
    }
    if (rt_timer_read()>endTime){
      rt_printf("Timeout\n");
      rt_task_delete(NULL);
    }
    if(rt_task_yield()){
      rt_printf("Task failed to yield\n");
      rt_task_delete(NULL);
    }
    
    rt_task_wait_period(NULL);
  }
}

void funcB(void *arg){
  unsigned long duration = 50000000000; //50 sec
  unsigned long endTime = rt_timer_read() + duration;
  
  rt_task_set_periodic(NULL, TM_NOW, period_ns);
  while(1){
    
    if(io_read(2)==0){
      io_write(2,0);
      usleep(5);
      io_write(2,1);
    }
    
    
    if (rt_timer_read()>endTime){
      rt_printf("Timeout\n");
      rt_task_delete(NULL);
    }
    if(rt_task_yield()){
      rt_printf("Task failed to yield\n");
      rt_task_delete(NULL);
    }
    rt_task_wait_period(NULL);
  }
}

void funcC(void *arg){
	unsigned long duration = 50000000000; //50 sec
	unsigned long endTime = rt_timer_read() + duration;
	  
	  
	  rt_task_set_periodic(NULL, TM_NOW, period_ns);
	  while(1){
		
		if(io_read(3)==0){
		  io_write(3,0);
		  usleep(5);
		  io_write(3,1);
		}
		
		
		if (rt_timer_read()>endTime){
		  rt_printf("Timeout\n");
		  rt_task_delete(NULL);
		}
		if(rt_task_yield()){
		  rt_printf("Task failed to yield\n");
		  rt_task_delete(NULL);
		}
		rt_task_wait_period(NULL);
	  }
	  
}

void* disturbance(void* arg){
	set_cpu(1);
	while(1){
		asm volatile("" ::: "memory");
	};
}

int main(){
  mlockall(MCL_CURRENT|MCL_FUTURE); 
  rt_print_auto_init(1);
  io_init();
  
  
  
  RT_TASK taskA, taskB, taskC;
  pthread_t d1,d2,d3,d4,d5,d6,d7,d8,d9,d10;
  
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
  
  rt_task_create(&taskA,"taskA",0,0,T_CPU(1));
  rt_task_create(&taskB,"taskB",0,0,T_CPU(1));
  rt_task_create(&taskC,"taskC",0,0,T_CPU(1));

  rt_task_start(&taskA, &funcA,0);
  rt_task_start(&taskB, &funcB,0);
  rt_task_start(&taskC, &funcC,0);

  pthread_join(d1, NULL);
  pthread_join(d2, NULL);
  pthread_join(d3, NULL);
  pthread_join(d4, NULL);
  pthread_join(d5, NULL);
  pthread_join(d6, NULL);
  pthread_join(d7, NULL);
  pthread_join(d8, NULL);
  pthread_join(d9, NULL);
  pthread_join(d10, NULL);
  	
	
  pause();



   return 0;
}

