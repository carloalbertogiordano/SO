#include <stdio.h>
#include <stdlib.h>
#include <ourhdr.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

#define read_pipe 0
#define write_pipe 1

#define NUM_SON 3
#define NUM_PIPE 4

void dad(int pip,int i){
	int fd,rab=0,wr;
	long int rtb=0;
	char buf[50];

	if((rab=read(pip,buf,sizeof(buf)))>0){
		printf("Il padre ha letto %d caratteri e ecco la schifezza ->%s\n",rab,buf);   								//aggiungiamo i byte letti a quelli totali
	}

	if(rab<0)
   		err_sys("Errore lettura\n");


	close(pip);
}

void worker(int pid[],int pipes[][2],int i){
	int fd,rab=0,wr;
	long int rtb=0;
	char buf[50];
	printf("Worker numero %d\n",i+1);

	if((rab=read(pipes[i][read_pipe],buf,sizeof(buf)))>0){
		if(rab<0)
			err_sys("Errore lettura\n");
		printf("Il figlio %d ha letto %d caratteri e ecco la schifezza ->%s\n",i+1,rab,buf);   								//aggiungiamo i byte letti a quelli totali
		close(pipes[i][read_pipe]);
		char mom[10];
		sprintf(mom,"%d \0",getpid());
		strcat(buf,mom);
		if((wr=write(pipes[i+1][write_pipe],buf,sizeof(buf)))<0)
			printf("Errore scrittura pipe %d \n",i+1);
		else
			printf("Il figlio %d ha scritto %d cartatteri nella pipe %d \n",i+1,wr,i+2);
		if(wr<0)
			err_sys("Errore scrittura\n");
		if(i!=NUM_SON-1){
			pid[i]=fork();
			if (pid[i]< 0){
						err_sys("Fork failed\n");
								}
					else if (pid[i]==0){
						worker(pid,pipes,i+1);
						exit(1);
								}
			if(waitpid(pid[i],NULL,0)<0)
						err_sys("You have to wait that all child terminates their task\n");
		}
		exit(1);
	}
}


int main(void) {
	int pid[NUM_SON];
	int pip[NUM_PIPE][2];

	char string[10];
	sprintf(string,"%d ",getpid());
	printf("Startt %s",string);
	for(int j=0;j<NUM_PIPE;j++)
		if(pipe2(pip[j],0)<0)
			err_sys("pipe failed\n");
	int i=0;
	int x=write(pip[0][write_pipe],string,sizeof(string));
	printf("%d \n",x);
	pid[i]= fork();
		if (pid[i]< 0){
			err_sys("Fork failed\n");
					}
		else if (pid[i]==0){
			close(pip[i][write_pipe]);
			worker(pid,pip,i);
			exit(1);
					}

	close(pip[3][write_pipe]);
	dad(pip[3][read_pipe],i);
		if(waitpid(pid[0],NULL,0)<0)
			err_sys("You have to wait that all child terminates their task\n");
	return EXIT_SUCCESS;
}
