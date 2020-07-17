#include <stdio.h>
#include <stdlib.h>

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
	// Rule 1: scanf() is not for reading input, it's for parsing input.
	// The important thing is: scanf() never reads anything it cannot parse. 
	// In our example, we tell scanf() to parse a number, using the %d conversion. 
	// Sure enough, abc is not a number, and as a consequence, abc is not even read
	// The next call to scanf() will again find our unread input and, again, can't parse it
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
	int rows, cols;
	
	printf("Kiek rows?: ");
	rows = parse(rows);
	
	printf("Kiek cols?: ");
	cols = parse(cols);
	
	int **arr = create_arr(rows, cols);
	pr(arr, rows, cols);

	free_matrix(arr, rows);
	return 0;
}
