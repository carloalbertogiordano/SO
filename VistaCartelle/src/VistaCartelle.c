/*
* Scrivere un programma C che prende su linea di comando
* un cammino (PATH) di una directory esistente e stampa in output
* tutte le direntry presenti nella directory
*
*
* TRACCIA 2
* Scrivere un programma C che prende su linea di comando
* una path e stampa tutti le direntries contenute nella directory
* e nelle sottodirectories


* TRACCIA 3
* Scrivere un programma C che prende su linea di comando una path e
* restituisce il cammino piu lungo visitando tutte le sottodirectory di
* PATH
*/

#include <ourhdr.h>
#include <dirent.h>
/*
int main(int argc, char *argv[]) {

	DIR *dir;

	if((dir = opendir(argv[1])) == NULL)
		err_sys("mannegg:");

	struct dirent * entry;

	while((entry = readdir(dir)) != NULL){
		printf("File %s\tType: %d\n", entry->d_name, entry->d_type);
	}

	return EXIT_SUCCESS;
}*/


