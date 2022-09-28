/*
 ============================================================================
Scrivere un programma C che a partire dalla root del ﬁle system (/) visiti ricorsivamente tutto il sot-
toalbero (ilsenza però cambiare device se ci sono altri ﬁle system montati) stampando tutti i nomi e
le dimensioni dei “plain ﬁle” di dimensioni superiori a 1MB
 ============================================================================
 */

#include <ourhdr.h>
#include <dirent.h>

#define PATH_MAX 4096
#define mountPoint "mnt"
#define DEVICES "dev"


void visitaRicorsiva(char *fullPath){

	struct dirent *ent;
	DIR *dir;
	struct stat buff;
	char newPath[4096] ;


	printf("Opening: %s\n", fullPath);

	if((dir = opendir(fullPath)) == NULL)
		err_sys("opendir failed");

	while((ent = readdir(dir)) != NULL){
		strcpy(newPath,fullPath);
		printf("Sono nel while\n");
		switch(ent->d_type){
		case DT_REG:
				if(stat(fullPath,&buff) < 0)
					err_sys("stat() failed");
				printf("Il file %s ha dim: %ld\n", ent->d_name, buff.st_size);
			break;
		case DT_DIR:
			printf("Ho trovato la directory: %s\n", ent->d_name);
			if(strcmp(ent->d_name,mountPoint) !=0&&strcmp(ent->d_name,DEVICES) != 0){
				if ((strcmp(ent->d_name,".") !=0 && strcmp(ent->d_name,"..")!= 0)){
					if(strcmp(fullPath,"/") == 0 ){
						sprintf(newPath,"%s%s",fullPath,ent->d_name);
					}
					else
						sprintf(newPath,"%s/%s",fullPath,ent->d_name);
					visitaRicorsiva(newPath);
				}
			}
		break;
		default:
			printf("Unknown Filetipe di %s\n", ent->d_name);

			break;

		}
	}
	closedir(dir);
}

int main(int argc,char *argv[]) {

	char fullPath[4096] = "/";
	visitaRicorsiva(argv[1]);
	return EXIT_SUCCESS;
}
