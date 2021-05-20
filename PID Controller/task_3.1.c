// Compile: simcc cont.c ./cont
/*
Author:     David Clarke
Project:    Final Project
Course:     ELE 709
Date:       March 13, 2021
*/

#include "dlab_def.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <mqueue.h>
#include <errno.h>


pthread_t controlThread;
sem_t data_avail;  

#define MAXS 10000            // Maximum number of samples.

float pi = 3.14159255;        // Set the value for pi
float theta[MAXS];            // Array for motor positions (radians).
float ref[MAXS];              // Array for reference positions (radians).
float refInput[MAXS];         // Holds the true values of the reference array as input to the system
float refPlot[MAXS];
float thetaPlot[MAXS];
float Kp = 60.0;              // Initialize Kp to 0.6 * Ku = 100.
float Ti = 0.025;             // Ti = Pu / 2 = 0.05 / 2 = 0.025
float Td = 0.00625;           // Td = Pu / 8 = 0.05 / 8 = 0.00625
float run_time = 30.0;        // Set the initial run time to 10 seconds.
float Fs = 200.0;             // Set the initial sampling frequency to 200 Hz.
int running = 0;
float N = 20;
int j;                        // This will be the reference index
int i;                        // Index for the theta index

int motorNumber = 4;          // Set motor number (d9 + 1) my d9 = 3

float satblk(float v) {
    if (-3 <= v && v <= 3) {
        return v;
    }
    else if (v > 3) {
        return (3.0);
    }
    else if (v < -3) {
        return (-3.0);
    }
}

void *Control(void *args) {
    float ek, uk = 0, P, I, D, motorPosition;
    float numPoints = Fs * run_time;
    float T = 1 / Fs;
    float Tt = 0.01;
    float prevI = 0;
    float prevD = 0;
    float vk = 0;
    float A, prevA = 0;
    float prevError = 0;
    i = 0;
    running = 1;
    while (running) {
        numPoints = Fs * run_time;
        while (i < MAXS) {
            if (j == MAXS) {
                j = 0;
            }
            sem_wait(&data_avail);
            motorPosition = EtoR(ReadEncoder());
            refInput[i] = ref[j];
            ek = ref[j] - motorPosition;
            A = (T / Tt) * prevA;
            I = prevI + ((Kp * T) / Ti) * prevError + A;
            //I = prevI + ((Kp / Ti) + (uk - vk) * (T / Tt)) * (prevError) * T;
            prevI = I;
            D = (Td / (N * T + Td)) * (prevD) + (Kp * Td * N / (N * T + Td)) * (ek - prevError);
            prevD = D;
            P = Kp * ek;
            vk = P + I + D;
            uk = satblk(vk);
            DtoA(VtoD(uk));
            theta[i] = motorPosition;
            prevError = ek;
            prevA = uk - vk;
            sem_post(&data_avail);
            i++;
            j++;
            sleep(1 / Fs);
        }
        i = 0;
    }
    pthread_exit(NULL);
}

int main(void *args) {
    char userInput, stepType;
    float stepMagnitude, squareMagnitude, squareFreq, squareDutyCycle;
    float numPoints = Fs * run_time;
    int index;
    sem_init(&data_avail, 0, 1); 
    while (1) {
        printf("Control Center Options:\nRun Control Thread: 'r'\nChange Kp: 'p'\nChange Td: 'd'\nChange Ti: 'i'\nChange N: 'n'\nChange Fs: 'f'\nChange Run Time: 't'\nPlot: 'g'\nSave 'h'\nStop Control Thread: 's'\nQuit Program: 'q'\n");
        scanf(" %c", &userInput);
        switch (userInput) {
            case 'r':
                // Run the control alogorithm
                printf("Initializing the control algorithm...\n");                                           
                Initialize(Fs, motorNumber);
                if (pthread_create(&controlThread, NULL, &Control, NULL) != 0) {
                    printf("Error creating the control thread");
                }
                // pthread_join(controlThread, NULL);
                Terminate();
                break;
            case 'p':
                // Change the value of Kp
                sem_wait(&data_avail);
                printf("Enter the knew value for Kp: (current value %f)\n", Kp);
                scanf(" %f", &Kp);
                sem_post(&data_avail);
                break;
            case 'd':
                // Change the value of Td
                sem_wait(&data_avail);
                printf("Enter the knew value for Td: (current value %f)\n", Td);
                scanf(" %f", &Td);
                sem_post(&data_avail);
                break;
            case 'i':
                // Change the value of Ti
                sem_wait(&data_avail);
                printf("Enter the knew value for Ti: (current value %f)\n", Ti);
                scanf(" %f", &Ti);
                sem_post(&data_avail);
                break;
            case 'n':
                // Change the value of N
                sem_wait(&data_avail);
                printf("Enter the knew value for N: (current value %f)\n", N);
                scanf(" %f", &N);
                sem_post(&data_avail);
                break;
            case 'f':
                // Change the sampling frequency, Fs
                sem_wait(&data_avail);
                printf("Enter the knew value for Fs:\n");
                scanf(" %f", &Fs);
                if (stepType == 'q') {
                       Square(ref, MAXS, Fs, (squareMagnitude * pi / 180), squareFreq, squareDutyCycle);
                       j = 0;             
                }
                numPoints = Fs * run_time;
                sem_post(&data_avail);
                break;
            case 't':
                // Change value of total run time, Tf
                sem_wait(&data_avail);
                printf("Enter the knew value for Tf: (current value is %f)\n", run_time);
                scanf(" %f", &run_time);
                numPoints = Fs * run_time;
                sem_post(&data_avail);
                break;
            case 'u':
                // Change the type of inputs (Step or Square)
                // For Step input, prompt for the magnitude of the step
                // For Square input, prompt for the magnitude, frequency and duty cycle
                printf("Enter 't' for Step Input and 'q' for Square Input:\n");
                scanf(" %c", &stepType);
                sem_wait(&data_avail);
                if (stepType == 't') {
                    printf("Enter magnitude:\n");
                    scanf(" %f", &stepMagnitude);
                    for (int i = 0; i < MAXS; i++) {
                        ref[i] = stepMagnitude * pi / 180;                    
                    }
                    //for (int i = 0; i < MAXS; i++) {printf("%f\n", ref[i]);};
                }
                else if (stepType == 'q') {
                    printf("Enter, magnitude, frequency and duty cycle:\n");
                    scanf(" %f %f %f", &squareMagnitude, &squareFreq, &squareDutyCycle);
                    //printf("Mag : %f, Freq: %f, DC: %f", squareMagnitude, squareFreq, squareDutyCycle);
                    Square(ref, MAXS, Fs, (squareMagnitude * pi / 180), squareFreq, squareDutyCycle);
                    // for (int i = 0; i < MAXS; i++) {printf("%f\n", ref[i]);}               
                }
                j = 0;  // reset the reference index variable
                sem_post(&data_avail);
                break;
            case 'g':
                // Plot the motor position
                sem_wait(&data_avail);
                if (index < 0) {
                    index = MAXS - 1 + index;
                }
                printf("The initial index: %d\n", index);
                for (int k = 0; k < numPoints; k++) {
                    if (index == MAXS) index = 0;
                    thetaPlot[k] = theta[index];
                    refPlot[k] = refInput[index];
                    index++;
                }
                plot(refPlot, thetaPlot, Fs, numPoints, SCREEN, "Graph Title", "x-axis", "y-axis");
                sem_post(&data_avail);
                break;
            case 'h':
                // Save a hard copy of the plot in Postscript
                sem_wait(&data_avail);
                if (index < 0) {
                    index = MAXS - 1 + index;
                }
                printf("The initial index: %d\n", index);
                for (int k = 0; k < numPoints; k++) {
                    if (index == MAXS) index = 0;
                    thetaPlot[k] = theta[index];
                    refPlot[k] = refInput[index];
                    index++;
                }
                plot(refPlot, thetaPlot, Fs, numPoints, PS, "Graph Title", "x-axis", "y-axis");
                sem_post(&data_avail);
                break;
            case 's':
                // Terminate the control thread
                printf("Terminating the control function...\n");
                running = 0;
                break;
            case 'q':
                // exit
                printf("Goodbye.\n");
                sem_destroy(&data_avail);
                return 0;
        }
    }
}
