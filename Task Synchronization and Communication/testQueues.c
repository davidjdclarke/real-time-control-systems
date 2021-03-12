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

struct Msg
{
    char str[50];
    unsigned int prioroty;
};

int errno;

const char *qname1 = "/inputQueue";
const char *qname2 = "/outputQueue";

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

int computePrime(int N)
{
    int counter = 0;
    int i, p;
    if (N == 1)
        return 2;
    for (i = 3;; i += 2)
    {
        p = isPrime(i);
        if (p == 1)
        {
            counter++;
            if (counter == (N - 1))
                return i;
        }
    }
}

void sendClient(int input)
{
    char msg[50];
    snprintf(msg, 50, "%d", input);

    int retcode;
    mqd_t tx;

    // Add number to queue
    tx = mq_open(qname2, O_WRONLY);

    if (tx < 0)
        perror("receiver mq_open:"); // Error checking

    retcode = mq_send(tx, msg, (size_t)strlen(msg), 1);
    mq_close(tx);
}

void main(void)
{
    mqd_t rx, trx1, trx2;
    struct mq_attr my_attrs;
    ssize_t msg;
    mode_t mode;
    int nMsgs, nthPrime, oflags;
    char inBuff[50];
    unsigned int priority;

    oflags = O_CREAT | O_RDWR; // Set oflags to create queue (O_CREAT)
                               //  and for read & write (O_RDWR)

    mode = S_IRUSR | S_IWUSR; // Set mode to allow other threads
                              //  (created by same user) to use the queue

    trx1 = mq_open(qname1, O_WRONLY);

    if (trx1 < 0) {
        trx1 = mq_open(qname1, oflags, mode, &my_attrs); // Create the MQ

        if (trx1 < 0)
            perror("Main mq_send:"); // Error checking
    }
        
    else {
        mq_close(trx1);
    }
    
    trx2 = mq_open(qname2, O_WRONLY);

    if (trx2 < 0) {
        trx2 = mq_open(qname2, oflags, mode, &my_attrs); // Create the MQ

        if (trx2 < 0)
            perror("Main mq_recieve:"); // Error checking
    }
    else {
        mq_close(trx2);
    }

    while (1)
    {
        rx = mq_open(qname1, O_RDONLY);

        if (rx < 0)
            perror("Receiver mq_open:"); // Error checking

        mq_getattr(rx, &my_attrs);
        nMsgs = my_attrs.mq_curmsgs;
        printf("Number of messages: %d\n", nMsgs);

        if (nMsgs == 0)
        {
            mq_close(rx);
            sleep(MAX_FREQ);
        }
        else if (nMsgs == 1)
        {
            msg = mq_receive(rx, inBuff, sizeof(inBuff), &priority);
            printf("The message: %ld", msg);
            nthPrime = computePrime(msg);
            mq_close(rx);
            sendClient(nthPrime);
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
