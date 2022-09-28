/*
 ============================================================================
 Name        : BenchIO.c
 Author      : Carlo
 Version     :
 Copyright   : Your copyright notice
 Description : Scrivere un programma C che legge da linea di comando la dimensione (dim) e prima scrive un file nella directory corrente di dimensione dim byte
 e legge questo file. Per ogni operazione (lettura/scrittura) mi dice quanto ci ha messo e mi stampa i MB/s.
 ============================================================================
 */

#include <ourhdr.h>

#define fName "file.tmp" //file

int writeDim(int dim, int blockSize){

	int fd = 0, wr = 0;
	long writtenBytes = 0;
	char buf[blockSize];

	if((fd = open(fName, O_WRONLY | O_CREAT | O_TRUNC, 664)) < 0)
		err_sys("Open failed. Cannot open file %s", fName);

	while (writtenBytes < dim){
		if((wr = write(fd, buf, blockSize)) < 0) //wr è written
			err_sys("Write failed. Cannot write to file %s", fName);
		writtenBytes += wr;
	}
	close(fd);
	return 0;
}//creo un file FName(se esiste già lo cancello) e scrivo almeno dim byte a blocchi BlockSize. Poi chiudo il file
//fin quando writtenBytes < dim scribo blockSize bytes.

int readDim(int dim, int blockSize){

	long readBytes = 0;
	int fd = 0, rr = 0;
	char buf[blockSize];


	if((fd = open(fName, O_RDONLY)) < 0){
		err_sys("Open failed. Cannot open file %s", fName);
	}

	while(readBytes < dim){
		if((rr = read(fd, buf, blockSize)) < 0)
			err_sys("Read failed. Cannot read file %s", fName);

		else if(rr == 0){
			printf("Premature EOF after %ld read bytes", readBytes);
		}

		readBytes += rr;
	}
	return 0;
}

int main(int argc, char* argv[]) {

	if(argc != 3){
		printf("Errore nei parametri. usage: %s dim", argv[0]);
		exit(-1);
	}

	long dim=atol(argv[1]); //con unsigned int posso scrivere file max 4 GB, quindi uso long
	int blockSize = atoi(argv[2]);

	startCounting();//start cronometro
	writeDim(dim, blockSize);
	stopCounting();//stop cronometro
	printResourceUsage();

	startCounting();//start cronometro
	readDim(dim, blockSize);
	stopCounting();//stop cronometro
	printResourceUsage();


	return EXIT_SUCCESS;
}
