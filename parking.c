#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define PARKING 0
#define BOOLEAN 3

int** parking;
int plazas, plantas;
int libres;
int boolean = 1;

typedef struct {
  int tipo;
  int rank;
} vehiculo;

void imprimirMatriz() {
  int i, j;
  for(i=0; i<plazas; i++) {
    for(j=0; j<plantas; j++) {
      printf("%d\t", parking[i][j]);
    }
    printf("\n");
  }
}

int buscarPlazas(int *plaza, int *planta, vehiculo v) {
  MPI_Status mpistatus;
  if(v.tipo == 1) {
    //HAY QUE BUSCAR DOS SITIOS CONTINUOS
    int i, j;
    for(i=0; i<plazas; i++) {
      for(j=0; j<plantas; j++) {
        if(parking[i][j] == v.rank) {
          parking[i][j] = 0;
          parking[i][j+1] = 0;
          libres = libres + 2;
          printf("SALIDA: Camion %d saliendo. Plazas libres: %d\n", v.rank, libres);
          return 0;
        }
      }
    }
    for(i=0; i<plazas; i++) {
      for(j=0; j<plantas; j++) {
        if(parking[i][j] == 0 && parking[i][j+1] == 0 && i != (plazas-1) && j != (plantas-1)) {
          *plaza = i;
          *planta = j;
          parking[i][j] = v.rank;
          parking[i][j+1] = v.rank;
          libres = libres - 2;
          printf("ENTRADA: Camion %d aparca en %d, %d y en %d, %d. Plazas libres: %d\n",v.rank, *plaza, *planta, *plaza, (*planta)++, libres);
          return 0;
        }
      }
    }
  } else {
    // HAY QUE BUSCAR UN SITIO
    int i, j;
    for(i=0; i<plazas; i++) {
      for(j=0; j<plantas; j++) {
        if(parking[i][j] == v.rank) {
          parking[i][j] = 0;
          libres++;
          printf("SALIDA: Coche %d saliendo. Plazas libres: %d\n", v.rank, libres);
          return 0;
        }
      }
    }
    for(i=0; i<plazas; i++) {
      for(j=0; j<plantas; j++) {
        if(parking[i][j] == 0) {
          *plaza = i;
          *planta = j;
          parking[i][j] = v.rank;
          libres--;
          printf("ENTRADA: Coche %d aparca en %d, %d. Plazas libres: %d\n",v.rank, *plaza, *planta, libres);
          return 0;
        }
      }
    }
  }
}

void camiones(int ranking) {
  int plaza=0, planta=0;
  vehiculo v;
  v.tipo=1;
  v.rank=ranking;
  buscarPlazas(&plaza, &planta, v);
}

void coches(int ranking) {
  int plaza=0, planta=0;
  vehiculo v;
  v.tipo=2;
  v.rank=ranking;
  buscarPlazas(&plaza, &planta, v);
}

int main(int argc, char** argv) {
  MPI_Status mpistatus;
  // INICIAMOS EL MPI
  MPI_Init(&argc, &argv);
  if(argc != 3) {
    printf("NO HAS INTRODUCIDO SUFICIENTES PARAMETROS.\nEL PRIMERO SON LAS PLAZAS Y EL SEGUNDO LAS PLANTAS\n\n");
    MPI_Finalize();
    return 0;
  }

  plazas = atoi(argv[1]);
  plantas = atoi(argv[2]);
  libres = plazas * plantas;

  int k;
  parking = (int**) malloc(sizeof(int*) * plazas);
  for(k=0; k<plantas; k++)
    parking[k] = (int *) malloc(sizeof(int) * plantas);

  int i, j;
  for(i=0; i<plazas; i++) {
    for(j=0; j<plantas; j++) {
      parking[i][j] = 0;
    }
  }

  int quien;
  MPI_Barrier(MPI_COMM_WORLD);
  while(1) {
    MPI_Recv(&quien, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &mpistatus);
    if(mpistatus.MPI_TAG == 1) {
      camiones(quien);
      imprimirMatriz();
    } else {
      coches(quien);
      imprimirMatriz();
    }
    fflush(stdout);
  }
  // Terminar MPI
  MPI_Finalize();
  return 0;
}
