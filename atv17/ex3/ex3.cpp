#include <iostream>
#include <mpi.h>
#include <omp.h>
#include <vector>
using namespace std;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 100;
    vector<int> data(N);

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            data[i] = i + 1;
        }
    }

    int base_chunk_size = N / size;
    int remainder = N % size;
    int local_chunk_size = (rank == 0) ? base_chunk_size + remainder : base_chunk_size;

    vector<int> local_data(local_chunk_size);

    if (rank == 0) {
        MPI_Scatter(&data[remainder], base_chunk_size, MPI_INT, &local_data[0], base_chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
        for (int i = 0; i < remainder; i++) {
            local_data[i] = data[i];
        }
    } else {
        MPI_Scatter(nullptr, base_chunk_size, MPI_INT, &local_data[0], base_chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
    }

    int local_sum = 0;
    #pragma omp parallel for reduction(+:local_sum)
    for (int i = 0; i < local_chunk_size; i++) {
        local_sum += local_data[i];
    }

    int total_sum = 0;
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double mean = static_cast<double>(total_sum) / N;
        cout << "Total sum: " << total_sum << endl;
        cout << "Mean: " << mean << endl;
    }

    MPI_Finalize();
    return 0;
}
