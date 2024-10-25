#include <iostream>
#include <sstream>
#include <cstring>
#include <mpi.h>
using namespace std;

int main(int argc, char **argv) {
    int number;
    int i, rank, size, tag = 99;
    MPI_Status status;


    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0){
        cout << "Number of processes: " << size << endl;
        cout << "Sum must be 2" << endl;
        number = 0;
        MPI_Send(&number, 1, MPI_INT, rank+1, tag, MPI_COMM_WORLD);
    } if (rank == size-1){
        MPI_Recv(&number, 1, MPI_INT, rank-1, tag, MPI_COMM_WORLD, &status);
        cout << "Process " << to_string(rank) << " received number " << number << endl;
    } else {
        MPI_Recv(&number, 1, MPI_INT, rank-1, tag, MPI_COMM_WORLD, &status);
        number++;
        MPI_Send(&number, 1, MPI_INT, rank+1, tag, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}