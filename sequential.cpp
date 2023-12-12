#include <iostream>
#include <vector>
#include <math.h>
#include <chrono>

using namespace std;

#define N 10000 // rows
#define M 10000 // columns
#define INF (1<<30)

void manipulate_matrix(vector<vector<int>>& matrix, int& maior, int& menor, long long int& soma, vector<long long int>& soma_linha, vector<long long int>& soma_coluna){
  for(int i = 0; i < N; i++){
    for(int j = 0; j < M; j++){
      soma += matrix[i][j];
      soma_linha[i] += matrix[i][j];
      soma_coluna[j] += matrix[i][j];
      maior = max(maior, matrix[i][j]);
      menor = min(menor, matrix[i][j]);
    }
  } 
}

int main(int argc, char *argv[]) {
  
  // create the matrix as a vector of vectors of integers
  vector<vector<int>> matrix(N, vector<int>(M));

  // fill the matrix with random numbers
  for(int i = 0; i < N; i++){
    for(int j = 0; j < M; j++){
      matrix[i][j] = rand() % 1000;
    }
  }

  int maior = -INF;
  int menor = INF;
  long long int soma = 0;
  vector<long long int> soma_linha(N, 0);
  vector<long long int> soma_coluna(M, 0);

  auto start = std::chrono::high_resolution_clock::now();

  manipulate_matrix(matrix, maior, menor, soma, soma_linha, soma_coluna);

  auto end = std::chrono::high_resolution_clock::now();

  double time_taken = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

  cout << "Time taken by the sequential program is: " << fixed << time_taken/1000 << " sec " << endl;

  cout << "Maior: " << maior << endl;
  cout << "Menor: " << menor << endl;
  cout << "Soma: " << soma << endl;
  // for(int i = 0; i < N; i++){
  //   cout << "Soma linha " << i << ": " << soma_linha[i] << endl;
  // }
  // for(int i = 0; i < M; i++){
  //   cout << "Soma coluna " << i << ": " << soma_coluna[i] << endl;
  // }
  return 0;
}