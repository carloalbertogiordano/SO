/*
 ============================================================================
 Name        : ComeCreareUnProcesso.c
 Author      : Zindrè
 Version     :
 Copyright   :
 Description : Scrivere un programma C che N_PROC
  	  	  	   Ogni processo figlio stamepra gli interi in un intervallo da inizio a fine ognuno preceduto dal proprio pid
  	  	  	   Il proceso padre attenderà la fine di tutti i suoi figli e poi uscirà
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <ourhdr.h>

#define N_PROC 10

void conta(int id, long da, long a)
{
	char buf[30];
	long i;
	for(i = da; i < a; i++)
	{
		sprintf(buf, "(id: %d->%ld)\n", id, i);

		if(i % 10000000 == 0)
			if(write(STDOUT_FILENO, buf, strlen(buf))<0) //send a message to another user
					err_sys("Write Failed");

		//usleep((random()%100000)+1000); //systemcall uspend execution for microsecond intervals
	}
}


int main(void)
{
	int pid;
	long i;
	long sizeTot=1000000;
	long sizeWorker = sizeTot/N_PROC;

	/*
	startCounting();
	conta(0,1L,sizeTot);
	stopCounting();
	printResourceUsage();
	*/

	startCounting();
	for ( i = 0; i< N_PROC; i++)
	{
		pid = fork();
		if(pid	< 0)
			printf("Errore\n");
		else if (pid == 0)
		{
			//printf("Figlio PID=%d\n",getpid());
			conta(getpid(), sizeWorker*i,sizeWorker*(i+1));
			exit(1); // se non termino il processo figlio una volta che questo ha effettuato le operaioni a lui connesse esegue lo stesso flusso del padre
		}
		else //if (pid > 0)
			printf("Padre PID=%d ha creato Figlio PID=%d\n",getpid(),pid);
	}
	for( i = 0; i< N_PROC;i++)
	{
		if((pid=wait(NULL))<0)
			err_sys("Waiting for child processes termination %d\n",i);
		else
			printf("Process %d termineted\n",pid);
	}
	stopCounting();
	printResourceUsage(RUSAGE_CHILDREN);
	return EXIT_SUCCESS;
}
