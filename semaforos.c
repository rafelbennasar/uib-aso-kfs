#include "semaforos.h"
key_t claveSem[2];
int mutex[2];

void semwait(int semnum){
	claveSem[semnum] = ftok("/usr/bin/ls", semnum);
	if (mutex[semnum] = semget(claveSem[semnum], 1, 0777|IPC_CREAT&IPC_EXCL) == -1)
		mutex[semnum] = semget(claveSem[semnum], 0, 0777|IPC_CREAT);
	else
		semctl(mutex[semnum], 0, SETVAL, 1);

	struct sembuf sops;
	sops.sem_num = 0;
	sops.sem_op = -1;
	sops.sem_flg = SEM_UNDO;
	semop(mutex[semnum], &sops, 1);
}

void semsignal(int semnum){
	struct sembuf sops;
	sops.sem_num = 0;
	sops.sem_op = 1;
	sops.sem_flg = 0;
	semop(mutex[semnum], &sops, 1);
	semctl(mutex[semnum], 0, IPC_RMID, NULL);
}

/*
int sem_get(int *mutex) {
	int n = ftok("/usr/bin/ls", 1);
	*mutex = semget(n, 1, 0600 | IPC_CREAT);
}

int sem_init(int *mutex) {
	int n = ftok("/usr/bin/ls", 1);
	*mutex = semget(n, 1, 0600 | IPC_CREAT);
	semctl(*mutex, 0, SETVAL, 1);
}

int sem_wait(int s) { 
	struct sembuf op [] = {0, -1, SEM_UNDO};
	semop(s, op, 1);
}

int sem_signal(int s) {
	struct sembuf op [] = {0, 1, 0};
	semop(s, op, 1);
}
*/
