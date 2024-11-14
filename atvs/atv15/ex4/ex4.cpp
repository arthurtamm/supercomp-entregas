#include <iostream>
#include <sstream>
#include <cstring>
#include <mpi.h>
using namespace std;

int main(int argc, char **argv) {
    char message[30];
    int i, rank, size, tag = 99;
    MPI_Status status;


    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0){
        for (int i = 1; i < size; i++) {
            string temp = "Message to process " + to_string(i);
            strcpy(message, temp.c_str());
            cout << "Process " << to_string(rank) << " sending message: " << message << endl;
            MPI_Send(message, strlen(message) + 1, MPI_CHAR, i, tag, MPI_COMM_WORLD);
        }

    } else {
        MPI_Recv(message, 30, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);
        cout << "Process " << to_string(rank) << " receiving message: " << message << endl;
    }

    MPI_Finalize();
    return 0;
}