/*
 Scrivere un programma C che genera 10 sotto processi.
Ogni figlio attende il segnale SIGUSR1 ed alla ricezione del segnale
incrementa il proprio contatore.
Quando il contatore raggiunge il valore 100, il processo termina.
Il padre ogni 100 msec invia il segnale SIGUSR1 ad uno dei figli scelti a caso.
Se un figlio termina, individua quale è terminato e uccide tutti gli altri
per poi terminare a sua volta.
 */
#include <ourhdr.h>
#include <signal.h>
#include <sys/wait.h>

#define NUMPROC 10

typedef void (*signaler_t)(int);
signaler_t Handler[32];

int pos;

void incpos(){
	pos++;
}

void worker() {

	signal(SIGUSR1, incpos);
	int p = getpid();
	while(1){
		if(pos == 100){
				printf("\n\nHO FINITO IO: %d\n\n", p);
				write(STDOUT_FILENO, &p, sizeof(int));
				exit(0);
			}
		else
			;
	}
}

int main(void) {

	int pid[NUMPROC];
	int choise = 0;
	int canExit = 0, status, a, c = 1;

	pos = 0;

	for(int i = 0; i < NUMPROC; i++){
		pid[i] = fork();
		if(pid[i] == 0){
			worker();
		}
	}
	while(c){
		choise = rand() % NUMPROC;
		kill(pid[choise],SIGUSR1);
		usleep(10000);
		for(int i = 0; i < NUMPROC; i++){
			a = waitpid(pid[i],&status,WNOHANG);
			if(a != 0)
				c = 0;
		}
	}
	killpg(0,SIGTERM);


	return EXIT_SUCCESS;
}
