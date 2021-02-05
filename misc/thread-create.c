/*
Listing 4.1 - Create a Thread
.
.
.
1.  How to create and start a thread

To compile this code use the shell command:
gcc -pthread -o <output_filename> thread-create.c
*/

#include <pthread.h>
#include <stdio.h>

// Prints x's to stderr.  The parameter is unused.  Doe not return.

void* print_xs(void* unused) {
  while(1) {
    fputc ('x', stderr);
  }
  return NULL;
}

// The Main
int main () {
  pthread_t thread_id;
  // Create new thread.  The new thread will run the print_xs function.
  pthread_create (&thread_id, NULL, &print_xs, NULL);
  // Print o's continously to stderr.
  while (1) {
    fputc ('o', stderr);
  }
  return 0;
}
