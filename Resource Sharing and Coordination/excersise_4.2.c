#include <stdio.h>       /* for fprintf() */
#include <stdlib.h>      /* for exit() */
#include <unistd.h>      /* for sleep() */
#include <pthread.h>     /* for pthreads functions */

#define NTHREADS 3

int string_index = 0;

pthread_mutex_t lock;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

char string_to_print[] = "0123456789";

void *funcA(void *arg)
{
   int tmp_index;

   sleep(1);   /* sleep 1 sec to allow all threads to start */

   while (1) {
     pthread_mutex_lock(&lock); // lock the mutex
     while (string_index % 2 == 1) {
       pthread_cond_wait(&cond, &lock);
     }

      tmp_index = string_index;

      if (!(tmp_index+1 >= sizeof(string_to_print))) {
         printf("A%c ", string_to_print[tmp_index]);
         usleep(1);   /* sleep 1 usec */
      }

      string_index = tmp_index + 1;

      if (tmp_index+1 > sizeof(string_to_print)) {
         printf("\n");
         string_index = 0;   /* wrap around */
      }
      pthread_mutex_unlock(&lock); // release the mutex lock
      pthread_cond_signal(&cond);
   }
}

void *funcB(void *arg)
{
   int tmp_index;

   sleep(1);   /* sleep 1 sec to allow all threads to start */

   while (1) {
     pthread_mutex_lock(&lock); // lock the mutex
     while (string_index % 2 == 0) {
       pthread_cond_wait(&cond, &lock);
     }

      tmp_index = string_index;

      if (!(tmp_index+1 > sizeof(string_to_print))) {
         printf("B%c ", string_to_print[tmp_index]);
         usleep(1);   /* sleep 1 usec */
      }

      string_index = tmp_index + 1;

      if (tmp_index+1 > sizeof(string_to_print)) {
         printf("\n");
         string_index = 0;   /* wrap around */
      }
      pthread_mutex_unlock(&lock); // release the mutex lock
      pthread_cond_signal(&cond);
   }
}


int main(void)
{
   pthread_t threadA, threadB;
   int k;

   if (pthread_mutex_init(&lock, NULL) != 0) {
     printf("\n mutex init has failed\n");
     return 1;
   }

   if (pthread_create(&threadA, NULL, &funcA, NULL) != 0) {
         printf("Error creating thread A\n");
         exit(-1);
   }

   if (pthread_create(&threadB, NULL, &funcB, NULL) != 0) {
         printf("Error creating thread B\n");
         exit(-1);
   }

   sleep(20);   /* sleep 20 secs to allow time for the threads to run */
                /* before terminating them with pthread_cancel()      */

  pthread_cancel(threadA);
  pthread_cancel(threadB);

   for (k = string_index; k < 10; k++) {
     printf("%c", string_to_print[k]);
   }

   // printf("Test: %c", string_to_print[10]);

   printf("\n");
   // pthread_exit(NULL);
   return 0;
}
