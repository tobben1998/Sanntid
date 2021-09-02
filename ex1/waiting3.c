#include <sys/times.h>
#include <stdio.h>
#include <stdlib.h>

void busy_wait(int t){
	struct tms thentime, nowtime;
	clock_t then=times(&thentime);
	clock_t now=times(&nowtime);
	while((now-then)/100<t){
		for(int i=0;i <10000; i++){}
		now=times(&nowtime);
	}
}

int main(){
	busy_wait(1);
}
