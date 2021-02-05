/*
Listing 4.2 - Create Two Threads
.
.
.
1.  This demonstrates how JOINABLE threads operate.
2.  How to join two threads to the creating thread.

The Shell Command to compile:
gcc -pthread -o file2 thread-create2.c
*/

#include <pthread.h>
#include <stdio.h>

// Params to the print function
struct char_print_params {
  char charector;   // Charector to print
  int count;        // Number of times print
};

// Prints a number of charectors to stderr, as given by PARAMETERS, which is a pointer to a struct char_print_params
void* char_print (void* parameters) {
  struct char_print_params* p = (struct char_print_params*) parameters;
  int i;

  for (i = 0; i < p->count; i++) {
    fputc (p->charector, stderr);
  }
  return NULL;
}

int main () {
  pthread_t thread1_id;
  pthread_t thread2_id;
  struct char_print_params thread1_args;
  struct char_print_params thread2_args;

  // Create a new thread to print 30,000 x's
  thread1_args.charector = 'x';
  thread1_args.count = 30000;
  pthread_create (&thread1_id, NULL, &char_print, &thread1_args);

  // Create a new thread to print 20,000 o's
  thread2_args.charector = 'o';
  thread2_args.count = 20000;
  pthread_create (&thread2_id, NULL, &char_print, &thread2_args);

  // Make sure that the first thread has finsihed
  pthread_join (thread1_id, NULL);
  // Make sure that the second thread has finished
  pthread_join (thread2_id, NULL);

  return 0;
}
