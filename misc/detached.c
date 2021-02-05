/*
Listing 4.5 - Skeleton Program That Creates a Detached Thread
.
.
.
1.  How to create a detached thread
2.  How to set and delete the thread attribute

To compile this code use the shell command:
gcc -pthread -o <output_filename> detached.c
*/

#include <pthread.h>

void* thread_function (void* thread_arg) {
  // Do work here...
}

int main () {
  pthread_attr_t attr;
  pthread_t thread;

  pthread_attr_init (&attr);
  pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);
  pthread_create (&thread, &attr, &thread_function, NULL);
  pthread_attr_destroy (&attr);

  // Do work here...

  // No need to join the second thread.

  return 0;
}
