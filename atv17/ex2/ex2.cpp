#include <iostream>
#include <mpi.h>
#include <omp.h>
using namespace std;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 10;
    int data[N][N];

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                data[i][j] = i + j;
            }
        }
    }

    double start_time = MPI_Wtime();

    int base_chunk_size = N / size;
    int remainder = N % size;

    int local_data[base_chunk_size][N];

    MPI_Scatter(data, base_chunk_size * N, MPI_INT, local_data, base_chunk_size * N, MPI_INT, 0, MPI_COMM_WORLD);

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < base_chunk_size; i++) {
        for (int j = 0; j < N; j++) {
            local_data[i][j] *= local_data[i][j];
        }
    }

    // Processo 0 cuida dos dados restantes (excedentes)
    if (rank == 0 && remainder > 0) {
        for (int i = N - remainder; i < N; i++) {
            for (int j = 0; j < N; j++) {
                data[i][j] *= data[i][j];
            }
        }
    }

    MPI_Gather(local_data, base_chunk_size * N, MPI_INT, data, base_chunk_size * N, MPI_INT, 0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();
    double local_duration = end_time - start_time;
    double max_duration;
    MPI_Reduce(&local_duration, &max_duration, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Execution time for N = " << N << ": " << max_duration << " seconds" << endl;

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                cout << data[i][j] << " ";
            }
            cout << endl;
        }
    }

    MPI_Finalize();
    return 0;
}
