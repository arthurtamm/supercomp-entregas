#include <iostream>
#include <mpi.h>
#include <random>
#include <omp.h>
#include <iomanip>
using namespace std;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int ARRAY_SIZE = 100;
    int array[ARRAY_SIZE];

    int chunk_size = ARRAY_SIZE / size;
    int recv_array[chunk_size];

    int max = 0;

    if (rank == 0) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, 100);

        cout << "Original array:" << endl;
        for (int i = 0; i < ARRAY_SIZE; i++) {
            array[i] = dist(gen);
            cout << array[i] << " ";
        }
        cout << endl << endl;

        max = array[0];
        #pragma omp parallel for reduction(max:max)
        for (int i = 1; i < ARRAY_SIZE; i++) {
            if (array[i] > max) {
                max = array[i];
            }
        }
    }

    MPI_Bcast(&max, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Scatter(array, chunk_size, MPI_INT, recv_array, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    double local_norm[chunk_size];
    for (int i = 0; i < chunk_size; i++) {
        local_norm[i] = static_cast<double>(recv_array[i]) / max;
    }

    double normalized_array[ARRAY_SIZE];

    MPI_Gather(local_norm, chunk_size, MPI_DOUBLE, normalized_array, chunk_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);


    if (rank == 0) {
        cout << fixed << setprecision(2);
        cout << "Normalized array:" << endl;
        for (int i = 0; i < ARRAY_SIZE; i++) {
            cout << normalized_array[i] << " ";
        }
        cout << endl;
    }

    MPI_Finalize();
    return 0;
}