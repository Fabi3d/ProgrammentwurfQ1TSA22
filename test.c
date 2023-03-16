#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

// cycletimes in ms (1s = 1000ms)
#define CYCLE1 1000
#define CYCLE2 5000
#define CYCLE3 10000
#define CYCLE4 100000

#define NUM_THREADS 4
#define NUM_TASKS 4

double a = 5;
double b = 8;

struct timeval start_time, current_time;

double array[NUM_TASKS][NUM_THREADS] = {
    //          T1  T2  T3  T4
    /*add*/    {3,  0,  0,  0},
    /*multi*/  {2,  2,  0,  0},
    /*div*/    {2,  1,  1,  0},
    /*mod*/    {0,  0,  0,  1}
};

void wait_200ms()
{
    //damit die Threads nicht gleichzeitig starten wird hier eine Funktion erstellt, die die Threads erst startet, wenn 200ms vergangen sind
    gettimeofday(&start_time, NULL);
    int elapsed_time = 0;

    while (elapsed_time < 200000)
    {
        gettimeofday(&current_time, NULL);
        elapsed_time = (current_time.tv_sec - start_time.tv_sec) * 1000000 + current_time.tv_usec - start_time.tv_usec;
    }
}

void registration(int thread, int priority, int task){
        printf("\nThread %d registriert Funktion%d mit Priorität %d \n\n", thread, task, priority);
        array[task-1][thread-1] = priority;
}

double addition(double a, double b, int firstRun)
{
    int func = 1;
    if(firstRun == 1){
        //hier kann eine registrierung der Funktion stattfinden
        registration(3, 2, func); //Thread 3 registriert Task 1 mit Priorität 2
    }
    else{
    double result;
    result = a + b;
    printf("%lf", result);
    return result;
}
}

double multiplication(double a, double b, int firstRun)
{
    int func = 2;
    if(firstRun == 1){
        //hier kann eine registrierung der Funktion stattfinden
    }
    else{
    double result;
    result = a * b;
    printf("%lf", result);
    return result;
}
}

double division(double a, double b, int firstRun)
{
    int func = 3;
    if(firstRun == 1){
        //hier kann eine registrierung der Funktion stattfinden
        //registration(4, 3, func);
    }
    else{
    double result;
    result = a / b;
    printf("%lf", result);
    return result;
}
}

double modulo(double a, double b, int firstRun)
{
    int func = 4;
    if(firstRun == 1){
        //hier kann eine registrierung der Funktion stattfinden
        //registration(1, 1, func);
    }
    else{
    double result;
    result = (int)a % (int)b;
    printf("%lf", result);
    return result;
}
}

void checkForTasks(int thread, double array[][NUM_THREADS]) {
    for(int j = 0; j < NUM_TASKS; j++){
        double *p = &array[0][thread-1];
        int i;
        for (i = 0; i < NUM_TASKS; i++) {
            if (*p == j+1) {
                switch(i%NUM_TASKS){
                    case 0:
                        addition(a, b, 0);
                        printf("\n");
                        break;
                    case 1:
                        multiplication(a, b, 0);
                        printf("\n");
                        break;
                    case 2:
                        division(a, b, 0);
                        printf("\n");
                        break;
                    case 3:
                        modulo(a, b, 0);
                        printf("\n");
                        break;
                };  
            }
            p = p + NUM_THREADS;
        }
    }
    
        
}

void *primary_task()
{
    //struct timeval start_time, current_time;
    int elapsed_time_ms = 0;

    while (1)
    {
        gettimeofday(&start_time, NULL);
        printf("Primary task running\n");
        fflush(stdout);
        checkForTasks(1, array);
        while (((current_time.tv_sec - start_time.tv_sec) * 1000 + (current_time.tv_usec - start_time.tv_usec) / 1000) <= CYCLE1)
        {

            gettimeofday(&current_time, NULL);
        }

        gettimeofday(&current_time, NULL);
        elapsed_time_ms = (current_time.tv_sec - start_time.tv_sec) * 1000 + (current_time.tv_usec - start_time.tv_usec) / 1000;

        printf("Primary task finished in %d ms\n", elapsed_time_ms);
        fflush(stdout);
    }
}

void *secondary_task()
{

    int elapsed_time_ms = 0;

    while (1)
    {
        gettimeofday(&start_time, NULL);
        printf("Secondary task running\n");
        fflush(stdout);
        checkForTasks(2, array);
        fflush(stdout);

        while (((current_time.tv_sec - start_time.tv_sec) * 1000 + (current_time.tv_usec - start_time.tv_usec) / 1000) <= CYCLE2)
        {
            gettimeofday(&current_time, NULL);
        }

        gettimeofday(&current_time, NULL);
        elapsed_time_ms = (current_time.tv_sec - start_time.tv_sec) * 1000 + (current_time.tv_usec - start_time.tv_usec) / 1000;

        printf("Secondary task finished in %d ms\n", elapsed_time_ms);
        fflush(stdout);
    }
}

void *third_task()
{
    int elapsed_time_ms = 0;

    while (1)
    {
        gettimeofday(&start_time, NULL);
        printf("Third task running\n");
        fflush(stdout);
        checkForTasks(3, array);
        while (((current_time.tv_sec - start_time.tv_sec) * 1000 + (current_time.tv_usec - start_time.tv_usec) / 1000) <= CYCLE3)
        {
            gettimeofday(&current_time, NULL);
        }

        gettimeofday(&current_time, NULL);
        elapsed_time_ms = (current_time.tv_sec - start_time.tv_sec) * 1000 + (current_time.tv_usec - start_time.tv_usec) / 1000;

        printf("Third task finished in %d ms\n", elapsed_time_ms);
        fflush(stdout);
    }
}

void *fourth_task()
{
    
    int elapsed_time_ms = 0;

    while (1)
    {
        gettimeofday(&start_time, NULL);
        printf("Fourth task running\n");
        fflush(stdout);
        checkForTasks(4, array);
        while (((current_time.tv_sec - start_time.tv_sec) * 1000 + (current_time.tv_usec - start_time.tv_usec) / 1000) <= CYCLE4)
        {
            gettimeofday(&current_time, NULL);
        }

        gettimeofday(&current_time, NULL);
        elapsed_time_ms = (current_time.tv_sec - start_time.tv_sec) * 1000 + (current_time.tv_usec - start_time.tv_usec) / 1000;

        printf("Fourth task finished in %d ms\n", elapsed_time_ms);
        fflush(stdout);
    }
}

void printArray(double array[][NUM_THREADS]) {
    int i, j;
    printf("Konfiguration:\n\n");
    printf("Task       1| 2| 3| 4| \n");
    printf("-----------------------\n");
    for (i = 0; i < NUM_TASKS; i++) {
        printf("Func %d:    ", i+1);
            for (j = 0; j < NUM_THREADS; j++) {
                printf("%d  ", (int) array[i][j]);
            }
        printf("\n-----------------------\n");
    }
    printf("\n");
}

void init()
{
 
    pthread_t thread_id1, thread_id2, thread_id3, thread_id4;
    // Start primary task
    pthread_create(&thread_id1, NULL, primary_task, NULL);
    wait_200ms();
    // Start secondary task
    pthread_create(&thread_id2, NULL, secondary_task, NULL);
    wait_200ms();
    // Start third task
    pthread_create(&thread_id3, NULL, third_task, NULL);
    wait_200ms();
    // Start fourth task
    pthread_create(&thread_id4, NULL, fourth_task, NULL);


    pthread_join(thread_id1, NULL);
    pthread_join(thread_id2, NULL);
    pthread_join(thread_id3, NULL);
    pthread_join(thread_id4, NULL);
}

int main()
{
    addition(a, b, 1);
    multiplication(a, b, 1);
    division(a, b, 1);
    modulo(a, b, 1);

    
    printArray(array);
    init();
}
