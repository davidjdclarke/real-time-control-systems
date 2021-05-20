#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <math.h>
#include <errno.h>
#include <error.h>
#include <sched.h>
#include <sys/mman.h>

#include <pthread.h>
#include <semaphore.h>

#ifndef PI
#define PI M_PI
#endif

#define SCREEN 0
#define PS     1

#define CALLOC(n,x) ((x *)calloc(n, sizeof(x)))

extern int Initialize(float, int );
extern int ReadEncoder(void);
extern int DtoA(short int);
extern void Terminate(void);
extern float EtoR(int);
extern short int VtoD(float);
extern float DLaBsat(float, float);
extern int gch(void);
extern int getch(void);

extern void read_matrix(FILE *, float **, char *, int *, int *);
extern void read_vector(FILE *, float  *, char *, int *);
extern void print_matrix(FILE *, float **, char *, int, int);
extern void print_vector(FILE *, float  *, char *, int);
extern void multiply_matrix(float **, int, int, float **, int, int, float **c);
extern void add_matrix(float **, float **, int, int, float **);
extern void subtract_matrix(float **, float **, int, int, float **);
extern void scalar_multiply(float, float **, int, int, float **);
extern void copy_matrix(float **, float **, int, int);
extern void transpose_matrix(float **, float **, int, int);

extern double model_tk, model_dt, model_y;
extern double model_x[3];

extern void plot(float *, float *, float, int, int, char *, char *, char *);
extern void sq_wave(float *, int, float, float);
extern void Square(float *, int, float, float, float, float);

extern void setup_sched_parameters(pthread_attr_t *, int);
