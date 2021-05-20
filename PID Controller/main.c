// Compile: simcc main.c ./main
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
float Kp = 100.0;               // Initialize Kp to 1.
float run_time = 10.0;        // Set the initial run time to 10 seconds.
float Fs = 200.0;             // Set the initial sampling frequency to 200 Hz.

int motorNumber = 4;          // Set motor number (d9 + 1) my d9 = 3

void *Control(void *args) {
    float ek, uk, motorPosition;
    float numPoints = Fs * run_time;
    for (int i = 0; i < numPoints; i++) {
        sem_wait(&data_avail);
        motorPosition = EtoR(ReadEncoder());
        ek = ref[i] - motorPosition;
        uk = Kp * ek;
        DtoA(VtoD(uk));
        theta[i] = motorPosition;
        sem_post(&data_avail);
    }
    pthread_exit(NULL);
}

int main(void *args) {
    char userInput, stepType;
    float stepMagnitude, squareMagnitude, squareFreq, squareDutyCycle;
    float numPoints = Fs * run_time;
    while (1) {
        printf("Control Center: 'r' - run, 'p' - Kp, 'f' - Fs, 't' - Tf, 'u' - Step/Square, 'g' - plot, 'h' - save, 'q' - exit\n");  // Display options
        scanf(" %c", &userInput);
        switch (userInput) {
            case 'r':
                // Run the control alogorithm
                printf("Initializing the control algorithm...\n");
                sem_init(&data_avail, 0, 1);                                            
                Initialize(Fs, motorNumber);
                if (pthread_create(&controlThread, NULL, &Control, NULL) != 0) {
                    printf("Error creating the control thread");
                }
                pthread_join(controlThread, NULL);
                Terminate();
                sem_destroy(&data_avail);
                break;
            case 'p':
                // Change the value of Kp
                printf("Enter the knew value for Kp: (current value %f)\n", Kp);
                scanf(" %f", &Kp);
                break;
            case 'f':
                // Change the sampling frequency, Fs
                printf("Enter the knew value for Fs:\n");
                scanf(" %f", &Fs);
                break;
            case 't':
                // Change value of total run time, Tf
                printf("Enter the knew value for Tf:\n");
                scanf(" %f", &run_time);
                break;
            case 'u':
                // Change the type of inputs (Step or Square)
                // For Step input, prompt for the magnitude of the step
                // For Square input, prompt for the magnitude, frequency and duty cycle
                printf("Enter 't' for Step Input and 'q' for Square Input:\n");
                scanf(" %c", &stepType);
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
                break;
            case 'g':
                // Plot the motor position
                plot(ref, theta, Fs, numPoints, SCREEN, "Graph Title", "x-axis", "y-axis");
                break;
            case 'h':
                // Save a hard copy of the plot in Postscript
                plot(ref, theta, Fs, numPoints, PS, "Graph Title", "x-axis", "y-axis");
                break;
            case 'q':
                // exit
                return 0;
        }
    }
}
