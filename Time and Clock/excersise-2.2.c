#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
ELE 709
Experiment 2:  Time and Clock
Auth:  David Clarke
Date:  Jan 27, 2020

Determine the average time required to perform the basic operations of Addition,
Subtraction, Multiplication and Division. This code ignores the timing effects
of the for loop used to perform each of the opertations a set number of times.
*/

int main(int argc, char * argv[]) {
  int i, c, itr;
  struct timespec start_time, stop_time, res;
  double tmp, a, b, addition_time, subtraction_time, multiplication_time, division_time, loop_time;

  // Print clock resolution
  clock_getres(CLOCK_REALTIME, &res);
  printf("The clock resolution of CLOCK %d is %ld sec and %ld nsec\n", CLOCK_REALTIME, res.tv_sec, res.tv_nsec);

  // Check command line arguments
  itr = 500000000;
  if(argc > 1) {
    a = atof(argv[1]);
    b = atof(argv[2]);
  }
  else {
    printf("No input arguments... Defuault values '2.3' & '4.5' used.\n");
    a = 2.3;
    b = 4.5;
  }

  clock_gettime(CLOCK_REALTIME, &start_time);
  for(i = 0; i < itr; i++) {
    // Do nothing
  }
  clock_gettime(CLOCK_REALTIME, &stop_time);
  tmp = (stop_time.tv_sec - start_time.tv_sec) * 1e9;
  tmp += (stop_time.tv_nsec - start_time.tv_nsec);
  loop_time = tmp / itr;

  clock_gettime(CLOCK_REALTIME, &start_time);
  for(i = 0; i < itr; i++) {
    // Addition
    c = a + b;
  }
  clock_gettime(CLOCK_REALTIME, &stop_time);
  tmp = (stop_time.tv_sec - start_time.tv_sec) * 1e9;
  tmp += (stop_time.tv_nsec - start_time.tv_nsec);
  addition_time = tmp / itr;

  clock_gettime(CLOCK_REALTIME, &start_time);
  for(i = 0; i < itr; i++) {
    // Subtraction
    c = a - b;
  }
  clock_gettime(CLOCK_REALTIME, &stop_time);
  tmp = (stop_time.tv_sec - start_time.tv_sec) * 1e9;
  tmp += (stop_time.tv_nsec - start_time.tv_nsec);
  subtraction_time = tmp / itr;

  clock_gettime(CLOCK_REALTIME, &start_time);
  for(i = 0; i < itr; i++) {
    // Multiplication
    c = a * b;
  }
  clock_gettime(CLOCK_REALTIME, &stop_time);
  tmp = (stop_time.tv_sec - start_time.tv_sec) * 1e9;
  tmp += (stop_time.tv_nsec - start_time.tv_nsec);
  multiplication_time = tmp / itr;

  clock_gettime(CLOCK_REALTIME, &start_time);
  for(i = 0; i < itr; i++) {
    // Division
    c = a / b;
  }
  clock_gettime(CLOCK_REALTIME, &stop_time);
  tmp = (stop_time.tv_sec - start_time.tv_sec) * 1e9;
  tmp += (stop_time.tv_nsec - start_time.tv_nsec);
  division_time = tmp / itr;

  //addition_time = addition_time / itr;
  //subtraction_time = subtraction_time / itr;
  //multiplication_time = multiplication_time / itr;
  //division_time = division_time / itr;

  // printf("Loop Time:            %lf nsec\n", loop_time);
  printf("Addition Time (Avg):        %lf nsec\n", addition_time);
  printf("Subtraction Time (Avg):     %lf nsec\n", subtraction_time);
  printf("Multiplication Time (Avg):  %lf nsec\n", multiplication_time);
  printf("Division Time (Avg):        %lf nsec\n", division_time);

  return 0;
}
