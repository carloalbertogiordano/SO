//Scrivere un programma C che legge dalla linea di comando la dimensione
//totale di memoria (TotalSize) da allocare attraverso 3 matrici quadrate
//(n x n => n = sqrt( (TotalSize * 1024 * 1024) / (3 * 4)))
//e un intero che stabilisce il valore di NumOps

#include <math.h>
#include <ourhdr.h>

void allocMatrix(int ***m,int size){

	int **p, i;
	*m = calloc(size, sizeof(int *));

	for(i = 0, p = *m; i < size; p++, i++){
		//*(m+) = calloc(size, sizeof(int));
		if ((*p = calloc(size, sizeof(int))) < 0)
			err_sys("Errore");
	}
}

void initMatrix (int **m, int size){
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			m[i][j] = (random() % 21) - 10;
		}
	}
}

void printMatrix(int **m, int size){
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			printf("%d\t", m[i][j]);
		}
	}
	printf("\n\n\n");
}

void deallocMatrix(int **ptr, int size){
	for (int i = 0; i < size; i++){
	    int* currentIntPtr = ptr[i];
	    free(currentIntPtr);
	}
}

int main(int argc, char* argv[]) {

	int TotalSizeMb, MatrixSize, n;
	long TotalSize, NumOps;

	if(argc != 3){
		fprintf(stderr,"Errore nei parametri\n Usage: %s #MB #ops", argv[0]);
		exit(-1);
	}
	//definizione parametri
	TotalSizeMb = atoi(argv[1]); //non fatò più di 2 miliardi di MB
	TotalSize = TotalSizeMb *1024L * 1024L; //Lo converto in byte perchè malloc vuole questa unità di misura
	NumOps = atol(argv[2]);
	MatrixSize = (int) (TotalSize / 3) / 4; // /4 è la taglia di byte di un intero
	n = sqrt(MatrixSize); //Numero di righe e colonne della matrice
	//Allocazione e inizzializzazione
	int **A, **B, **C;
	allocMatrix(&A, n);
	allocMatrix(&B, n);
	allocMatrix(&C, n);
	//Inizzializzazione Matrici
	initMatrix(A, n);
	initMatrix(B, n);
	initMatrix(C, n);
	//Stampa Matrici
	printMatrix(A, 10);
	printMatrix(B, 10);
	printMatrix(C, 10);
	//EVVAjI




	//fai pulizia
	deallocMatrix(A, n);
	deallocMatrix(B, n);
	deallocMatrix(C, n);


	return EXIT_SUCCESS;
}
