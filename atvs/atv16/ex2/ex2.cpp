#include <iostream>
#include <mpi.h>
#include <cmath>
using namespace std;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int iterations = 0;

    if (rank == 0) {
        iterations = 1000;
        cout << "Processo 0 definindo número de iterações: " << iterations << endl;
    }

    MPI_Bcast(&iterations, 1, MPI_INT, 0, MPI_COMM_WORLD);

    double local_sum = 0.0;
    for (int i=rank; i<iterations; i+=size) {
        local_sum += pow(-1, i) / (2 * i + 1);
    }

    double global_sum = 0.0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double pi = 4 * global_sum;
        cout << "Pi: " << pi << endl;
    }
    MPI_Finalize();
    return 0;
}