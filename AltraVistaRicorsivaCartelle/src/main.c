#include <ourhdr.h>
#include <dirent.h>

int counter = 0;

void vistaRicorsivaCartelle (char *path) {

	struct Dir *dir;
	struct dirent *dirEntry;
	struct stat fInfo;

	if((dir = opendir(path)) == NULL )
		err_sys("Impossibile aprire directory:");

	while((dirEntry = readdir(dir) )!= NULL){
		if(dirEntry->d_type == DT_REG){
			char fileName[4096];
			sprintf(fileName,"%s/%s",path,dirEntry->d_name);
			printf("\tStat: %s\n", fileName);
			printf("\tvisto file %s\n",fileName);
			if((stat(path, &fInfo)) < 0)
					err_sys("Errore nella stat");
			if(fInfo.st_size > 1024){
				counter++;
				printf(counter);
			}
		}
		if(dirEntry->d_type == DT_DIR && (strcmp(dirEntry->d_name,".")!=0) && (strcmp(dirEntry->d_name,"..")!=0)){
			printf("Apri directory %s\n",dirEntry->d_name);
			char newPath[1024];
			sprintf(newPath,"%s/%s",path,dirEntry->d_name);
			vistaRicorsivaCartelle(newPath);
		}
	}

	if(closedir(path) < 0)
		err_sys("Impossibile chiudere directory:");
	free(dirEntry);
}



int main () {
	vistaRicorsivaCartelle("/home/carlo/eclipse-workspace");
	printf("%d\n", counter);
}
