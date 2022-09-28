/*
 ============================================================================
Scrivere un programma C che sulla riga di comando legge la path di una directory esistente e visitando
tutto il sottoalbero con radice path lista i ﬁle che contengono uno spazio non allocato stampando la
path completa, la size ed il numero di blocchi.
 ============================================================================
 */

#include <ourhdr.h>
#include <dirent.h>

int filtro(const struct dirent *entry){
	if((entry->d_type == DT_REG) || (entry->d_type == DT_DIR && strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0)) {
		return 1;
	}
	return 0;
}

int main(int argc, char* argv[]) {

	struct dirent **array;
	int n;

	if((n = scandir(argv[1], &array, filtro, NULL)) < 0)
		err_sys("Scandir() error");

	if(n < 0)
		err_sys("Scandir error");

	for(int i = 0; i < n; i++){
		struct stat buff;
		char fullpath[4096];

		sprintf(fullpath,"%s/%s",argv[1], array[i]->d_name);
		if(stat(fullpath,&buff) < 0){
			err_sys("stat() error");
		}
		if(buff.st_size > buff.st_blocks * 512)
			printf("Il file %s ha un buco \n", fullpath);

	}
	return EXIT_SUCCESS;
}
