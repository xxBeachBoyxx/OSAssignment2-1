#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>
#include<sys/types.h>
#include<sys/wait.h>

void sub(struct timespec t1, struct timespec t2, struct timespec *td) {
	td->tv_nsec = t2.tv_nsec - t1.tv_nsec;
	td->tv_sec = t2.tv_sec - t1.tv_sec;

	if(td->tv_sec > 0 && td->tv_nsec < 0) {
		td->tv_nsec += 1000000000;
		td->tv_sec--;
	}
	else if(td->tv_sec < 0 && td->tv_nsec > 0) {
		td->tv_nsec -= 1000000000;
		td->tv_sec++;
	}
}

int main(int argc, char *argv[]) {
	int pid0, pid1, pid2;
	int status0, status1, status2;
	struct timespec start, finish, delta;	
	
	clock_gettime(CLOCK_REALTIME, &start);
	pid0 = fork();

	if(pid0 < 0) {
		perror("Fork failed");
	}
	else if(pid0 == 0) {
		execl("/bin/sh", "ker", "./ker.sh", (char *)NULL);
	}
	else {
		waitpid(-1, &status0, 0);
		clock_gettime(CLOCK_REALTIME, &finish);
		sub(start, finish, &delta);
		printf("Time for process 0: %d.%.9f\n", (int)delta.tv_sec, delta.tv_nsec);
		
		clock_gettime(CLOCK_REALTIME, &start);		
		pid1 = fork();

		if(pid1 < 0) {
			perror("Fork failed");
		}
		else if(pid1 == 0) {
			execl("/bin/sh", "ker", "./ker.sh",  (char *)NULL);
		}
		else {
			waitpid(-1, &status1, 0);
			clock_gettime(CLOCK_REALTIME, &finish);
			sub(start, finish, &delta);
			printf("Time for process 1: %d.%.9f\n", (int)delta.tv_sec, delta.tv_nsec);


			clock_gettime(CLOCK_REALTIME, &finish);
			pid2 = fork();

			if(pid2 < 0) {
				perror("Fork failed");
			}
			else if(pid2 == 0) {
				execl("/bin/sh", "ker", "./ker.sh", (char *)NULL);
			}
			else{
				waitpid(-1, &status2, 0);
				clock_gettime(CLOCK_REALTIME, &finish);
				sub(start, finish, &delta);
				printf("Time for process 2: %d.%.9f\n", (int)delta.tv_sec, delta.tv_nsec);
			}
		}
	}
}
