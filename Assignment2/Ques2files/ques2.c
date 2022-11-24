#include<stdio.h>
#include<sys/syscall.h>
#include<linux/kernel.h>
#include<unistd.h>
#include<errno.h>

int main() {
	float arr1[4][4] = {{1,2,3,4},{11,22,33,44},{6,7,8,9},{66,77,88,99}};
	float arr2[4][4];
	
	long status;
	int sys_id = 441+10;
	status = syscall(sys_id, arr1, arr2, 4);
	printf("%ld\n", status);
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			printf("%f ", arr2[i][j]);
		}
		printf("\n");
	}

	
}
