#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

// Declare global variables
#define ra 18
#define ca 16
#define rb 16
#define cb 18
#define MAX_NUM_THREADS 1

int A[ra][ca];
int B[rb][cb];
int C[ra][cb];

// Information to be passed to threads
struct thread_info {
  int row, col;
};

typedef struct thread_info thread_info_t;

void *compute_C_ij(void *arg) {
  int i, j, k, sum, temp1, temp2;
  thread_info_t *info;

  info = (thread_info_t *)arg;

  // Set local variables
  i = info->row;
  j = info->col;
  sum = 0;

  // Compute element i, j of Matrix C
  for (k = 0; k < ca; k++) {
    sum += A[i][k] * B[k][j];
  }

  // Store the value calculated to Matrix C
  C[i][j] = sum;
  pthread_exit(NULL);
}

int main () {
  pthread_t threads[MAX_NUM_THREADS];
  struct timespec time_1, time_2;
  double exec_time;

  printf("No. of Threads used in computation: %d\n\n", MAX_NUM_THREADS);

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
  }

  //----------------------------------------------------------------------------
  // ---------------- Calculate Matrix C Using Threads... ----------------------
  //----------------------------------------------------------------------------
  exec_time = 0.0;
  clock_gettime(CLOCK_REALTIME, &time_1);

  // Set thread info(s)
  thread_info_t info[MAX_NUM_THREADS];

  // Find list of C indexes
  int max_index = ra * cb;
  int index[max_index][2];
  int num = 0;
  for (i = 0; i < ra; i++) {
    for (j = 0; j < cb; j++) {
      index[num][0] = i;
      index[num][1] = j;
      num++;
    }
  }

  // Start First Set of Threads
  for (i = 0; i < MAX_NUM_THREADS; i++) {
    info[i].row = index[i][0];
    info[i].col = index[i][1];
    pthread_create(&threads[i], NULL, &compute_C_ij, &info[i]);
  }

  // Join old threads, and Create New Threads (as they finish executing)
  j = 0;
  for (i = MAX_NUM_THREADS; i < max_index; i++) {
    // Join the oldest thread
    pthread_join(threads[j], NULL);
    info[j].row = index[i][0];
    info[j].col = index[i][1];

    // Use the joined thread to start next computation
    pthread_create(&threads[j], NULL, &compute_C_ij, &info[j]);

    // Set j to the next Thread to be refreshed
    if (j < (MAX_NUM_THREADS - 1)) {
      j++;
    }
    else {
      j = 0;
    }
  }

  // Join the remaining threads
  for (j = 0; j < MAX_NUM_THREADS; j++) {
    pthread_join(threads[j], NULL);
  }

  clock_gettime(CLOCK_REALTIME, &time_2);

  exec_time = (time_2.tv_sec - time_1.tv_sec);
  exec_time = exec_time + (time_2.tv_nsec - time_1.tv_nsec) / 1.0e9;

  printf("\nExec time: %lfsec\n\n", exec_time);


  //*/
  // Print Matrix C
  printf("Matrix C: \n");
  for (i = 0; i < ra; i++) {
    for (j = 0; j < cb; j++) {
      printf("%d  ", C[i][j]);
    }
    printf("\n");
  }
  return 0;
}
