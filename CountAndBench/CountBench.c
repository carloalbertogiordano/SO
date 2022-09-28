#include <stdio.h>
#include <ourhdr.h>

#define N_PROC 4
#define COUNT_TO 10000000000

//Sequential version
/*
void conta(int id, long da, long a){
	char buf[30];
	long i;
	for(i = da; i < a; i++){
		sprintf(buf,"id:%d->%d", id, i);
	}
}




int main (void){
	startCounting();
	conta(0, 1L, 1000000000L);
	stopCounting();
	printResourceUsage();

	return EXIT_SUCCESS;
}*/


void conta(int id, long da, long a){
	char buf[30];
	long i;
	for(i = da; i < a; i++){
		sprintf(buf,"id:%d->%l", id, i);
		if(i%1000000==0){
			buf[0]='.';
			buf[1]='\0';
			if(write(STDOUT_FILENO, buf, strlen(buf))<0)
				err_sys("ERRORE MANNAGINA");
		}
	}
	exit(1);
}

int main(void){
	int pid;
	int i;
	long size =(long) (COUNT_TO / N_PROC);
	startCounting();
	for(i = 0 ;i < N_PROC; i++){
		pid = fork();

		if(pid < 0){
			err_sys("Err: impossibile generare figli");
		}
		else if(pid==0){
			printf("Sono il figlio con pid->%d\n", getpid());
			conta(getpid(), size*i, size*(i + 1));
			exit(1);
		}
		else
			printf("Sono il padre: pid %d\n", getpid());
	}
	for(i = 0; i < N_PROC; i++){
		if((pid = wait(NULL)) < 0){
			err_sys("Error in wait");
		}
		else
			printf("All done\n");
	}
	stopCounting();
	printResourceUsage();
	return EXIT_SUCCESS;
}
