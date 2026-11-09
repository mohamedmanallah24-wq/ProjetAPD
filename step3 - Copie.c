#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MAXN 2000
#define MAXM 2000

int A[MAXN][MAXM];
long long Ai[MAXN][MAXM];

void generate(int n, int m) {
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            A[i][j] = rand() % 10;
}

void compute_mpi(int n, int m, int rank, int size) {

    int rows = n / size;
    int start = rank * rows;
    int end = (rank == size-1) ? n : start + rows;

    for(int i = start; i < end; i++) {

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

        // communication entre processus
        if(i == end-1 && rank != size-1) {
            MPI_Send(Ai[i], m, MPI_LONG_LONG, rank+1, 0, MPI_COMM_WORLD);
        }

        if(i == start && rank != 0) {
            MPI_Recv(Ai[i-1], m, MPI_LONG_LONG, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
}

void test(int n, int m, int rank, int size) {

    if(rank == 0) {
        generate(n, m);
    }

    MPI_Bcast(A, MAXN*MAXM, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    double t1 = MPI_Wtime();

    compute_mpi(n, m, rank, size);

    MPI_Barrier(MPI_COMM_WORLD);

    double t2 = MPI_Wtime();

    if(rank == 0) {
        printf("Size %dx%d | Proc = %d | Time = %f sec\n", n, m, size, t2 - t1);
    }
}

int main(int argc, char *argv[]) {

    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // tailles testées
    test(500, 500, rank, size);
    test(1000, 1000, rank, size);
    test(1500, 1500, rank, size);

    MPI_Finalize();
    return 0;
}