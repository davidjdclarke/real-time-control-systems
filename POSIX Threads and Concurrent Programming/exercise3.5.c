#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

struct thread_info {
  int ra, row, col;
  int * matrix_A;
  int * matrix_B;
  int * matrix_C;
};

typedef struct thread_info thread_info_t;

void *compute_C_ij(void *arg) {
  int i, j, ra, k, sum;
  int A, B, C;
  int * ptr_a, ptr_b, ptr_c;
  thread_info_t *info;

  info = (thread_info_t *)arg;

  i = info->row;
  j = info->col;
  ra = info->ra;
  /*
  ptr_a = info->matrix_A;
  A = *ptr_a;
  ptr_b = info->matrix_B;
  B = *ptr_b;
  ptr_c = info->matrix_C;
  C = *ptr_c;

  sum = 0;
  for (k = 0; k < ra; k++) {
    sum += A[i][k] * B[k][j];
  }
  */
}

int main () {
  pthread_t thread1, thread2, thread3, thread4;

  int ra = 18, ca = 16;
  int rb = 16, cb = 18;

  int A[ra][ca];
  int B[rb][cb];
  int C[ra][cb];

  int i, j;
  // Set A Matrix: Cij = i + j (i = 1, 2, ... 18, and j = 1, 2, ... , 18)
  printf("Matrix A: \n");
  for (i = 0; i < ra; i++) {
    for (j = 0; j < ca; j++) {
      A[i][j] = (i + 1) + (j + 1);
      printf("%d  ", A[i][j]);
    }
    printf("\n");
  }

  // Set B Matrix: Cij = i + 2j (i = 1, 2, ... 18, and j = 1, 2, ... , 18)
  printf("Matrix B: \n");
  for (i = 0; i < rb; i++) {
    for (j = 0; j < cb; j++) {
      B[i][j] = (i + 1) + 2 * (j + 1);
      printf("%d  ", B[i][j]);
    }
    printf("\n");


  thread_info_t info;

  info.matrix_A = &A;
  // info.matrix_B = 
  info.ra = ra;

  pthread_create(&thread1, NULL, &compute_C_ij, &info);


  // Calculate Matrix C
  int i, x, y, sum;
  for (x = 0; x < ra; x++) {
    for (y = 0; y < cb; y ++) {
      sum = 0;
      for (i = 0; i < ra; i++) {
        sum += A[x][i] * B[i][y];
      }
      C[x][y] = sum;
    }
  }

  // Print C Matrix
  printf("Matrix C: \n");
  for (i = 0; i < ra; i++) {
    for (j = 0; j < cb; j++) {
      printf("%d  ", C[i][j]);
    }
    printf("\n");
  }
  return 0;
  }
}
