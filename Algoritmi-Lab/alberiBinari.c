//INCLUDO LE LIBRERIE
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <string.h>

//STRUTTURE DATI
typedef struct tree_node {
	int key;
	struct tree_node* parent;
	struct tree_node* left_child;
	struct tree_node* right_child;
} tree_node;

typedef struct tree {
	int cardinality;
	struct tree_node* root;
} tree;

//DICHIARAZIONI GLOBALI
clock_t tTot;
clock_t tStart;
clock_t tEnd;
clock_t tElapsed;
clock_t t;
int SEED = 13;
int min_keys = 1;
int max_keys = 100;
int max_search;
int max_delete;
int max_instances = 50;
int step = 10;
int percentage_search = 60;
tree T;
int maxRand = 500;

//DICHIARO LE FUNZIONI
double singleExperiment(int max_keys, int max_search, int max_delete, int max_instances);
void BSTTreeInsert(tree x, tree_node* k);
tree_node* BSTTreeSearch(tree_node* r, int k);
void BSTTreeDelete(tree T, tree_node* z);
void initializeTree(tree T);
tree_node* randomNode();
int randomKey();
void emptyTree(tree T);
void experiment(int min_keys, int max_keys);
bool isSorted(const int*, const int);
void BSTTreeTransplant(tree T, tree_node* z, tree_node* u);
tree_node* BSTTreeMinimum(tree_node* x);

//FUNZIONI PRINCIPALE
int main(){
	srand(SEED);

	experiment(min_keys, max_keys);

	return 0;
}

//IMPLEMENTO LE FUNZIONI
double singleExperiment(int max_keys, int max_search, int max_delete, int max_instances){
	tTot = 0;

	for(int i = 0; i < max_instances; i++){
		initializeTree(T);
		tStart = clock();
		for(int k = 0; k < max_keys; k++){
			BSTTreeInsert(T, randomNode());
		}
		for(int s = 0; s < max_search; s++){
			BSTTreeSearch(T.root, randomKey());
		}
		for(int d = 0; d < max_search; d++){
			BSTTreeDelete(T, randomNode());
		}
		tEnd = clock();
		tElapsed = tEnd - tStart;
		tTot = tTot + tElapsed;
		emptyTree(T);
	}
	t = tTot / max_keys;
	return t;
}

void experiment(min_keys, max_keys){

	FILE* file = fopen("result2nd.txt", "w");

	if (file == NULL){
		printf("File non aperto\n");
		exit(1);
	}

	fprintf(file, "+-----------+-------------------------------+\n");
        fprintf(file, "| ######### | Alberi Binari                 |\n");
	fprintf(file, "+-----------+-------------------+-----------+\n");
	fprintf(file, "| Dimension | Time              | isSorted? |\n");
	fprintf(file, "+-----------+-------------------+-----------+\n");

	for(int k = min_keys; k < max_keys; k+=step){
		srand(SEED);
		max_search = k * percentage_search/100;
		max_delete = k - max_search;
		t = singleExperiment(k, max_search, max_delete, max_instances);

		fprintf(file, "| %9d | %17f |\n", k, (double) t);

		printf("| Dimension | Time              | isSorted? |\n");
		printf("| %9d | %17f |\n", k, (double) t);

		SEED = SEED + 1;
	}
	fclose(file);
}

//IMPLEMENTO LE FUNZIONI:
/**
 * @brief Albri binari di ricerca
 */

tree_node* BSTTreeSearch(tree_node* x, int k){
	if(x == NULL || x->key == k) return x;
	if(k <= x->key) return BSTTreeSearch(x->left_child, k);
	else return BSTTreeSearch(x->right_child, k);
}

void BSTTreeInsert(tree T, tree_node* z){
	tree_node* y = NULL;
	tree_node* x = T.root;
	while(x !=  NULL){
		y = x;
		if(z->key <= x->key) x = x->left_child;
		else x = x->right_child;
	}
	z->parent = y;
	if(y != NULL) T.root = z;
	if(y != NULL && z->key <= y->key) y->left_child = z;
	if(y != NULL && z->key > y->key) y->right_child = z;
}

void BSTTreeDelete(tree T, tree_node* z){
	if(z->left_child == NULL) BSTTreeTransplant(T, z, z->right_child);
	if(z->left_child != NULL && z->right_child == NULL) BSTTreeTransplant(T, z, z->right_child);
	if(z->left_child != NULL && z->right_child != NULL){
		tree_node* y = BSTTreeMinimum(z->right_child);
		if(y->parent != z){
			BSTTreeTransplant(T, y, y->right_child);
			y->right_child = z->right_child;
			y->right_child->parent = y;
		}
		BSTTreeTransplant(T, z, y);
		y->left_child = z->left_child;
		y->left_child->parent = y;			
	}
}

void BSTTreeTransplant(tree T, tree_node* u, tree_node* v){
	if(u->parent == NULL) T.root = v;
	if(u->parent != NULL && u == u->parent->left_child) u->parent->left_child = v;
	if(u->parent != NULL && u == u->parent->right_child) u->parent->right_child = v;
	if(v != NULL) v->parent = u->parent;
}

tree_node* BSTTreeMinimum(tree_node* x){
	if(x->left_child == NULL) return x;
	return BSTTreeMinimum(x->left_child);
}

void initializeTree(tree T){
	T.cardinality = 1;
	tree_node* r = (tree_node*)malloc(sizeof(tree_node));
	T.root = r;
	T.root->key = 0;
	T.root->left_child = NULL;
	T.root->right_child = NULL;
}

void emptyTree(tree T){
	free(T.root);
	T.root = NULL;
	T.cardinality = 0;
}

tree_node* randomNode(){
	int a = rand() % maxRand + 1;
	tree_node* n = (tree_node*)malloc(sizeof(tree_node));
	n->key = a;
	return n;
}

int randomKey(){
	int a = rand() % maxRand + 1;
	return a;	
}
