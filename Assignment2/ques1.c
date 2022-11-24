#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<pthread.h>
#include<math.h>
#include<time.h>

void sub(struct timespec t1, struct timespec t2, struct timespec *td) {
	td->tv_nsec = t2.tv_nsec - t1.tv_nsec;
	td->tv_sec = t2.tv_sec - t1.tv_sec;

	if(td->tv_sec > 0 && td->tv_nsec < 0) { //)))()(((()()(&&&&&&&&&&&&&&
		td->tv_nsec += 1000000000;
		td->tv_sec--;
	}
	else if(td->tv_sec < 0 && td->tv_nsec > 0) {
		td->tv_nsec -= 1000000000;
		td->tv_sec++;
	}
}

void *countA(void *args) {
	FILE *f0;
	f0 = fopen("sched_other.txt", "a");
	struct timespec start, end, ans;
	clock_gettime(CLOCK_REALTIME, &start);
	int l = (int)pow(2, 20);
	//sleep(1);
	//int c  1;
	for(int i = 1;i <= l; i++) {
		//c++;
	}
	clock_gettime(CLOCK_REALTIME, &end);
	sub(start, end, &ans);
	printf("Thread A time: %d.%.9ld\n", (int)ans.tv_sec, ans.tv_nsec);
	fprintf(f0, "%d.%.9ld\n", (int)ans.tv_sec, ans.tv_nsec);
	fclose(f0);
}

void *countB(void *args) {
	FILE *f1;
	f1 = fopen("sched_rr.txt", "a");
	struct timespec start, end, ans;
	clock_gettime(CLOCK_REALTIME, &start);
	int l = (int)pow(2, 20);
	//sleep(1);
	//int c  1;
	for(int i = 1; i <= l; i++) {
		//c++;
	}
	clock_gettime(CLOCK_REALTIME, &end);
	sub(start, end, &ans);
	printf("Thread B time: %d.%.9ld\n", (int)ans.tv_sec, ans.tv_nsec);
	fprintf(f1, "%d.%.9ld\n", (int)ans.tv_sec, ans.tv_nsec);
	fclose(f1);
}

void *countC(void *args) {
	FILE *f2;
	f2 = fopen("sched_fifo.txt", "a");
	struct timespec start, end, ans;
	clock_gettime(CLOCK_REALTIME, &start);
	int l = (int)pow(2, 20);
	//sleep(1);
	//int c = 1;
	for(int i = 1; i <= l; i++) {
		//c++;
	}
	clock_gettime(CLOCK_REALTIME, &end);
	sub(start, end, &ans);
	printf("Thread C time: %d.%.9ld\n", (int)ans.tv_sec, ans.tv_nsec);
	fprintf(f2, "%d.%.9ld\n", (int)ans.tv_sec, ans.tv_nsec);
	fclose(f2);
}

int main(int argc, char* argv[]) {
	pthread_t t[3];
	int rc;
	int p0 = 50;
	int p1 = 50;
	int p2 = 50;  //5
	FILE *f0, *f1, *f2;
	pthread_attr_t attr0, attr1, attr2;
	struct sched_param param0, param1, param2;
	
	f0 = fopen("sched_other.txt", "a");
	f1 = fopen("sched_rr.txt", "a");
	f2 = fopen("sched_fifo.txt", "a");

	fprintf(f0, "%d ", p0);
	fprintf(f1, "%d ", p1);
	fprintf(f2, "%d ", p2);

	fclose(f0);
	fclose(f1);
	fclose(f2);

	pthread_attr_init(&attr0);
	pthread_attr_init(&attr1);
	pthread_attr_init(&attr2);

	pthread_attr_getschedparam(&attr0, &param0);
	pthread_attr_getschedparam(&attr1, &param1);
	pthread_attr_getschedparam(&attr2, &param2);

	param0.sched_priority = p0;
	param1.sched_priority = p1;
	param2.sched_priority = p2;

	pthread_attr_setschedpolicy(&attr0, SCHED_OTHER);
	pthread_attr_setschedpolicy(&attr1, SCHED_RR);
	pthread_attr_setschedpolicy(&attr2, SCHED_FIFO);

	pthread_attr_setschedparam(&attr0, &param0);
	pthread_attr_setschedparam(&attr1, &param1);
	pthread_attr_setschedparam(&attr2, &param2);
	
	if(pthread_create(&t[0], &attr0, &countA, NULL) != 0) {
		perror("Pthread Create Error");
	}
	if(pthread_create(&t[1], &attr1, &countB, NULL) != 0) {
		perror("Pthread Create Error");
	}
	if(pthread_create(&t[2], &attr2, &countC, NULL) != 0) {
		perror("Pthread Create Error");
	}

	for(int i = 0; i < 3; i++) {
		pthread_join(t[i], NULL);
	}
}
