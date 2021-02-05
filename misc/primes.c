/*
Listing 4.4 - Compute Prime Numbers in Thread
.
.
.
1.  How to return values from threads.

The Shell Command to compile:
gcc -pthread -o file2 primes.c
*/


#include <pthread.h>
#include <stdio.h>

/*
Compute the successive prime numbers (very inefficiently). Return the Nth
prime number, where N is the value pointed to by *ARG
*/

void* compute_prime (void* arg) {
  int candidate = 2;
  int n = *((int*) arg);

  while (1) {
    int factor;
    int is_prime = 1;

    // Test primality by successive division
    for (factor = 2; factor < candidate; ++factor) {
      if (candidate % factor == 0) {
        if (--n == 0) {
          // Return the desired prime number as the thread return value
          pthread_exit((void*) candidate);
        }
      }
      ++candidate;
    }
  }
  pthread_exit(NULL);
}

int main () {
  pthread_t thread;
  int which_prime = 5000;
  int prime;

  // Start the computing thread, up to the 5,000th prime number
  pthread_create (&thread, NULL, &compute_prime, &which_prime);
  // Do some work here...
  // Wait for the thread to finish:
  pthread_join (thread, (void*) &prime);

  // Print the largest prime number:
  printf("The %dth prime number is %d.\n", which_prime, prime);
  return 0;
}
