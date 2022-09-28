#include <stdlib.h>
#include <ourhdr.h>

#define WP 1
#define RP 0

void padre(int pW,int pR){

	char buf[50];
	char buf1[50];
	int fd = open("file.txt", O_RDONLY, 0666);
	int rd;

	if((rd =	read(fd, buf, sizeof(buf))) < 0)
		err_sys("Errore OPEN");

	//Scrive nella pipe
	if(fd < 0){
		err_sys("Err1");
	}
	if(write(STDOUT_FILENO, buf, strlen(buf))<= 0)
		err_sys("Err2");
	if(write(pW, buf, strlen(buf))<= 0)
		err_sys("Err4");
	//Legge dalla pipe
	if(read(pR, buf1, sizeof(buf1))<= 0)
		err_sys("Err5");
}

void figlio(int pR,int pW){
	char buf[50];

	if(read(pR, buf, 10)<= 0)
		err_sys("Err");
	if(write(pW, buf, strlen(buf))<= 0)
		err_sys("Err");
	if(write(STDOUT_FILENO, "\n", 2)<= 0)
		err_sys("Err");
}



int main(void){
	int line[2][2];
	int pid;

	for(int i=0; i < 2; i++){
		pipe(line[i]);
	}

	pid = fork();

	if(pid < 0)
		err_sys("ERROR: generating sons failed");

	if(pid == 0){
		figlio(line[0][RP],line[1][WP]);
		exit(0);
	}

	if(pid > 0){
		padre(line[0][WP], line[1][RP]);
	}


	return EXIT_SUCCESS;
}
