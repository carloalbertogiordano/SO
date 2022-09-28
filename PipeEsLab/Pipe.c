/*
  Description : Scrivere un programma C che prende sulla linea di comando un numero arbitrario
 di path. Per ogni path il main dovrà creare un sotto processo che esegue il comando /bin/ls sulla apth indicata
 il processo padre dovrà attendere la terminazione di tutti i fili prima
 di stampare il numero totale dei processi eseguiti con successo (exit code = 0) ed uscire

 A scelta del candidato il processo padre potrà contare anche il numero totale di file (parole) restituite da ciascun figlio
 su stdout attraverso una pipe dedicata
*/
#include <stdlib.h>
#include <stdio.h>
#include <ourhdr.h>

int main(int argc,char *argv[]){
	//Prendi il file sul quale scriverò
	int fd = open("exec.log", O_WRONLY | O_APPEND| O_CREAT | O_TRUNC, 0644);
	int f[2];
	for(int i = 1; i < argc; i++){
		//Creo una pipe f
		pipe(f);
		//Creo un figlio che eseguirà l'if
		if(!fork()){
			//Chiudo lo stdout e gli dico di usare il file
			dup2(f[1], 1);
			//chiudo lo stdin perchè non serve (non dovrebbe essere necessario ma meglio così forse)
			close(0);
			//Creo la stringa da dare a ls e poi è facile lo sai
			char *tmpArgLs[]={"ls", argv[i], NULL};
			execve("/bin/ls",tmpArgLs,NULL);
		}
		if(!fork()){
			//Sostituisce stdin con l'in della pipe
			dup2(f[0], 0);
			//chiude lo stdout dellam pipe che tanto non serve
			close(f[1]);
			//sostituisce lo stdout con il file
			dup2(fd,1);
			//Vabbe prima o poi va chiuso sto file
			close(fd);
			//Ok da qua lo sai
			char *tmpArgWc[] = {"wc","-w",NULL};
			execve("/bin/wc",tmpArgWc,NULL);
		}

	}
	return EXIT_SUCCESS; //Grazie Dio
}
