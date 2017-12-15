#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define PARKING 0
#define COCHE 2

int main(int argc, char** argv) {
  MPI_Status mpistatus;
  // INICIAMOS EL MPI
  MPI_Init(&argc, &argv);

  int myrank;
  int error = MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  MPI_Barrier(MPI_COMM_WORLD);
  while(1) {
    MPI_Send(&myrank, 1, MPI_INT, 0, COCHE, MPI_COMM_WORLD);

    srand(time(NULL));
    int r = rand() % (myrank+1);
    sleep(myrank+r);
  }

  // Terminar MPI
  MPI_Finalize();
}
