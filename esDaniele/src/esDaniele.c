/*
 ============================================================================
 Name        : esDaniele.c
 Author      : Carlo
 Version     :
 Copyright   : Your copyright notice
 Description : Scrivere un programma in c che legge da linea di comando
 la dimensione e crea nella directory corrente un dile di dim byte e
 legge questo file. Per ogni operazione (scrittura e lettura ) stampa il
 tempo di esecuzione e il trough in Mb/s. per casa fare mb/s
 ============================================================================
 */

#include <ourhdr.h>

#define fName "file.tmp"
//#define Blocksize 4096

int scrittura(int dim,int GrandezzaBlocco){
	int fd;
    long wb=0; //long int wb=0
    int wr=0;
    char buf[GrandezzaBlocco];


    if((fd = open(fName, O_WRONLY|O_CREAT|O_TRUNC,664))<0)
    	err_sys("Errore nel apertura del file 0 %s",fName);

//  while(wb<dim){
//    	wr=write(fName,buf,GrandezzaBlocco);
//    	printf("wr %d",wr);
//    	if(wr<0)
//    		err_sys("Errore scrittura 1");
//    	wb+=wr;
//    }
    while (wb < dim){
    		if((wr = write(fd, buf, GrandezzaBlocco)) < 0)
    			err_sys("Write failed. Cannot write to file %s", fName);
    		wb += wr;
    	}
    close(fName);
    return 0;
}

int lettura(int dim,int GrandezzaBlocco){
	int fd;
    if(fd=open(fName, O_RDONLY,0664)<0);
    	err_sys("Errore nel apertura del file 1");
    long int rtb=0;
    int rab=0;
    char buf[GrandezzaBlocco];
    while(rtb<dim){
    	if(rab=read(fName,buf,GrandezzaBlocco)<0)
    		err_sys("Errore lettura");
    	else if(rab==0){
    		printf("Ho letto meno di quello che dovevo %d byte",rtb);
    		break;
    	}
    	rtb+=rab;
    }
    close(fName);
    return 0;
}

int main(int argc,char *argv[]) {
	if(argc!=3){
		printf("err paramteri %d dim\n",argv[0]);
		exit(-1);
	}

	//printf("%s %s ",argv[1],argv[2]);

	long dim=atol(argv[1]); //long int dim=atol(argv[1]);
	int GrandezzaBlocco=atoi(argv[2]);

	//printf("%ld %d",dim,GrandezzaBlocco);
	//startCounting();
	scrittura(dim,GrandezzaBlocco);
	//stopCounting();
	//printResourceUsage();
	//startCounting();
	lettura(dim,GrandezzaBlocco);
	//stopCounting();
	//printResourceUsage();

	//printf("done\n");


	return EXIT_SUCCESS;
}
