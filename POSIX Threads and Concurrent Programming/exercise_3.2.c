#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

struct thread_info {
   double a, b;       // The numbers a and b
   char op;           // The mathematical operation to be performed: +,-,*,/
   int maxitr;        // The number of iterations to be performed
   double c;          // The result: c = a <op> b
   double exec_time;  // The execution time per iteration in nsec
};

typedef struct thread_info thread_info_t;

void *func(void *arg)
{
   struct timespec time_1, time_2;
   double exec_time;
   thread_info_t *info;
   int i, maxitr;
   char op;
   double a, b, c;

   info = (thread_info_t *)arg;
   maxitr = info->maxitr;

   b = info->b; a = info->a;

   op = info->op;

   exec_time = 0.0;

   if (op == '+') {
     // Addition
     printf("Starting excution of: %c operation...\n", op);
     clock_gettime(CLOCK_REALTIME, &time_1);
     for (i = 0; i < maxitr ; i++) {
           c = a + b;
     }
     clock_gettime(CLOCK_REALTIME, &time_2);
   }
   else if (op == '-') {
     // Subtraction
     printf("Starting excution of: %c operation...\n", op);
     clock_gettime(CLOCK_REALTIME, &time_1);
     for (i = 0; i < maxitr ; i++) {
           c = a - b;
     }
     clock_gettime(CLOCK_REALTIME, &time_2);
   }
   else if (op == '*') {
     // Multiplication
     printf("Starting excution of: %c operation...\n", op);
     clock_gettime(CLOCK_REALTIME, &time_1);
     for (i = 0; i < maxitr ; i++) {
           c = a * b;
     }
     clock_gettime(CLOCK_REALTIME, &time_2);
   }
   else if (op == '/') {
     printf("Starting excution of: %c operation...\n", op);
     clock_gettime(CLOCK_REALTIME, &time_1);
     for (i = 0; i < maxitr ; i++) {
           c = a / b;
     }
     clock_gettime(CLOCK_REALTIME, &time_2);
   }

   exec_time = (time_2.tv_sec - time_1.tv_sec) * 1.0e9;
   exec_time = exec_time + (time_2.tv_nsec - time_1.tv_nsec);

   info->exec_time = (exec_time / maxitr);
   info->c = c;

   pthread_exit(NULL);

}

int main(void)
{
   pthread_t thread_add, thread_sub, thread_mul, thread_div;
   thread_info_t info_add, info_sub, info_mul, info_div;
   double maxitr;

   maxitr = 5.0e8;

   // Addition Thread Info:
   info_add.maxitr = (int)maxitr;
   info_add.a = (double)2.3;
   info_add.b = (double)4.5;
   info_add.op = (char)'+';

   // Subtraction Thread Info:
   info_sub.maxitr = (int)maxitr;
   info_sub.a = (double)2.3;
   info_sub.b = (double)4.5;
   info_sub.op = (char)'-';

   // Multiplication Thread Info:
   info_mul.maxitr = (int)maxitr;
   info_mul.a = (double)2.3;
   info_mul.b = (double)4.5;
   info_mul.op = (char)'*';

   // Division Thread Info:
   info_div.maxitr = (int)maxitr;
   info_div.a = (double)2.3;
   info_div.b = (double)4.5;
   info_div.op = (char)'/';

   if (pthread_create(&thread_add, NULL, &func, &info_add) != 0) {
           printf("Error in creating Addition thread\n");
           exit(1);
   }

   pthread_join(thread_add, NULL);
   printf("Exec time for operation: %lf %c %lf = %lf, is %lfnsec\n", info_add.a, info_add.op, info_add.b, info_add.c, info_add.exec_time);

   if (pthread_create(&thread_sub, NULL, &func, &info_sub) != 0) {
           printf("Error in creating Subtraction thread\n");
           exit(1);
   }

   pthread_join(thread_sub, NULL);
   printf("Exec time for operation: %lf %c %lf = %lf, is %lfnsec\n", info_sub.a, info_sub.op, info_sub.b, info_sub.c, info_sub.exec_time);

   if (pthread_create(&thread_mul, NULL, &func, &info_mul) != 0) {
           printf("Error in creating Subtraction thread\n");
           exit(1);
   }

   pthread_join(thread_mul, NULL);
   printf("Exec time for operation: %lf %c %lf = %lf, is %lfnsec\n", info_mul.a, info_mul.op, info_mul.b, info_mul.c, info_mul.exec_time);

   if (pthread_create(&thread_div, NULL, &func, &info_div) != 0) {
           printf("Error in creating Subtraction thread\n");
           exit(1);
   }

   pthread_join(thread_div, NULL);
   printf("Exec time for operation: %lf %c %lf = %lf, is %lfnsec\n", info_div.a, info_div.op, info_div.b, info_div.c, info_div.exec_time);

   pthread_exit(NULL);
}
