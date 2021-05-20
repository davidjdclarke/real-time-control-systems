#include <stdio.h>
#include "dlab_def.h"
#define MAXS 10000
#define pi 3.14
#include <semaphore.h>

pthread_t Control;
sem_t data_avail;
float theta[MAXS];
float ref[MAXS];
float kp = 1.0;
float run_time = 10.0;
float Fs = 200.0;
const int d9 = 3;
const int motor_number = d9 + 1;
char title[6] = {"GRAPH"};
char xlabel[5] = {"Time"};
char ylabel[2] = {"y"};



void *Control_Func (void *arg)
{
    int k = 0;
    int num_samples = (int)(run_time*Fs);
    float motor_position, ek, uk, oldError;
    
    
    while (k < num_samples)
    {
        sem_wait(&data_avail);
        motor_position = EtoR(ReadEncoder());
        ek = ref[k] - motor_position;
        uk = kp * ek;
        DtoA(VtoD(uk));
        theta[k] = motor_position;
        k++;
        ek = oldError;
        sem_post(&data_avail);
        
    }
    pthread_exit(NULL);
}

int main(void)
{
    
    char user_input;
    char input_type;
    float step_magnitude;
    float square_magnitude;
    float square_frequency;
    float square_duty_cycle;
    int num_points;
    

    

    while(1)
    {
        printf("Enter a letter:\nr to run the program\np to change the value of kp\nf to change fs\nt to change the total run time\nu to decide input type\ng to plot motor posistion\nh to save a hard copy\nand q to exit\n");    
        scanf(" %c", &user_input);
        switch(user_input)
        {
            case'r':
                sem_init(&data_avail, 0, 1);
                if(Initialize(Fs, motor_number) < 0)
                {
                    printf("Error in Initialize()\n");
                    exit(0);
                }
                pthread_create(&Control, NULL, &Control_Func, NULL);
                pthread_join(Control, NULL);
                Terminate();
                sem_destroy(&data_avail);
                break;
            case'p':
                printf("Enter a new value for Kp:\n");
                scanf(" %f", &kp);
                break;
            case'f':
                printf("Enter a new value for Fs:\n");
                scanf(" %f", &Fs);
                break;
            case't':
                printf("Enter a new value for Tf:\n");
                scanf(" %f", &run_time);
                break;
            case'u':
                printf("Enter t for a step input or q for a square:\n");
                scanf(" %c", &input_type);
                if(input_type == 't')
                {
                    printf("Please enter the magnitude:\n");
                    scanf(" %f", &step_magnitude);
                    for(int i = 0; i < MAXS; i++)
                    {
                        ref[i] = step_magnitude*pi/180;    
                    }
                }
                else if(input_type == 'q')
                {
                    printf("Please enter the magnitude, frequency, and duty cycle \n");
                    scanf(" %f %f %f", &square_magnitude, &square_frequency, &square_duty_cycle);
                    Square(ref, MAXS, Fs, square_magnitude*pi/180, square_frequency, square_duty_cycle);
                    //for(int i = 0; i < 200; i++) printf("%f\n", ref[i]);
                    
                }
                
     
                
                break;
            case'g':
                num_points = (int) run_time * Fs;
                plot(ref, theta, Fs, num_points, SCREEN, title, xlabel, ylabel);   
                break;
            case'h':
                num_points = (int) run_time * Fs;
                plot(ref, theta, Fs, num_points, PS, title, xlabel, ylabel);
                break;
            case'q':
                exit(0);
                break;
            default:
                printf("Invalid Selection\n");           
                break;
        }
        
                
    }


}
