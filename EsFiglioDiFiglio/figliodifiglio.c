#include <stdio.h>
#include <ourhdr.h>

#define NUMPROC 3
#define READ_END 0
#define WRITE_END 1

void worker(int k, int pipes[NUMPROC + 1][2]){
	char buf[50];
	char currentPID_string[10];
	int currentPID = getpid();

	printf("figlio %d \n",k);
	int rb=0;
	rb=read(pipes[k-1][READ_END], buf, sizeof(buf));
	printf("ho superato la read leggendo %d byte\n",rb);

	sprintf(currentPID_string, "%d ", currentPID);//converti il pid in char
	printf("buf %s\n",buf);
	strcat(buf, currentPID_string);//concatena il nuovo char al buffer da inviare a la pipe

	printf("buf %s\n",buf);

	if(write(pipes[k][WRITE_END], buf, strlen(buf)) < 0){
		err_sys("ERR write failed: ");
	}

	if(k < NUMPROC){
			printf("NOTE: k < NUMPROC\n");
			int pid = fork();
			worker(k+1, pipes);
			if(waitpid(pid, NULL, 0)<0)
				err_sys("Error waitpif < 0 \n");
		}

	exit(0);
}

int main(void){
	int pid;
	int pipes[NUMPROC + 1][2];
	int k = 0;
	char msg_TMP[] = "12345678";
	char buf[50];

	pipe(pipes[0]);

	for(int i = 1; i < NUMPROC + 1; i++){
		if(pipe(pipes[i]) < 0)
			err_sys("ERR: Impossibile creare le pipe");
	}

	int wb = 0;
	if((wb = write(pipes[0][WRITE_END], msg_TMP, strlen(msg_TMP)))<0)
		err_sys("write failed"); //Scrive sulla prima pipe

	printf("wb: %d\n", wb);

	if((pid = fork() ) < 0){
		err_sys("Fork failed");
	}

	else if(pid == 0){
		printf("cioa\n");
		printf("chiamo worker\n");
		worker(k+1, pipes);
	}

	read(pipes[NUMPROC][READ_END], buf, sizeof(buf));

	if(waitpid(pid, NULL, 0)<0)
					err_sys("waitpid < 0 ");

	write(STDOUT_FILENO, buf, strlen(buf));

	return EXIT_SUCCESS;
}
