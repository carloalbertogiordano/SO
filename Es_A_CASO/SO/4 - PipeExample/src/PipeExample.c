/*
 ============================================================================
 Name        : ComeCreareUnProcesso.c
 Author      : Zindrè
 Version     :
 Copyright   :
 Description : Scrivere un programma C che prende sulla linea di comando un numero arbitrario
 di path. Per ogni path il main dovrà creare un sotto processo che esegue il comando /bin/ls sulla apth indicata
 il processo padre dovrà attendere la terminazione di tutti i fili prima
 di stampare il numero totale dei processi eseguiti con successo (exit code = 0) ed uscire

 A scelta del candidato il processo padre potrà contare anche il numero totale di file (parole) restituite da ciascun figlio
 su stdout attraverso una pipe dedicata
  ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <ourhdr.h>

int main(int argc, char* argv[])
{	// array of 2 size a[0] is for
	// reading and a[1] is for
	// writing over a pipe

	int fd = open("exec.log", O_WRONLY | O_APPEND| O_CREAT | O_TRUNC, 0644);
	// using pipe for inter
	// process communication
	int i;
	for(i=1;i<=argc;i++)
	{
		int a[2];
		pipe(a);
		if(!fork())
		{
			if(!fork())
			{
				// closing normal stdin
				// making stdin same as a[0]
				dup2(a[0],0);
				// closing writing part in parent,
				// we don't need of it at this time
				close(a[1]);
				dup2(fd, 1);   // make stdout go to file
				close(fd);
				// executing wc
				char *newargv[] = {"wc","-w",NULL};
				execve("/bin/wc",newargv,NULL);
				// closing normal stdout
				// making stdout same as a[1]
			}
			// closing normal stdout
			// making stdout same as a[0]
			dup2(a[1],1);

			// closing reading part of pipe
			// we don't need of it at this time
			close(a[0]);

			// executing ls
			char *newargv[] = {"ls",argv[i],NULL};
			execve("/bin/ls",newargv,NULL);
		}
	}
	return 0;
}

