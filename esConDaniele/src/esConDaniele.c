/*
 ============================================================================
il padre crea un figlio che presa la path fa ls della directory e la scrive nella STDOUT e il padre la cattura. Successivamente crea n processi quanti file. Ogni processo prenda il file
e ad ogni occorrenza di una vocale manda al padre l'offset della vocale al padre
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <ourhdr.h>

#define location "/bin/ls"
#define WRITE_END 1
#define READ_END 0

void worker1(char *string){
	char *argv[]={location, string, NULL};

	execve(location, argv, NULL);
	exit(0);
}


void worker2 (int pipedr, char * name){
	int fd;
	if((fd = open(name, O_RDONLY)) < 0 )
		err_sys("File 2 open");

	int rb;
	char buf;

	while((rb = read(fd, buf, sizeof(buf))) > 0){
			if(buf == 'a' || buf == 'e' || buf == 'i' ||buf == 'o' || buf == 'u'){
				off_t offset=lseek(fd, 0, SEEK_CUR);
				if(write(pipedr, &offset, sizeof(offset)) < 0)
					err_sys("write failed");
			}
		}
	}

	close(fd);
	close(pipedr);
	exit(0);
}


int main(int argc, char *argv[]) {

	int pid;

	if(argc != 2)
		err_sys("Mettine solo una di path");

	int fd=open("file.txt", O_CREAT | O_TRUNC | O_WRONLY, 0664);

	if(fd < 0)
		err_sys("ERR: open failed");

	dup2(fd, STDOUT_FILENO);

	if((pid = fork()) < 0)
		err_sys("fork failed");

	worker1(argv[1]);

	close(fd);

	FILE *s=fopen(fd, "r");
	char buf[50];
	char listname[50][50];
	int cont = 0;
	while(fgets(buf, sizeof(buf), s) != EOF){
		strcpy(listname[cont], buf);
		cont++;
	}

	int pipes[cont][2];
	int pids[cont];

	for(int c = 0; c < cont; c++){
		pipe2(pipes[c], O_NONBLOCK);
	}
	for(int i = 0; i < cont; i++){
		pids[i]=fork();
		if(pids[i] == 0){
			worker2(pipes[i][WRITE_END], buf[i]);
		}
	}

	return EXIT_SUCCESS;
}
