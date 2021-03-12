#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <mqueue.h>
#include <math.h>
#include <errno.h>

int computePrime(int arg)
{
    int candidate = 2;
    int n = arg;
    if (arg == 0)
        return 0;
    while (1)
    {
        int factor;
        int is_prime = 1;
        // Test primality by successive division.
        for (factor = 2; factor < candidate; ++factor)
            if (candidate % factor == 0)
            {
                is_prime = 0;
                break;
            }
        // Is this the prime number we’re looking for?
        if (is_prime)
        {
            if (--n == 0)
                // Return the desired prime number as the thread return value.
                return candidate;
        }
        ++candidate;
    }
    return -1;
}

void main(void *arg) {
    int n = 10;
    int done;
    
    done = computePrime(n);
    printf("%d\n", done);

}