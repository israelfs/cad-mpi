#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <iomanip>
#include <chrono>
#include <mpi.h>

using namespace std;

#define N 10000 // rows
#define M 10000 // columns
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

  int maior = -INF;
  int menor = INF;
  long long int soma = 0;
  vector<long long int> soma_linha(N, 0);
  vector<long long int> soma_coluna(M, 0);

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

  for (int i = start; i < end; i++){
    for (int j = 0; j < M; j++){
      soma += matrix[i][j];
      soma_linha[i] += matrix[i][j];
      soma_coluna[j] += matrix[i][j];
      maior = max(maior, matrix[i][j]); 
      menor = min(menor, matrix[i][j]);
    }
  }

  if (rank == 0){
    auto time_end = std::chrono::high_resolution_clock::now();

    double time_taken = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start).count();

    cout << "Time taken by the parallel program is: " << fixed << time_taken / 1000 << " sec " << endl;
    cout << "Maior: " << maior << endl;
    cout << "Menor: " << menor << endl;
    cout << "Soma: " << soma << endl;
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