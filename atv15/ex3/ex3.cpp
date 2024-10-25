#include <iostream>
#include <sstream>
#include <cstring>
#include <mpi.h>
using namespace std;

int main(int argc, char **argv) {
    char message[20];
    int i, rank, size, tag = 99;
    MPI_Status status;


    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0){
        strcpy(message, "Hi");
        cout << "Number of processes: " << to_string(size) << endl;
        cout << "Process " << to_string(rank) << " sending message: " << message << endl;
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, 1, tag, MPI_COMM_WORLD);
    } if (rank == size-1){
        MPI_Recv(message, 20, MPI_CHAR, rank-1, tag, MPI_COMM_WORLD, &status);
        cout << "Process " << to_string(rank) << " receiving message: " << message << endl;
    } else {
        MPI_Recv(message, 20, MPI_CHAR, rank-1, tag, MPI_COMM_WORLD, &status);
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, rank+1, tag, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}