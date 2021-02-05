#include <stdio.h>
#include <time.h>

int main() {
  int i, itr;
  struct timespec start_time, stop_time, sleep_time, res;
  double tmp, elapsed_time[10];

  clock_getres(CLOCK_REALTIME, &res);
  printf("The clock resolution of CLOCK %d is %ld sec and %ld nsec\n", CLOCK_REALTIME, res.tv_sec, res.tv_nsec);

  sleep_time.tv_sec = 0;
  sleep_time.tv_nsec = 20000000;
  printf("Sleep time requested is %ld sec %ld nsec\n", sleep_time.tv_sec, sleep_time.tv_nsec);

  itr = 10;
  for(i = 0; i < itr; i++) {
    clock_gettime(CLOCK_REALTIME, &start_time);
    nanosleep(&sleep_time, NULL);
    clock_gettime(CLOCK_REALTIME, &stop_time);

    tmp = (stop_time.tv_sec - start_time.tv_sec) * 1e9;
    tmp += (stop_time.tv_nsec - start_time.tv_nsec);
    elapsed_time[i] = tmp;
  }

  for(i = 0; i < itr; i++) {
    printf("Iteration %2d ... Slept for %lf nsec\n", i, elapsed_time[i]);
  }

  return 0;
}
