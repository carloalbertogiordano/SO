/*
 * TestPipeLab.c
 *
 *  Created on: 4 nov 2021
 *      Author: carlo
 *Scrivere un programma C che crea una pipe ed un processo figlio
 *Il processo padre apre un file e invia tutto il contenuto sulla pipe.
 *Il processo figlio legge dalle pipe e conta il numero di vocali.
 *Quando la pipe è terminata stampa il numero totale di vocali e termina.
 *
 *A questo punto termina anche il padre
 */
#include <ourhdr.h>
#include <stdlib.h>
#include <sys/wait.h>
//#include <unistd.h>


#define read_pipe 0
#define write_pipe 1
#define fName "file.txt"


int padre(int p){

	int fd=open(fName, O_RDONLY,664);
	char buf[50];
	int rb ;

	if(fd<0)		//apriamo il file
	    err_sys("Errore nel apertura del file 0 %s",fName);		//se ci sono errori terminaimo il programma

    while((rb = read(fd, buf, sizeof(buf))) > 0 ){	//il ciclo si ripetera fino a quando non si riempe il buffer o finisce il file
    	write(p, buf, rb);
    }

    if(rb < 0)
    	err_sys("Errore nel leggere i bytee nel file");

    close(p);
    close(fd);
}


int figlio(int p){

	char buf[50];
	int rb = 0;

	printf("Sono nel figlio\n");
	while((rb = read(p, buf, sizeof(buf))) > 0 ){	//il ciclo si ripetera fino a quando non legge tutta la pipe
	    	for(int i = 0; i < rb; i++){
	    		putchar(buf[i]);
	    	}
	    }

	if(rb < 0)
	    	err_sys("Errore nel leggere i bytee nel file");
    close(p);


}


int main(void){

	int p[2];
	int pid;

	pipe(p);
	pid = fork();
	printf("PID: %d %d\n", getpid(),pid);

	if(pid<0)
		printf("Figlio morto");
	if(pid == 0){
		printf("Figlio\n");
		close(p[write_pipe]);
		figlio(p[read_pipe]);
		exit(0);
	}

	if(pid > 0){
		//padre
		printf("Padre\n");
		close(p[read_pipe]);
		padre(p[write_pipe]);
		wait(NULL);
	}

	return EXIT_SUCCESS;
}

