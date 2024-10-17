#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>
#include <math.h>

#define NUM_PHILOSOPHERS 5

int randomGaussian(int mean, int stddev);
void philosopher(int id);	//philosopher routine

int main(int argc, char *argv[]){
	int semID;
	int procID;
	struct sembuf semWait[1] = {0,-1,0};
	struct sembuf semSig[1] = {0,1,0};

	return 0;
}

int randomGaussian(int mean, int stddev) {
	double mu = 0.5 + (double) mean;
	double sigma = fabs((double) stddev);
	double f1 = sqrt(-2.0 * log((double) rand() / (double) RAND_MAX));
	double f2 = 2.0 * 3.14159265359 * (double) rand() / (double) RAND_MAX;
	if (rand() & (1 << 5)) 
		return (int) floor(mu + sigma * cos(f2) * f1);
	else            
		return (int) floor(mu + sigma * sin(f2) * f1);
}
