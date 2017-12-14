#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define CAMION 1

int main(int argc, char** argv) {
  // INICIAMOS EL MPI
  MPI_Init(&argc, &argv);

  int myrank;
  int error = MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  MPI_Barrier(MPI_COMM_WORLD);
  while(1) {
    MPI_Send(&myrank, 1, MPI_INT, 0, CAMION, MPI_COMM_WORLD);

    srand(time(NULL));
    int r = rand() % 10;
    sleep(myrank);
  }

  // Terminar MPI
  MPI_Finalize();
}
