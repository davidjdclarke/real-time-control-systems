// Compile: gcc -pthread -o client client.c -lrt

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <mqueue.h>
#include <errno.h>

int errno;
int MAX_FREQ = 1;

const char *qname1 = "/clientRequestQueue";
const char *qname2 = "/serverReturnQueue";

void *getServerOutput (void *arg) {
    mqd_t rx;
    struct mq_attr my_attrs;
    ssize_t n;
    int msg;
    int nMsgs;
    char inBuff[50];
    unsigned int priority;

    while (1) {
        rx = mq_open(qname2, O_RDONLY);

        if (rx < 0)
            perror("Receiver mq_open:"); // Error checking

        mq_getattr(rx, &my_attrs);
        nMsgs = my_attrs.mq_curmsgs;

        if (nMsgs == 0) {
            mq_close(rx);
            sleep(MAX_FREQ);
        }
        else if (nMsgs == 1) {
            n = mq_receive(rx, inBuff, sizeof(inBuff), &priority);
            inBuff[n] = '\0';
            msg = atoi(inBuff);
            char inBuff[50];
            printf("Received computed value: %d\n", msg);
            mq_close(rx);
            printf("------------------------------------------------\n\n");
        }
        else {
            printf("WARNING! YOU HAVE STUMBLED INTO A TRAP, CODE BROKEN...");
            sleep(2);
            printf("goodbye...");
            break;
        }
    }
}

void *getUserInput (void *arg) {
    int retcode;
    char input[10], check[10] = "0";
    mqd_t tx;

    while (1)
    {
        printf("Enter a number to compute... ('0' will terminate).\n");
        scanf("%s", input);
        
        if (strcmp(check, input) != 0)
        {
            // Add number to queue
            tx = mq_open(qname1, O_WRONLY);

            if (tx < 0)
                perror("Sender mq_open:"); // Error checking

            retcode = mq_send(tx, input, (size_t)strlen(input), 1);
            //printf("Number: %s.\n\n", input);
            mq_close(tx);
        }
        else
        {
            tx = mq_open(qname1, O_WRONLY);

            if (tx < 0)
                perror("Sender mq_open:"); // Error checking

            retcode = mq_send(tx, input, (size_t)strlen(input), 1);
            //printf("Number: %s.\n\n", input);
            mq_close(tx);

            printf("Goodbye!\n");
            pthread_exit(NULL);
        }

        sleep(2);
    }
}

int main (void) {
    mqd_t trx1, trx2;
    mode_t mode1, mode2;
    int oflags1, oflags2;
    struct mq_attr my_attrs1, my_attrs2;
    pthread_t sendingThread, receivingThread;

    my_attrs1.mq_maxmsg = 1;   // Set max no of msg in queue to 1 msgs
    my_attrs1.mq_msgsize = 50; // Set the max msg size to 50 bytes

    oflags1 = O_CREAT | O_RDWR; // Set oflags to create queue (O_CREAT)
                               //  and for read & write (O_RDWR)

    mode1 = S_IRUSR | S_IWUSR; // Set mode to allow other threads
                              //  (created by same user) to use the queue

    my_attrs2.mq_maxmsg = 1;   // Set max no of msg in queue to 1 msgs
    my_attrs2.mq_msgsize = 50;   // Set the max msg size to 50 bytes

    oflags2 = O_CREAT | O_RDWR;  // Set oflags to create queue (O_CREAT)
                                 //  and for read & write (O_RDWR)

    mode2 = S_IRUSR | S_IWUSR; // Set mode to allow other threads
                               //  (created by same user) to use the queue

    trx1 = mq_open(qname1, oflags1, mode1, &my_attrs1); // Create the MQ
    
    if (trx1 < 0)
        perror("Main mq_open:"); // Error checking
    
    trx2 = mq_open(qname2, oflags2, mode2, &my_attrs2);

    if (trx2 < 0)
        printf("fuck my ass\n");
        perror("Main mq_receive:"); // Error checking

    // Start the "Read Input" Thread...
    if (pthread_create(&sendingThread, NULL, &getUserInput, NULL) != 0) {
        printf("Error creating output thread.\n");
        exit(-1);
    }

    if (pthread_create(&receivingThread, NULL, &getServerOutput, NULL) != 0)
    {
        printf("Error creating recieving thread.\n");
        exit(-1);
    }

    pthread_join(sendingThread, NULL);
    pthread_cancel(receivingThread);
}