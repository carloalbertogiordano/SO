/*
 ============================================================================
 Name        : Benchmarking.c
 Author      : Andrea Vitolo
 Version     :
 Copyright   :
 Description : Scrivere un programma C che letto da linea di comando una dimensione
               -Scriva un file nella directory corrente di dimensione pari
                a quella ricevuta in input
               -Termina la scrittura legga il file
               -Per ogni operazione visualizzi il tempo di esecuzione

 ============================================================================
 */


#include <ourhdr.h>

#define FILE_NAME "file.tmp" // temporaneo creato automaticamente da un programma software
						     // che di solito funge da file di backup o di cache

#define BLOCK_SIZE 4096 // 4096 bit = 512 byte

int writeFile(int dimensioneFile)
{
	/*
		 * Apro il file FILE_NAME
		 * Scivo almeno dimensioneFile byte in blocchi di dimensione BLOCK_SIZE
		 * Chiudo il file
	*/

	int fd;
	if ((fd= open( FILE_NAME, O_WRONLY|O_CREAT|O_TRUNC, 0664))< 0)
		err_sys("Open failed. Cannot open file %s for writing",FILE_NAME);

	int long writedByte = 0;
	int wr;
	char buffer[BLOCK_SIZE];

	while(writedByte < dimensioneFile)
	{
		if((wr = write(fd, buffer, BLOCK_SIZE)) < 0)
			err_sys("Open failed. Cannot open file %s for writing",FILE_NAME);
		writedByte += wr;
	}

	close(fd);

	return 0;
}

int readFile(int dimensioneFile)
{
	/*
		 * Apro il file FILE_NAME
		 * Leggo dimensioneFile byte in blocchi di dimensione BLOCK_SIZE
		 * Chiudo il file
	*/

	int fd;
	if ((fd= open( FILE_NAME, O_RDONLY))< 0)
		err_sys("Write failed");

	int long readedByte = 0;
	int rd;
	char buffer[BLOCK_SIZE];

	while(readedByte < dimensioneFile)
	{
		if((rd = read(fd, buffer, BLOCK_SIZE)) < 0)
			err_sys("Read failed");
		else if(rd == 0)
		{
			err_sys("Dimensione file non corrispondente\n Mancano %d byte",dimensioneFile-readedByte );
			break;
		}
		readedByte += rd;
	}

	close(fd);

	return 0;

}

#include <ourhdr.h>

int main(int argc, char *argv[])
{
	//input check
	if(argc != 2)
	{
		printf("Errore nei parametri\nDimensione file non fornita\n");
		exit(EXIT_FAILURE);
	}

	int long dimensioneFile=atol(argv[1]); // system call which convert a string to an integer
	//int blockSize = atoi(argv[2]);

	//start write counting
	startCounting();
	writeFile(dimensioneFile);
	stopCounting();
	//stop write couting
	printResourceUsage();
	//start read couting
	startCounting();
	readFile(dimensioneFile);
	stopCounting();
	//stop read counting
	printResourceUsage();
	return EXIT_SUCCESS;
}
