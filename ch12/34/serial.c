#include "csapp.h"

#define M 256
#define N 512

long A[M][N] = {{}};
long B[N][M] = {{}};
long C[M][N] = {{}};

int main() {
  for (int i = 0; i < M; ++i) {
    for (int j = 0; j < N; ++j) {
      for (int k = 0; k < N; ++k) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
}
