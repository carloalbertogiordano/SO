/*
============================================================================
Name : TestSignal2.c
Author :
Version :
Copyright : Your copyright notice
Description : Hello World in C, Ansi-style
============================================================================

 * Parte 1:
* Scrivere un programma C che stampa un carattere a caso ogni 400 msec.
* con probabilità dell'1% stampa la 'o' ogni 400ms
* Quando viene premuto ^C l'handler deve verificare che l'ultimo carattere
* stampato sia 'o' ed in caso positivo incrementa un contatore.
* In ogni caso il programma riprende.
* Dopo 1000 caratteri il programa termina stampando il valore del contatore.
* Quando il processo riceve il segnale SIGHUP terminerà.
*
* Parte 2:
* Scrivere un programma C che registra un handler per ciascuno dei segnali
* noti e quando riceve un segnale stampa l'identificativo del segnale
* riprendendo successivamente la computazione.
*
* Parte 3:
* Scrivere un programma C che processa input e ad intervalli di tempi
* random (100 - 600 ms), stampa un carattere.
*/

#include <ourhdr.h>
#include <signal.h>

typedef void (*signaler_t)(int);

signaler_t vecchioHandler[32];
char carattereCorrente;
int cont = 0;
int totO = 0;
int intervallo;

void checkCurrentTargetHandler(int p){
	if(carattereCorrente == 'O'){
		cont++;
		printf("Oh Piano che mi hai colpito\n");
	}
	else
		printf("Sfigato\n");
}

void stampaCarattere(){
	int p;

	p = random() % 300;

	if(p == 0){
		carattereCorrente = 'O';
		totO++;
	} else
		carattereCorrente = '.';

	write(STDOUT_FILENO, &carattereCorrente, sizeof(carattereCorrente));

}

void termina(){
	printf("Numero totale di centri %d su %d\n", cont, totO);
}

void accdecc(int p){
	if(p == SIGUSR1)
		intervallo -= 50000;
	else if(p == SIGUSR2)
		intervallo += 50000;
}

int main(void) {

	if((vecchioHandler[SIGINT] = signal(SIGINT, checkCurrentTargetHandler)) == SIG_ERR)
		err_sys("Aiaa\n");

	if((vecchioHandler[SIGTERM] = signal(SIGTERM, termina)) == SIGTERM)
		err_sys("AAAAAAAAAA");

	if((signal(SIGUSR1, accdecc)) == SIGTERM)
		err_sys("No brumBrum");

	if((signal(SIGUSR1, accdecc)) == SIGTERM)
		err_sys("Dove sono i freni?");


	for(int i = 0; i < 1000; i++){
		stampaCarattere();
		usleep(intervallo);
	}

	termina();
	return EXIT_SUCCESS;
}
