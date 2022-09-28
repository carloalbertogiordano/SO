#include <ourhdr.h>
#include <dirent.h>


void vistaCartelle (int fd, char* startPath) {

	struct Dir *dir = opendir(startPath);
	struct dirent *dirEntry;
	//struct stat dirInfo;
	//char buff[2048];

	printf("dir: %s\n", startPath);
	if(dir == NULL)
		err_sys("Impossibile ottenere info sulla directory");
	while((dirEntry = readdir(dir)) != NULL ){
		if(!((strcmp(dirEntry->d_name, ".")==0 || (strcmp(dirEntry->d_name, "..")==0)))){
			if(dirEntry->d_type == DT_REG){

			}
			if(dirEntry->d_type == DT_DIR){
				printf("DirEntry: %s\n", dirEntry->d_name);
				char newPath[2048];
				sprintf(newPath,"%s/%s", startPath, dirEntry->d_name);
				vistaCartelle(fd, newPath);
			}
		}
	}
	if(closedir(dir) < 0)
		err_sys("Impossibile chiudere directory:");

}


int main (int argc, char *argv[]) {

	int fd;

	if(argc != 2)
		err_sys("è necessario specificare la path");

	if( (fd = open("out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0664) )< 0)
		err_sys("impossibile creare risultati");

	vistaCartelle(fd, argv[1]);


	return EXIT_SUCCESS;
}

