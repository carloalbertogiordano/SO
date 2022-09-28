/*
 Description : Scrivere un programma in c che legge da linea di comando
 la dimensione e crea nella directory corrente un dile di dim byte e
 legge questo file. Per ogni operazione (scrittura e lettura ) stampa il
 tempo di esecuzione e il trough in Mb/s. per casa fare mb/s
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <ourhdr.h>
#include <time.h>

#define fName "file.txt"

void riempiArr(char arr[],long double size){
	for(int i=0;i<size;i++){
		char x=rand()%10+'0';						//generiamo un numero casuale e lo trasformiamo in carattere
		arr[i]=x;									//assegnamo all elemento i-esimo del buffer il carattere casuale precedemente generato
	}
}

int scrittura(long double dim,long unsigned int GrandezzaBlocco){
	int fd,wr=0;
	long int wb=0;
    char buf[GrandezzaBlocco];

    if((fd=open(fName, O_WRONLY|O_CREAT|O_TRUNC,664))<0)		//apriamo il file
    	err_sys("Errore nel apertura del file 0 %s",fName);		//se ci sono errori terminaimo il programma

    time_t t;
    srand(time(&t));							//inizializzaimo srand() per la random()

    while(wb<dim){								//il ciclo si ripetera fino a quando i byte scritti sono maggiori della dimensione del file
    	riempiArr(buf,GrandezzaBlocco);			//riempiamo il buffer in maniera casuale
    	wr=write(fd,buf,GrandezzaBlocco);		//scriviamo il buffer su file
    	if(wr<0)								//controlliamo che non ci siano stati errori
    		err_sys("Errore scrittura 1");
    	wb+=wr;									//aggiungiamo i byte scritti al conto dei byte totali scritti
    }
    close(fName);								//chiudiamo il file
    return 0;
}

int lettura(long  double dim,long unsigned int GrandezzaBlocco){
	int fd,rab=0;
    long int rtb=0;
    char buf[GrandezzaBlocco];

    if((fd=open(fName,O_RDONLY))<0)									//apriamo il file
		err_sys("Errore nel apertura del file 1 %s %d",fName,fd);	//se ci sono errori terminiamo il programma

    while(rtb<dim){									//il cilo si ripetera fino a quando i byte totali letti sono minori della dimensione del file
    	rab=read(fd,buf,GrandezzaBlocco);			//leggiamo il buffer da file
    	if(rab<0)									//controlliamo che non ci siano stati errori
    		err_sys("Errore lettura");
    	rtb+=rab;									//aggiungiamo i byte letti a quelli totali
    }
    close(fName);									//chiudiamo il file
    return 0;
}

int main(int argc,char *argv[]) {
	long double dim;
	long unsigned int GrandezzaBlocco;
	double timeWriting,timeReading,mb_s;

	if(argc!=3){
		printf("err paramteri %d dim",argv[0]);
		exit(-1);
	}

	dim=atol(argv[1]);							//assegnamo a dim il valore corrispettivo in long int
	GrandezzaBlocco=atol(argv[2]);				//assegnamo a blocksize il valore corrispettivo in long int


	startCounting();							//iniziamo a contare
	scrittura(dim,GrandezzaBlocco);				//scriviamo
	stopCounting();								//smettiamo di contare

	timeWriting=getRealTime();					//salviamo il tempo che abbiamo impiegato
	printf("Tempo per la scrittura %f s \n",timeWriting);


	startCounting();							//iniziamo a contare
	lettura(dim,GrandezzaBlocco);				//leggiamo
	stopCounting();								//smettiamo di contare

	timeReading=getRealTime();					//salviamo il tempo che abbiamo impiegato
	printf("Tempo per la lettura %lf s \n",timeReading);

	dim/=1000000;								//da byte a MegaByte

	mb_s=dim/timeWriting;						//calcoliamo i Mb/s per la scrittura
	printf("Mb/s per scrivere %f \n",mb_s);
	mb_s=(double)dim/timeReading;				//calcoliamo i Mb/s per la scrittura
	printf("Mb/s per leggere %lf  \n",mb_s);

	printf("Done.\n");
	return EXIT_SUCCESS;
}
