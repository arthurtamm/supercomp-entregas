#include <iostream>
#include <mpi.h>
#include <cmath>
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

    if (rank == 0) {
        for (int i = 0; i < ARRAY_SIZE; i++) {
            array[i] = i;
        }
    }

    MPI_Scatter(array, chunk_size, MPI_INT, recv_array, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    int sum = 0;
    for (int i = 0; i < chunk_size; i++) {
        sum += recv_array[i];
    }
    double avg = static_cast<double>(sum) / chunk_size;
    double var = 0.0;
    for (int i = 0; i < chunk_size; i++) {
        var += pow((recv_array[i] - avg), 2) / chunk_size;
    }
    
    double *all_averages = nullptr;
    double *all_vars = nullptr;
    if(rank == 0){
        all_averages = new double[size];
        all_vars = new double[size];
    }
    
    MPI_Gather(&avg, 1, MPI_DOUBLE, all_averages, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Gather(&var, 1, MPI_DOUBLE, all_vars, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);


    if (rank == 0) {
        double global_mean = 0.0;

        for (int i = 0; i < size; i++) {
            global_mean += all_averages[i] * chunk_size;
        }
        global_mean /= ARRAY_SIZE;


        double global_var = 0.0;
        for (int i = 0; i < size; i++) {
            global_var += all_vars[i] * chunk_size;
            global_var += pow(all_averages[i] - global_mean, 2) * chunk_size;
        }
        global_var /= ARRAY_SIZE;

        double std_dev = sqrt(global_var);

        cout << "Global Standard Deviation: " << std_dev << endl;

        delete[] all_averages;
        delete[] all_vars;
    }

    MPI_Finalize();
    return 0;
}