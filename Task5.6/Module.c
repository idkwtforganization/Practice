#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/sem.h>
#include <unistd.h>

int antisemid;

void signal_processing(){
	semctl(antisemid, 0, IPC_RMID, 0);
	exit(0);
}

int main(){
	key_t key = ftok("shared_resource.txt", 'Y');
	if ((antisemid = semget(key, 0, 0)) == -1)
		antisemid = semget(key, 4, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); //попадает как-то еврей в антимир
	signal(SIGINT, signal_processing);
	semctl(antisemid, 2, SETVAL, 0);
	struct sembuf sops;
	sops.sem_flg = 0;
	while (1){
		sops.sem_num = 0;
		sops.sem_op = -1;
		if (semop(antisemid, &sops, 1) == -1)
			exit(1);
		sops.sem_num = 1;
		if (semop(antisemid, &sops, 1) == -1)
			exit(1);
		sops.sem_op = 1;
		sops.sem_num = 2;
		if (semop(antisemid, &sops, 1) == -1)
			exit(1);
		printf("Create: Module\n");
	}
	return 0;
}
