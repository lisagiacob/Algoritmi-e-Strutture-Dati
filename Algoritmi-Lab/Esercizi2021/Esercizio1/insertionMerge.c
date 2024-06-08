//INCLUDO LE LIBRERIE
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <string.h>

//DICHIARO LE FUNZIONI
double singleExperiment(int length, int maxInstances, char* algo);	//ordina maxInstances array casuali di lunghezza legth, e fa la media del tempo necessario.
void generateRandomArray(int* A, int length);	//Genera array di lunghezza length formati da valori casuali
void experiment(int minLength, int maxLength);
void insertionSort(int* A, const int length);
void merge(int* A, const int low, const int mid, const int high);
void mergeSort(int* A, const int low, const int high);
void hybridSort(int* A, const int low, const int high);
void adaptedInsertionSort(int* A, const int low, const int high);
void quickSort(int* A, int low, int high);
int partition(int* A, int low, int high);
void medianOfThreeQuickSort(int* A, int low, int high);
int medianOfThreePartition(int* A,int low,int high);
int medianOfThree(int* A, int i, int j, int k);
void tailQuickSort(int* A, int low, int high);
void swapValue(int* A, int a, int b);
bool isSorted(int* A, int n);	//Controlla che l'algoritmo di ordinamento funzioni assicurandosi che l'array sia effettivamente ordinato

//DICHIARAZIONI GLOBALI
clock_t tStart = 0;		//inizio
clock_t tEnd = 0;		//fine
clock_t tElapsed = 0;		//variabile ausiliaria prima di sommare in tTot
clock_t tFinal = 0;
clock_t tTot = 0;
clock_t timeIS = 0;
clock_t timeMS = 0;
clock_t timeHS = 0;
clock_t timeQS = 0;
clock_t timeTQS = 0;
int seed = 13;			//necessario per randomizzazione e ripetibilità dell'esperimento
int maxInstances = 50;
int step = 10;
int minLength = 10;
int maxLength = 1000;
bool isSortedIS = true;
bool isSortedMS = true;
bool isSortedHS = true;
bool isSortedQS = true;
bool isSortedTQS = true;
int maxRand=1000;
const int threshold = 220;
const int threshold2 = 240;

//FUNZIONE PRINCIPALE
int main(){
	srand(seed);

	experiment(minLength, maxLength);

	return 0;
}

//IMPLEMENTO LE FUNZIONI
double singleExperiment(int length, int maxInstances, char* algo){
	tTot = 0;
	
	int* A = malloc(maxLength * sizeof(int));		//puntatore all'array random generato
	
	isSortedIS = true;
	isSortedMS = true;
	isSortedHS = true;
	isSortedQS = true;
	isSortedTQS = true;
	
	for (int i=0; i<maxInstances; i++){		//->numero massimo di esperimenti
		generateRandomArray(A, length);	

		if(strcmp(algo, "IS")==0){
			tStart = clock();			//tempo all'inizio
			insertionSort(A, length);		
			if (!isSorted(A, length)) isSortedIS = false;
			tEnd = clock();				//tempo alla fine
		}
		if(strcmp(algo, "MS")==0){
			tStart = clock();			
			mergeSort(A, 0, length-1);
			if (!isSorted(A, length)) isSortedMS = false;		
			tEnd = clock();				
		}

		if(strcmp(algo, "HS")==0){
			tStart = clock();			
			hybridSort(A, 0, length-1);
			if (!isSorted(A, length)) isSortedHS = false;	
			tEnd = clock();				
		}

		if(strcmp(algo, "QS")==0){
			tStart = clock();			
			medianOfThreeQuickSort(A, 0, length);
			if (!isSorted(A, length)) isSortedQS = false;	
			tEnd = clock();				
		}

		if(strcmp(algo, "TQS")==0){
			tStart = clock();			
			tailQuickSort(A, 0, length);
			if (!isSorted(A, length)) isSortedTQS = false;	
			tEnd = clock();				
		}

		tElapsed = tEnd - tStart;
		tTot += tElapsed;
	}
	tFinal=tTot/maxInstances;
	return tFinal;			//ritorno il tempo medio necessario per l'algoritmo algo
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
	fprintf(file, "+-----------+-------------------------------+-------------------------------+-------------------------------+-------------------------------+-------------------------------+\n");
        fprintf(file, "| ######### | InsertionSort                 | MergeSort                     | HybridSort                    | QuickSort M3                  | Tail QuickSort                |\n");
        fprintf(file, "+-----------+-------------------+-----------+-------------------+-----------+-------------------+-----------+-------------------+-----------+-------------------+-----------+\n");
        fprintf(file, "| Dimension | Time              | isSorted? | Time              | isSorted? | Time              | isSorted? | Time              | isSorted? | Time              | isSorted? |\n");
        fprintf(file, "+-----------+-------------------+-----------+-------------------+-----------+-------------------+-----------+-------------------+-----------+-------------------+-----------+\n");

	int s = seed;
	for(int i = minLength; i < maxLength; i=i+step){
		
		seed = s;	//faccio il reset del seed in modo da avere per IS, MS, HS sempre lo 
				//stesso array di numeri random, perchè se il seed è uguale ottengo sempre gli stessi valori
		timeIS = singleExperiment(i, maxInstances, "IS");
		seed = s;
		timeMS = singleExperiment(i, maxInstances, "MS");
		seed = s;
		timeHS = singleExperiment(i, maxInstances, "HS");
		seed = s;
		timeQS = singleExperiment(i, maxInstances, "QS");
		seed = s;
		timeTQS = singleExperiment(i, maxInstances, "TQS");

		fprintf(file, "| %9d | %17f | %9s | %17f | %9s | %17f | %9s | %17f | %9s | %17f | %9s |\n",
                i,
                (double) timeIS, isSortedIS ? "true" : "false",      // insertionSort
                (double) timeMS, isSortedMS ? "true" : "false",      // mergeSort
                (double) timeHS, isSortedHS ? "true" : "false",      // hybridSort
		(double) timeQS, isSortedQS ? "true" : "false",     // quickSort
		(double) timeTQS, isSortedTQS ? "true" : "false");     // tailQuickSort

		s = s+1; //cambio il seed per avere dati diversi
	}
	fclose(file);
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

void merge(int* A, const int low, const int mid, const int high) {
	int n1 = mid-low+1; //numero di elementi nel primo dei nuovi array (parte sinistra dell'array originale)
	int n2 = high-mid; //numero di elementi nel secondo dei nuovi array (parte destra dell'array originale)
	int* L = (int*) malloc(n1*sizeof(int)); //alloco due array di dimensione n1 e n2, rispettivamente
	int* R = (int*) malloc((n2+1)*sizeof(int));
	for(int i=0; i<=n1 ;i++){  //copio sugli array L e R allocati sopra, gli elementi di A
        	L[i]=A[low+i];      //su L andranno i valori da A[low] a A[q]
    	}
    	for(int j=0; j<=n2; j++){  //su R i valori da A[q+1] a A [high].
    		R[j]=A[mid+j+1];
    	}
    	int i=0,j=1;
    	int ALenght=high-low;
    	for (int k=0; k<=ALenght+1; k++){
    		if (i<=n1) {
        		if(j<=n2){
        		        if(L[i]<=R[j]){
        		        	A[k]=L[i];
					i++;
        	        	} 
				else {
					A[k]=R[j];
					j++;
				}
        	    	} 
			else {
				A[k]=L[i];
				i++;
			}
        	} 
		else {
			A[k]=R[j];
			j++;
		}
	}
}

void mergeSort(int* A, const int low, const int high) {
    	if(low<high){
    		int mid=((low+high)/2);     //punto medio tra gli indici dell'array
    		mergeSort(A,low,mid);       //chiama la funzione sulla sola metà destra
    		mergeSort(A,mid+1,high);    //chiama la fuzione sulla sola metà sinistra -> è come se da un array iniziale A, "creassi" due array.
    		merge(A,low,mid,high);  //quando low=high, ossia l'array ha un solo elemento (ed è quindi ordinato), l'ultima chiamata ricorsiva si chiude e il controllo 
    	    	//torna al chiamante, che fa partire merge che una chiamata dopo l'altra unisce gli array parziali (e ordinati) a due a due, fino ad otennere un unico array finale ordinato.
    	}
}

void hybridSort(int* A, const int low, const int high) {
	if((high-low+1)>threshold){
		int mid=(high+low)/2;
	        hybridSort(A,low,mid);
	        hybridSort(A,mid+1,high);
	        merge(A,low,mid,high);
	}
	else {
		adaptedInsertionSort(A,low, high);
	}
}

void adaptedInsertionSort(int* A, const int low, const int high) { //passo low e high invece della lunghezza
	int key;
	int i;
	for(int j=low+1; j<=high; j++){ // minore o UGUALE!!
		key=A[j];
		i = j-1;
		while(i>=low && A[i]>key){
		A[i+1]=A[i];
		i--;
		}
		A[i+1]=key;
	}
	return;
}

void quickSort(int* A, int low, int high) {
	if(low < high) {
		int q = partition(A, low, high);
		quickSort(A, low, q-1);
		quickSort(A, q+1, high);
	}
}

void tailQuickSort(int* A, int low, int high) {
	while(low < high) {
		int length = high - low;
		if(length < threshold2){
			insertionSort(A, length);
		}
		else {
			int q = medianOfThreePartition(A, low, high);
			tailQuickSort(A, low, q-1);
			tailQuickSort(A, q+1, high);
			low = q-1;
		}
	}
}

int partition(int* A, int low, int high) {
	int x = A[high];			//pivot		
	int i = low-1;
	for(int j = low; j < high; j++) {
		if(A[j] <= x) {
			i++;
			swapValue(A, i, j);
		}
	}
	swapValue(A, i+1, high);
	return i+1;
}

void medianOfThreeQuickSort(int* A, int low, int high) {
	if(low < high) {
		int m = medianOfThreePartition(A, low, high);
		quickSort(A, low, m-1);
		quickSort(A, m+1, high);
	}
}

int medianOfThreePartition(int* A,int low,int high) {
	int r = (low+high)/2;
	int m = medianOfThree(A, low, high, r);
	swapValue(A, m, high);
	int x = A[high];
	int i = low - 1;

	for(int j = low; j < high; j++) {
		if(A[j] <= x) {
			i++;
			swapValue(A, i, j);
		}
	}
	swapValue(A, i+1, high);
	return i+1;
}

void swapValue(int* A, int a, int b){
	int t = A[a];
	A[a] = A[b];
	A[b] = t;
}

int medianOfThree(int* A, int i, int j, int k) {
	if(A[i] > A[j]) {
		if(A[j] >= A[k]) return j;
		else if(A[i] < A[k]) return i;
		else return k;
	}
	else {
		if(A[i] >= A[k]) return i;
		else if(A[j] < A[k]) return k;
		else  return k;
	}
}

bool isSorted(int* A, int n) {		//controlla che l'array che torna da insertionSort sia effettivamente ordinato
	for (int i = 0; i < n-1; i++) {
		if (A[i] > A[i+1]) return false;
	}
    	return true;
}