#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int **A;
long long **Ai;

// Allocation
void allocate(int n, int m) {
    A = malloc(n * sizeof(int*));
    Ai = malloc(n * sizeof(long long*));

    for(int i = 0; i < n; i++) {
        A[i] = malloc(m * sizeof(int));
        Ai[i] = malloc(m * sizeof(long long));
    }
}

// Initialisation
void generate(int n, int m) {
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            A[i][j] = rand() % 10;
}

// Version parallèle
void compute_parallel(int n, int m) {

    // Phase 1: lignes
    #pragma omp parallel for
    for(int i = 0; i < n; i++) {
        Ai[i][0] = A[i][0];
        for(int j = 1; j < m; j++)
            Ai[i][j] = Ai[i][j-1] + A[i][j];
    }

    // Phase 2: colonnes
    #pragma omp parallel for
    for(int j = 0; j < m; j++) {
        for(int i = 1; i < n; i++)
            Ai[i][j] += Ai[i-1][j];
    }
}

// Version séquentielle (pour comparer)
void compute_seq(int n, int m) {
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

// Test complet
void test(int n, int m) {

    printf("\n===== Size %dx%d =====\n", n, m);

    allocate(n, m);
    generate(n, m);

    // Temps séquentiel
    double t1 = omp_get_wtime();
    compute_seq(n, m);
    double t2 = omp_get_wtime();

    double Tseq = t2 - t1;

    printf("Sequential time = %f\n", Tseq);

    // Threads testés
    int threads_list[] = {1, 2, 4, 8};

    for(int k = 0; k < 4; k++) {

        int p = threads_list[k];

        omp_set_num_threads(p);

        double t3 = omp_get_wtime();
        compute_parallel(n, m);
        double t4 = omp_get_wtime();

        double Tpar = t4 - t3;

        double speedup = Tseq / Tpar;
        double efficiency = speedup / p;

        printf("Threads = %d | Time = %f | Speedup = %f | Efficiency = %f\n",
               p, Tpar, speedup, efficiency);
    }
}

int main() {

    // tailles réalistes
    test(1000, 1000);
    test(2000, 2000);

    return 0;
}