#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LIMIT 30

int** fill_arr(int **m, int r, int c){
	for (int i = 0; i < r; i++){
		for (int j = 0; j < c; j++){
			m[i][j] = i*j;
		}
	}
	return m;
}

int** create_arr(int r, int c){
	int **m = malloc(r * sizeof(int*));
	for (int i = 0; i < r; i++){
		m[i] = malloc(c * sizeof(int));
	}
	m = fill_arr(m, r, c);
	return m;
}


void pr(int **m, int r, int c){
	for (int i = 0; i < r; i++){
		for (int j = 0; j < c; j++){
		   printf("%.2d ", m[i][j]);
		}
		printf("\n");
	}
}

void free_matrix(int **m, int r){
	for (int i = 0; i < r; i++){
		free(m[i]);
	}
	free(m);
}

int parse(int sk){
	int status;
	int temp;
	status = scanf("%d", &sk);
	while(status!=1){
		while((temp = getchar()) != EOF && temp != '\n');
		printf("Turi būti skaičius: ");
		status = scanf("%d", &sk);
	}
	return sk;
}


int main()
{
	int eil, stulp;
	char asmuo[MAX_LIMIT];
	char matrica[MAX_LIMIT];
	
	printf("Koks jūsų vardas, pavardė (iki %d simbolių)?: ", MAX_LIMIT);
	fgets(asmuo, MAX_LIMIT, stdin);
	asmuo[strlen(asmuo) - 1] = '\0';


	printf("Kaip norite pavadinti matricą (iki %d simbolių)?: ", MAX_LIMIT);
	fgets(matrica, MAX_LIMIT, stdin);
	matrica[strlen(matrica) - 1] = '\0';


	printf("Kiek eilučių?: ");
	eil = parse(eil);
	
	printf("Kiek stulpelių?: ");
	stulp = parse(stulp);
	
	int **arr = create_arr(eil, stulp);
	printf("Esate: %s\n", asmuo);
	printf("Jūsų matricos pavadinimas \"%s\"\n", matrica);

	printf("Matrica atrodo taip: \n");
	pr(arr, eil, stulp);

	free_matrix(arr, eil);

	return 0;
}
