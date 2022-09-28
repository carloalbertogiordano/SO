#include <ourhdr.h>
#define WRITE_END 1
#define READ_END 0
#define NUMPROC 4
#define ITERATIONS 100000

void worker(int fd, int p, int div){
	int num = 0;
	int rb = 0;
	printf("FIGLIO %d\n", div);
	do{
		if((rb = (read(p, &num, sizeof(num)))) < 0){
			err_sys("Err5: errore nel leggere dalla pipe per il figlio o uno di loro. Io guido le navette, mica faccio l'idraulico\n");
		}
		if(num % div == 0){
			char dio[20];
			sprintf(dio,"%d divide %d \n",num,div);
			if((write(fd, dio, strlen(dio))) < 0){
				err_sys("Err6: Impossibile scrovere sul file\n");
			}
		}
	} while(rb > 0);
	close(fd);
	close(p);
	exit(0);
}



int main(void){
	int pipes[NUMPROC][2];
	int pid;
	int fd=open("out.txt",O_CREAT | O_TRUNC | O_WRONLY, 0664);
	int divisori[]={3,7,9,4};

	for(int j=0;j<NUMPROC;j++)
		if((pipe(pipes[j])) < 0){
			err_sys("Err 1: Errore nel generare la pipe\n");

	}

	for(int i = 0; i < NUMPROC; i++){
		if((pid = fork()) < 0)
			err_sys("Err 2: Impossibile fare la fork\n");

		else if(pid == 0){
		//figlio
			worker(fd, pipes[i][READ_END], divisori[i]);
		}
	}

		//padre
		for(int k = 0; k < ITERATIONS; k++){
			int random=rand();
			for(int i = 0; i < NUMPROC; i++){
				if(write(pipes[i][WRITE_END], &random, sizeof(random)) < 0){
					err_sys("Err3: impossibile scrivere nella pipe in posizione 0\n");
				}
				//usleep(100);
			}
		}

		for(int j=0;j<NUMPROC;j++){
			close(pipes[j][WRITE_END]);
			}


	return EXIT_SUCCESS;
}
