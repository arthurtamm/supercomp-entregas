#include <iostream>
#include <mpi.h>
#include <cstring>
using namespace std;

int main(int argc, char **argv) {
    char message[20];
    int i, rank, size, tag = 99;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0){
        strcpy(message, "Hello");
        cout << "Process " << to_string(rank) << ": " << message << endl;
        MPI_Send(message, strlen(message), MPI_CHAR, 1, tag, MPI_COMM_WORLD);
    } else {
        MPI_Recv(message, 20, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);
        strcpy(message, "Hi");
        cout << "Process " << to_string(rank) << ": " << message << endl;
        MPI_Send(message, strlen(message), MPI_CHAR, 0, tag, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}