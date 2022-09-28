/*
 ============================================================================
Scrivere un programma C in cui il processo padre crea 5 sottoprocessi e dopo, ripetutamente, aspetta
50 msec, invia un segnale ad uno dei ﬁgli scelto a caso, controlla se uno dei ﬁgli è terminato e ri-
comincia. Ogni ﬁglio entra in un loop inﬁnito nel quale aspettano (pause()) il segnale SIGUSR1 e lo
trattano; il primo che riceve un numero di segnali pari a 100 terminerà stampando il proprio PID. Il
processo padre termina quando uno dei ﬁgli è terminato, dopo aver terminato tutti i ﬁgli ancora in
esecuzione.
 ============================================================================
 */

#include <ourhdr.h>
#include <signal.h>

#define NUMPROC 5
#define TIME 50

int position = 0;

void incpos() {
	position++;
}

void worker(){

	signal(SIGUSR1, incpos);

	while(1){
		if(position == 100){
			printf("Ho finito: %d\n", getpid());
			exit(0);
		}
	}

}



int main(void) {

	int pids[NUMPROC];
	int hasExited = 0;

	for(int i = 0; i < NUMPROC; i++){
		if((pids[i] = fork()) < 0)
			err_sys("Fork_FAILED");

		if(pids[i] == 0){
			worker();
			exit(0);
		}
	}

	for(int i = 0; i < NUMPROC; i++){
		printf("Figlo creato con pid: %d\t", pids[i]);
	}
	printf("\n");
	while(1){
		usleep(TIME * 1000);
		kill(pids[(rand() % NUMPROC)],SIGUSR1);
		for(int i = 0; i < NUMPROC; i++){
			hasExited = waitpid(pids[i],NULL,WNOHANG);
				if(hasExited != 0)
					killpg(0,SIGTERM);
		}
	}


	return EXIT_SUCCESS;
}
