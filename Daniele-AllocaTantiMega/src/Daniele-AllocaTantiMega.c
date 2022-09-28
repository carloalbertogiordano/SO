#include <ourhdr.h>
#include <math.h>

void allocMatrix(int ***m, int size) {
	int i;

	if((*m = calloc(size, sizeof(int *))) == NULL)//allochiamo size elementi di taglia pari a quella di un puntatore ad intero
		err_sys("Errore allocazione matrice (size:%d)",size);

	for(i = 0; i < size; i++){
		if(((*m)[i] = calloc(size, sizeof(int))) == NULL)

			err_sys("Errore allocazione riga %d-esima",i);
	}
}

void initMatrix(int **m, int size) { //doppio puntatore perchè non dobbiamo modificare ma solo leggere
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			// random() % (max - min) + min
			m[i][j] = random() % 21 - 10;
		}
	}
}

void printMatrix(int **m, int size){
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			// random() % (max - min) + min
			printf("%d", m[i][j]);
		}
		printf("\n");
	}
}

void freeMatrix(int **ptr, int size){
	for (int i = 0; i < size; i++){
	    int* currentIntPtr = ptr[i];
	    free(currentIntPtr);
	}
}

int main(int argc, char * argv[]){

	long TotalSize, NumOps;
	int TotalSizeMb;
	int MatrixSize;
	int n;

	if(argc != 3){
		fprintf(stderr, "Errore nei parametri\nUsage : %s #MB #ops\n",argv[0]);
		exit(-1);
	}

	//definzione dei parametri
	TotalSizeMb = atoi(argv[1]);
	TotalSize = TotalSizeMb * 1024L * 1024L; //convertiamo la totalSize da Megabyte a byte
	NumOps = atol(argv[2]);
	MatrixSize = TotalSize / (3 * 4); //num elementi per ciascuna matrice
	n = sqrt(TotalSize / (3 * 4)); //num righe e colonne di ciascuna matrice

	//allocazione delle matrici A, B e C;

	//int A[n][n], B[n][n], C[n][n]; //NON FUNZIONA CON VECCHI COMPILATORI

	int **A, **B, **C; //OK,doppio puntatore perchè la matrice è un array di array cioè una lista di puntatori

	allocMatrix(&A, n);
	allocMatrix(&B, n);
	allocMatrix(&C, n);

	//inizializzazione delle matrici
	initMatrix(A, n);
	initMatrix(B, n);
	initMatrix(C, n);

	printMatrix(A,10); //stampa i primi 10 elementi della matrice
	printMatrix(B,10);

	//esegui le numOps richieste

	long i;
	int elem1,elem2;
	char puntino = '.';

	startCounting();
	for(i = 0; i < NumOps; i++){
		//esegui l'operazione i-esima
		elem1 = A[random()%n][random()%n];
		elem2 = B[random()%n][random()%n];

		C[random()%n][random()%n] += elem1 * elem2;

		if(i % 100000 == 0){
			if(write(STDOUT_FILENO,&puntino,sizeof(char)) < 0)
				err_sys("write failed");
		}
	}
	stopCounting();

	printf("#ops x sec = %,2f\n", (NumOps / getRealTime()));

	printResourceUsage(RUSAGE_SELF);

	freeMatrix(A, n);
	freeMatrix(B, n);
	freeMatrix(C, n);

}
