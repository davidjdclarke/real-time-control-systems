// Compile: gcc -pthread -o server server.c -lrt

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

int MAX_FREQ = 1;

struct Msg {
    char str[50];
    unsigned int prioroty;
};

int errno;

const char *qname1 = "/clientRequestQueue";
const char *qname2 = "/serverReturnQueue";

int isPrime(int n)
{
    int i;
    for (i = 2; i <= n / 2; i++)
    {
        if (n % i != 0)
            continue;
        else
            return 1;
    }
    return 0;
}

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

void sendClient(int input) {
    char msg[50];
    snprintf(msg, 50, "%d", input);

    int retcode;
    mqd_t tx;

    // Add number to queue
    tx = mq_open(qname2, O_WRONLY);

    if (tx < 0)
        perror("receiver mq_open:"); // Error checking

    retcode = mq_send(tx, msg, (size_t)strlen(msg), 1);
}

int main (void) {
    mqd_t rx;
    struct mq_attr my_attrs;
    ssize_t n;
    mode_t mode;
    int nMsgs, msg, nthPrime, oflags;
    char inBuff[50];
    unsigned int priority;

    while (1) {
        rx = mq_open(qname1, O_RDONLY);

        if (rx < 0)
            perror("Receiver mq_open:"); // Error checking
    
        mq_getattr(rx, &my_attrs);
        nMsgs = my_attrs.mq_curmsgs;
        //printf("Number of messages: %d\n", nMsgs);

        if (nMsgs == 0)
        {
            mq_close(rx);
            sleep(MAX_FREQ);
        }
        else if (nMsgs == 1)
        {
            printf("Message Recieved!\n");
            n = mq_receive(rx, inBuff, sizeof(inBuff), &priority);
            inBuff[n] = '\0';
            msg = atoi(inBuff);
            char inBuff[50];
            printf("- %d\n\n", msg);
            if (msg == 0) {
                printf("Server Received Termination Message...\n");
                sleep(1);
                return 0;
            }
            nthPrime = computePrime(msg);
            printf("Sending Value: %d\n", nthPrime);
            sendClient(nthPrime);
            mq_close(rx);
        }
        else if (nMsgs > 1)
        {
            printf("Here 1\n\n");
            sleep(2);
            printf("goodbye...\n");
            break;
        }
    }
}


