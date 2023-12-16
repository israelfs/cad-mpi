#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <iomanip>
#include <chrono>
#include <mpi.h>

using namespace std;

#define N 20000 // rows
#define M 20000 // columns
#define INF (1 << 30)

int main(int argc, char *argv[]){
  // create the matrix as a vector of vectors of integers
  vector<vector<int>> matrix(N, vector<int>(M));

  // fill the matrix with random numbers
  for (int i = 0; i < N; i++){
    for (int j = 0; j < M; j++){
      matrix[i][j] = rand() % 1000;
    }
  }

  // Local variables for each process
  int local_maior = -INF;
  int local_menor = INF;
  long long int local_soma = 0;
  vector<long long int> local_soma_linha(N, 0);
  vector<long long int> local_soma_coluna(M, 0);

  // Perform reduction operations to find global maximum, minimum, and sum
  int global_maior, global_menor;
  long long int global_soma;
  vector<long long int> global_soma_linha(N, 0);
  vector<long long int> global_soma_coluna(M, 0);

  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); // get current process id
  MPI_Comm_size(MPI_COMM_WORLD, &size); // get number of processes

  int chunk = N / size;
  int start = rank * chunk;
  int end = start + chunk;

  auto time_start = std::chrono::high_resolution_clock::now();
  if (rank == 0){
    time_start = std::chrono::high_resolution_clock::now();
  }

  for (int i = start; i < end; i++) {
    for (int j = 0; j < M; j++) {
      local_soma += matrix[i][j];
      local_maior = max(local_maior, matrix[i][j]);
      local_menor = min(local_menor, matrix[i][j]);
      local_soma_linha[i] += matrix[i][j];
      local_soma_coluna[j] += matrix[i][j];
    }
  }

  MPI_Reduce(&local_maior, &global_maior, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Reduce(&local_menor, &global_menor, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
  MPI_Reduce(&local_soma, &global_soma, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

  // Gather local_soma_linha from all processes to global_soma_linha
  MPI_Gather(local_soma_linha.data(), chunk, MPI_LONG_LONG, global_soma_linha.data(), chunk, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

  // Perform an allreduce operation to get the sum of each column across all processes
  MPI_Allreduce(local_soma_coluna.data(), global_soma_coluna.data(), M, MPI_LONG_LONG, MPI_SUM, MPI_COMM_WORLD);

  if (rank == 0){
    auto time_end = std::chrono::high_resolution_clock::now();

    double time_taken = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start).count();

    cout << "Time taken by the parallel program is: " << fixed << time_taken / 1000 << " sec " << endl;
    cout << "Maior: " << global_maior << endl;
    cout << "Menor: " << global_menor << endl;
    cout << "Soma: " << global_soma << endl;
  // for(int i = 0; i < N; i++){
  //   cout << "Soma linha " << i << ": " << soma_linha[i] << endl;
  // }
  // for(int i = 0; i < M; i++){
  //   cout << "Soma coluna " << i << ": " << soma_coluna[i] << endl;
  // }
  }

  MPI_Finalize();
  return 0;
}