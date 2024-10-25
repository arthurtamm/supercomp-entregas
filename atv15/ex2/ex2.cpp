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
        strcpy(message, "Hi, rank 1!");
        cout << "Process " << to_string(rank) << " sending message: " << message << endl;
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, 1, tag, MPI_COMM_WORLD);
        MPI_Recv(message, 20, MPI_CHAR, size-1, tag, MPI_COMM_WORLD, &status);
        cout << "Process " << to_string(rank) << " receiving message: " << message << endl;
    } if (rank < size-1){
        MPI_Recv(message, 20, MPI_CHAR, rank-1, tag, MPI_COMM_WORLD, &status);
        cout << "Process " << to_string(rank) << " receiving message: " << message << endl;

        ostringstream oss;
        oss << "Hi, rank " << rank+1 << "!";
        string str = oss.str();
        strcpy(message, str.c_str());
        cout << "Process " << to_string(rank) << " sending message: " << message << endl;
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, rank+1, tag, MPI_COMM_WORLD);
    
    } else {
        MPI_Recv(message, 20, MPI_CHAR, rank-1, tag, MPI_COMM_WORLD, &status);
        cout << "Process " << to_string(rank) << " sending message: " << message << endl;
        strcpy(message, "Hi, rank 0!");
        cout << "Process " << to_string(rank) << " sending message: " << message << endl;
        MPI_Send(message, 4, MPI_CHAR, 0, tag, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}