/*
 ============================================================================
 /**
* Scrivere un programma C che sulla riga di comando legge la path di una directory esistente, e creando un processo figlio
* per ciascun file regolare presente in essa.
* Ogni processo figlio avrà il compito di trovare un file che contenga uno spazio non allocato(buco), o il carattere "proibito" '&'
* stampandone la path completa, la size ed il numero di blocchi, inviando poi un segnale al padre che terminerà dunque tutti i processi figli.
============================================================================
 */

#include <ourhdr.h>
#include <dirent.h>
#include <aio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>

#define FORBIDDEN_CHAR '&'

int n;
pid_t *pids;

int filter(const struct dirent *entry){
	if(entry->d_type == DT_REG)
		return 1;
	return 0;
}

void child_worker(char *path){
	struct stat buffer;
	char c;
	int fd;
	int read_characters;

	sleep(1);
	if((stat(path, &buffer)) < 0)
		err_sys("Stat failed\n");
	//controllo buco nel file
	if(buffer.st_size > buffer.st_blocks*512) { //512 è la size normale di unix (ext4)
		printf("Il file %s ha il buco ed ha size %ld e numero di blocchi %ld.\n", path, buffer.st_size, buffer.st_blocks);
		kill(getppid(), SIGUSR1);
		exit(0);
	}

	if((fd = open(path, O_RDONLY)) < 0)
		err_sys("Open failed");


	while((read_characters = read(fd,&c,sizeof(char))) > 0){
		if(c == FORBIDDEN_CHAR){
			printf("Il file ha il carattero proibito\n");
			kill(getppid(), SIGUSR1);
			exit(0);
		}
	}

	if(read_characters < 0)
		err_sys("Read failed");

	usleep(100000);
	exit(0);

}

void handler(int sig){
	if(sig == SIGUSR1)
		if (killpg(0,SIGKILL) < 0)
			err_sys("You can't kill me:");
}

int main(int argc, char* argv[]) {

	struct dirent **array;
	char fullpath[4096];

	if(argc < 2)
		err_sys("No command line arguments\n");

	n = scandir(argv[1], &array, filter, NULL);

	if(n<0)
		err_sys("Scandir Failed\n");

	pids = malloc(n * sizeof(pid_t));

	for(int i = 0; i < n; i++){
		if((pids[i]=fork()) < 0)
				err_sys("Fork failed");
		if(pids[i] == 0){
			sprintf(fullpath, "%s/%s", argv[1], array[i]->d_name);
			child_worker(fullpath);
		}
	}


	signal(SIGUSR1, handler);

	for(int i = 0; i < n; i++){
		if(wait(NULL) < 0)
			err_sys("wait failed");
	}

	free(pids);


	return 0;
}


