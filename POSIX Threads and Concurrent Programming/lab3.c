#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

struct thread_info {
   int maxitr;
   double exec_time;
};

typedef struct thread_info thread_info_t;

void *func(void *arg)
{
   struct timespec time_1, time_2;
   double exec_time;
   thread_info_t *info;
   int i, maxitr;
   volatile double a, b, c;

   info = (thread_info_t *)arg;
   maxitr = info->maxitr;

   b = 2.3; c = 4.5;

   exec_time = 0.0;

   clock_gettime(CLOCK_REALTIME, &time_1);

   for (i = 0; i < maxitr ; i++) {
         a = b*b + b*c + c*c;
   }
         
   clock_gettime(CLOCK_REALTIME, &time_2);

   exec_time = (time_2.tv_sec - time_1.tv_sec);
   exec_time = exec_time + (time_2.tv_nsec - time_1.tv_nsec)/1.0e9;

   info->exec_time = exec_time;

   pthread_exit(NULL);
   
}

int main(void)
{
   pthread_t thread1;
   thread_info_t info1;
   double maxitr;

   maxitr = 5.0e8;

   info1.maxitr = (int)maxitr;

   if (pthread_create(&thread1, NULL, &func, &info1) != 0) {
           printf("Error in creating thread 1\n");
           exit(1);
   }

   pthread_join(thread1, NULL);
   printf("Exec time for thread1 = %lf sec\n", info1.exec_time);

   pthread_exit(NULL);
}
