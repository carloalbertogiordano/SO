/*
 ============================================================================
 * Scrivere un programma C che prende sulla linea di comando un numero arbitrario
* di path (stringhe). Per ogni path, il main dovrà creare un sotto processo
* che esegue il comando /bin/ls sulla path indicata.
* Il processo padre dovrà attendere la terminazione di tutti i figli prima
* di stampare il numero totale dei processi eseguiti con successo
* (exit code = 0) ed uscire.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <ourhdr.h>

char binFile[]="/bin/ls";

void worker(char *path){
 if(execve(binFile,path,NULL)<0)
  err_sys("errore execve");
 exit(1);
}

int main(int argc,char *argv[]) {
 pid_t pid;
 argv[argc]=NULL;

	 pid = fork(); /*crea un processo figlio e ne restituisce il pid*/
	 if (pid < 0){
		 err_sys("Fork failed");
	 }
  else if (pid == 0){ //Mi trovo gia' nel processo figlio
     printf("Sono il figlio %d",getpid());
     worker(argv); //passo alla funzione che esegue execve la path
     exit(1); //Senza questa exit il processo figlio diventerebbe a sua volta padre di altri processi
    }

  else{
	  pid=wait(NULL);
	  if (pid < 0)
			  err_sys("All the children have to wait their task. Wait...");
	 printf("il processo %d, figlio ha terminato \n",pid);
    }
  exit(1);
}
