// Compile: gcc -pthread -o out lab5.c -lrt

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <mqueue.h>
#include <errno.h>

struct Msg {char str[20]; unsigned int priority; };
int errno;

const char *qname = "/myque";  // Define the name of the MQ, use your
                               // last name as part of the name to create
                               // a unique name.

void *Receiver(void *arg)  // This thread receives msgs from the MQ
{
   mqd_t rx;
   struct mq_attr my_attrs;
   ssize_t n;
   int nMsgs;
   char inBuf[50];
   unsigned int priority;

   sleep(5);

   rx = mq_open(qname, O_RDONLY);  // Open the MQ for reading only (O_RDONLY)

   if (rx < 0 ) perror("Receiver mq_open:");  // Error checking

   mq_getattr(rx, &my_attrs);     // Determine no. of msgs currently in the MQ
   nMsgs = my_attrs.mq_curmsgs;

   printf("Receiver found %d messages\n", nMsgs);

   // Receive all messages in the MQ by calling mq_receive

   while (nMsgs-- > 0) {
      n = mq_receive(rx, inBuf, sizeof(inBuf), &priority);
      inBuf[n] = '\0';
      if (n < 0) perror("Receiver mq_receive:");  // Error checking
      printf("Receiver got <%s>, %ld bytes at priority %d\n", inBuf, n, priority);
   }

   mq_close(rx);   // Close the MQ
   pthread_exit(NULL);
}
   
void *Sender(void *arg)  // This thread sends msgs to the MQ
{
   int retcode, i;
   mqd_t tx;
   struct Msg myMsg;
   unsigned int msg_priority[5] = {3, 1, 5, 8, 4};  // Set up message priorities

   sleep(1);

   tx = mq_open(qname, O_WRONLY);  // Open the MQ for writing only (O_WRONLY)

   if (tx < 0) perror("Sender mq_open:");  // Error checking

   // Send 5 messages to the MQ by calling mq_send

   for (i = 1; i <= 5; i++) {
      sprintf(myMsg.str, "Test msg %d", i);  // Set up message to be sent
      myMsg.priority = msg_priority[i-1];    // Set up priority of this message
      retcode = mq_send(tx, myMsg.str, (size_t)strlen(myMsg.str), myMsg.priority);
      if (retcode < 0) perror("Sender mq_send:");  // Error checking
   }

   mq_close(tx);   // Close the MQ
   pthread_exit(NULL);
}

int main(void *arg)
{
   mqd_t trx;
   mode_t mode;
   int oflags;
   struct mq_attr my_attrs;
   pthread_t Sender_thread, Receiver_thread;

   my_attrs.mq_maxmsg = 10;      // Set max no of msg in queue to 10 msgs
   my_attrs.mq_msgsize = 50;     // Set the max msg size to 50 bytes

   oflags = O_CREAT | O_RDWR ;   // Set oflags to create queue (O_CREAT)
                                 //  and for read & write (O_RDWR)
   
   mode = S_IRUSR | S_IWUSR;     // Set mode to allow other threads
                                 //  (created by same user) to use the queue

   trx = mq_open(qname, oflags, mode, &my_attrs);  // Create the MQ

   if (trx < 0) perror("Main mq_open:");  // Error checking

   // Create a thread to send messages to the MQ

   printf("Creating Sender thread\n");
   if (pthread_create(&Sender_thread, NULL, &Sender, NULL) != 0) {
       printf("Error creating Sender thread.\n");
       exit(-1);
   }

   // 

   // Cretae a thread to receive the messages from the MQ
   printf("Creating Receiver thread\n");
   if (pthread_create(&Receiver_thread, NULL, &Receiver, NULL) != 0) {
       printf("Error creating Receiver thread.\n");
       exit(-1);
   }

   pthread_join(Sender_thread, NULL);
   pthread_join(Receiver_thread, NULL);

   mq_unlink(qname);     // Destroy the MQ
   exit(0);
}
