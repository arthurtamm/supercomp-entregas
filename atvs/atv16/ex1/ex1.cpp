#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int ARRAY_SIZE = 100;
    int array[ARRAY_SIZE];

    if (rank == 0) {
        for (int i = 1; i <= ARRAY_SIZE; i++) {
            array[i] = i;
        }
    }
    int chunk_size = ARRAY_SIZE / size;
    int recv_array[chunk_size];

    MPI_Scatter(array, chunk_size, MPI_INT, recv_array, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    int sum = 0;
    for (int i = 0; i < chunk_size; i++) {
        sum += recv_array[i];
    }
    // cout << endl;
    double avg = static_cast<double>(sum) / chunk_size;

    double *all_averages = nullptr;
    if(rank == 0){
        all_averages = new double[size];
    }

    MPI_Gather(&avg, 1, MPI_DOUBLE, all_averages, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double final_avg = 0.0;
        for (int i = 0; i < size; i++) {
            final_avg += all_averages[i];
        }
        final_avg /= size;

        cout << "Final average: " << final_avg << endl;

        delete[] all_averages;
    }

    MPI_Finalize();
    return 0;
}