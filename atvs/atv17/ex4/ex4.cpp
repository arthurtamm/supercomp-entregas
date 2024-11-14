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
    const int target = 7;
    vector<int> data(N);

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            data[i] = i % 50;
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

    vector<int> local_positions;
    #pragma omp parallel for
    for (int i = 0; i < local_chunk_size; i++) {
        if (local_data[i] == target) {
            #pragma omp critical
            local_positions.push_back(i + rank * base_chunk_size + (rank == 0 ? 0 : remainder));
        }
    }

    int local_count = local_positions.size();
    vector<int> all_counts(size);
    MPI_Gather(&local_count, 1, MPI_INT, &all_counts[0], 1, MPI_INT, 0, MPI_COMM_WORLD);

    vector<int> displs(size, 0);
    int total_count = 0;
    if (rank == 0) {
        for (int i = 0; i < size; i++) {
            displs[i] = total_count;
            total_count += all_counts[i];
        }
    }

    vector<int> all_positions(total_count);
    MPI_Gatherv(local_positions.data(), local_count, MPI_INT, all_positions.data(), all_counts.data(), displs.data(), MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Positions of target " << target << " found at: ";
        for (int pos : all_positions) {
            cout << pos << " ";
        }
        cout << endl;
    }

    MPI_Finalize();
    return 0;
}
