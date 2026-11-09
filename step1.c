#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Allocation 
int **allocate_int(int n, int m) {
    int **mat = malloc(n * sizeof(int *));
    for(int i = 0; i < n; i++)
        mat[i] = malloc(m * sizeof(int));
    return mat;
}

long long **allocate_ll(int n, int m) {
    long long **mat = malloc(n * sizeof(long long *));
    for(int i = 0; i < n; i++)
        mat[i] = malloc(m * sizeof(long long));
    return mat;
}

// Initialisation 
void generate(int **A, int n, int m) {
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            A[i][j] = rand() % 10;
}

// Calcul Ai
void compute(int **A, long long **Ai, int n, int m) {

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {

            if(i == 0 && j == 0)
                Ai[i][j] = A[i][j];

            else if(i == 0)
                Ai[i][j] = Ai[i][j-1] + A[i][j];

            else if(j == 0)
                Ai[i][j] = Ai[i-1][j] + A[i][j];

            else
                Ai[i][j] = A[i][j] + Ai[i-1][j] + Ai[i][j-1] - Ai[i-1][j-1];
        }
    }
}

// Libération mémoire
void free_matrix_int(int **mat, int n) {
    for(int i = 0; i < n; i++)
        free(mat[i]);
    free(mat);
}

void free_matrix_ll(long long **mat, int n) {
    for(int i = 0; i < n; i++)
        free(mat[i]);
    free(mat);
}

// Test pour une taille
void test(int n, int m) {

    printf("\nTesting size %d x %d\n", n, m);

    int **A = allocate_int(n, m);
    long long **Ai = allocate_ll(n, m);

    generate(A, n, m);

    clock_t start = clock();

    compute(A, Ai, n, m);

    clock_t end = clock();

    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Execution time: %f seconds\n", time_taken);

    free_matrix_int(A, n);
    free_matrix_ll(Ai, n);
}

int main() {

    srand(time(NULL));

    test(3, 3);
    test(100, 100);
    test(1024, 1024);
    test(3000, 3000);   
    test(5000, 5000);   
    test(10000, 10000); 

    return 0;
}