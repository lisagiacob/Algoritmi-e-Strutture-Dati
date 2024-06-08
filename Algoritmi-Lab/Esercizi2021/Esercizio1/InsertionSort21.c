//INCLUDO LE LIBRERIE
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 

//DICHIARO LE FUNZIONI
//ordina maxInstances array casuali di lunghezza legth, e fa la media del tempo necessario.
double singleExperiment(int length, int maxInstances);
//Genera array di lunghezza length formati da valori casuali
void generateRandomArray(int* A, int length);
void experiment(int minLength, int maxLength);
void insertionSort(int* A, int length);
//Controlla che l'algoritmo di ordinamento funzioni assicurandosi che l'array sia effettivamente ordinato
bool isSorted(int* A, int n);

//DICHIARAZIONI GLOBALI
clock_t tStart = 0;		//inizio
clock_t tEnd = 0;		//fine
clock_t tElapsed = 0;		//variabile ausiliaria prima di sommare in tTot
clock_t tTot = 0;
int seed = 13;			//necessario per randomizzazione e ripetibilità dell'esperimento
int maxInstances = 50;
int step = 10;
int minLength = 10;
int maxLength = 300;
bool isSortedIS = true;
int maxRand=1000;

//FUNZIONE PRINCIPALE
int main(){
	srand(seed);

	experiment(minLength, maxLength);

	return 0;
}

//IMPLEMENTO LE FUNZIONI
double singleExperiment(int length, int maxInstances){
	int t_tot = 0;
	int* A = malloc(maxLength*sizeof(int));					//puntatore all'array random generato
	
	isSortedIS = true;
	
	for (int i=0; i<maxInstances; i++){
		generateRandomArray(A, length);	

		tStart = clock();			//tempo all'inizio
		insertionSort(A, length);		//dichiarare e implementare insertionSort!
		tEnd = clock();				//tempo alla fine
		tElapsed = tEnd - tStart;
		tTot += tElapsed;

		//ciclo di controllo
		for(int k=0; k<length; k++){
			printf("%d  ",A[k]);
		}
		printf("\n");

		if (!isSorted(A, length)) isSortedIS = false;
	}
	return tTot/maxInstances;			//ritorno il tempo medio necessario per InsertionSort
	free(A);
}

void generateRandomArray(int* A, int length){
	for (int i = 0; i < length; i++){
		A[i] = rand() % maxRand + 1;		//Creo un array A di lunghezza length con numeri random tra 1 e 1000
	}
}

void experiment(int minLength, int maxLength){
	FILE* file = fopen ("results.txt","w");
	if (file==NULL){
		printf("File non aperto\n");
		exit(1);
	}
	fprintf(file, "+-----------+-------------------------------+\n");
        fprintf(file, "| ######### | InsertionSort                 |\n");
        fprintf(file, "+-----------+-------------------+-----------+\n");
        fprintf(file, "| Dimension | Time              | isSorted? |\n");
        fprintf(file, "+-----------+-------------------+-----------+\n");

	for(int i = minLength; i < maxLength; i=i+step){
		clock_t time = singleExperiment(i, maxInstances);
		fprintf(file, "| %9d | %17f | %9s |\n", i, (double) time, isSortedIS ? "true" : "false");
	}
}

void insertionSort(int* A, int length){
	int key;
	int i;
	for (int j=1; j<length; j++){
		key = A[j];
		i = j-1;
		while((i>-1) && (A[i]>key)){		//non ho messo i>0 perchè altrimenti non ordinava il primo valore (noi iniziamo a contare da 0, non da 1)
			A[i+1] = A[i];
			i = i-1;
		}
		A[i+1] = key;
	}
	return;
}

bool isSorted(int* A, int n) {		//controlla che l'array che torna da insertionSort sia effettivamente ordinato
    for (int i = 0; i < n-1; i++) {
	    if (A[i] > A[i+1]) return false;
    }
    return true;
}