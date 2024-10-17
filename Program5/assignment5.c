#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <math.h>

#define NUM_PHILOSOPHERS 5

int randomGaussian(int mean, int stddev);
void philosopher(int id, int semid);		//philosopher routine
void semInit();				//initialize semaphores(chopsticks)
void semWait(int semid, int left, int right);	//try to acquire L/R sems, wait if impossible
void semSig(int semid, int left, int right);	//release L/R sems
void semClean();			//cleanup function

int main(int argc, char *argv[]){
	int semID;
	int shmID;
	struct sembuf semWait[1] = {0,-1,0};
	struct sembuf semSig[1] = {0,1,0};

	//initialize all chopsticks to 1 so they can be decremented	
	semID = semget(IPC_PRIVATE, NUM_PHILOSOPHERS, IPC_CREAT | IPC_EXCL | 0600);
	for(int i=0;i<NUM_PHILOSOPHERS;i++){
		semctl(semID, i, SETVAL, 1);	//set [i]th sem to 1
	}

	//fork N philosophers
	for(int i=0;i<NUM_PHILOSOPHERS;i++){
		pid_t pid=fork();	//fork a philo
		if(pid == 0){	//child
			printf("Generating philosopher %d.\n",i);
			philosopher(i, semID);
			exit(EXIT_SUCCESS);
		}else if(pid<0){	//parent
			printf("Fork failed for philo %d\n",i);
			semctl(semID, 0, IPC_RMID);
			exit(EXIT_FAILURE);
		}
	}

	//busywork to make sure we don't generate extra philos
	for(int i=0;i<NUM_PHILOSOPHERS;i++){
		if(wait(NULL)==-1){
			printf("Err: Waiting on child\n");
		}
	}

	return 0;
}

//main philosopher routine
void philosopher(int id, int semid){
	int leftChop = id;
	int rightChop = id+1;
	int eatTime=0;
	int thinkTime=0;

	//Think for thinkTime
	//attempt to grab chops
	//block if chops are blocked
	//eat for eatTime
	//loop
	while(eatTime < 100){
		
	}
}

//attempt to grab two chopsticks, or wait if both are not available
void semWait(int semid, int left, int right){
	struct sembuf sops[2];

	sops[0].sem_num = left;	//operate on sem0
	sops[0].sem_op = -1;	//block until sem0 == 1, then decrement
	sops[0].sem_flg = 0;	
	
	sops[1].sem_num = right;//operate on sem1
	sops[1].sem_op = 0;	//block until sem1 == 1, then decrement
	sops[1].sem_flg = 0;	

	semop(semid, sops, 2);	//grab both or neither
}

//release both chopsticks, freeing semaphores and allowing other philos to eat
void semSig(int semid, int left, int right){
	struct sembuf sops[2];

	sops[0].sem_num = left;	//operate on sem0
	sops[0].sem_op = 1;	//increment, making chopstick available
	sops[0].sem_flg = 0;

	sops[1].sem_num = right;//operate on sem1
	sops[1].sem_op = 1;	//increment, making chopstick available
	sops[1].sem_flg = 0;

	semop(semid, sops, 2);	//release both
}

void semClean(){

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
