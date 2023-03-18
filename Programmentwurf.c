#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>

// cycletimes in ms (1s = 1000ms)

#define NUM_TASKS 4
#define NUM_FUNC 5

//array für cycletimes
int cycleTimes[NUM_TASKS] = {1000, 5000, 10000, 100000};

double averageJitter = 0;
int counter = 0;

double a = 5;
double b = 8;

struct timeval start_time, current_time;

double array[NUM_FUNC][NUM_TASKS] = {
    //          T1  T2  T3  T4
    /*add*/    {3,  0,  0,  0},
    /*multi*/  {2,  2,  0,  0},
    /*div*/    {1,  1,  1,  0},
    /*mod*/    {0,  0,  0,  1},
    /*div2*/   {0,  3,  0,  0}
};

void wait_200ms() {
    struct timespec delay = {0, 200000000}; // 200ms in Nanosekunden

    // Pausiere das Programm für 200ms
    nanosleep(&delay, NULL);
}

void registration(int thread, int priority, int task){
        printf("\nThread %d registriert Funktion%d mit Priorität %d \n\n", thread, task, priority);
        array[task-1][thread-1] = priority;
}

double addition(double a, double b, double firstRun)
{
    int func = 1;
    if(firstRun == 1){
        //hier kann eine registrierung der Funktion stattfinden
        //registration(3, 2, func); //Thread 3 registriert Task 1 mit Priorität 2
    }
    else{
    double result;
    result = a + b;
    printf("%lf", result);
    sleep(5);
    return result;
}
}

double multiplication(double a, double b, double firstRun)
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

double division(double a, double b, double firstRun)
{
    int func = 3;
    if(firstRun == 1){
        //hier kann eine registrierung der Funktion stattfinden
        //registration(4, 3, func);
    }
    else{
        if(b == 0){
            printf("Division durch 0 nicht möglich");
            return 0;
        }
    double result;
    result = a / b;
    printf("%lf", result);
    return result;
}
}

double modulo(double a, double b, double firstRun)
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

double division2(double a, double b, double firstRun)
{
    int func = 5;
    if(firstRun == 1){
        //hier kann eine registrierung der Funktion stattfinden
        //registration(2, 3, func);
    }
    else{
        if(a == 0){
            printf("Division durch 0 nicht möglich");
            return 0;
        }
    double result;
    result = b / a;
    printf("%lf", result);
    return result;
}
}

double (*fktPointer[NUM_FUNC])(double, double, double) = {addition, multiplication, division, modulo, division2};

void checkForTasks(int thread, double array[][NUM_TASKS]) {
    for(int j = 0; j < NUM_FUNC; j++){
        double *p = &array[0][thread-1];
        int i;
        for (i = 0; i < NUM_FUNC; i++) {
            if (*p == j+1) {
                fktPointer[i%NUM_FUNC](a, b, 0);
                printf("\n");
            }
            p = p + NUM_TASKS;
        }
    }
    
        
}

void* thread_function(void* arg) {
    int thread_id = *((int*) arg);
    struct timeval start_time, current_time;
    int elapsed_time_ms = 0;
    int last_elapsed_time_ms = 0;
    int jitter_ms = 0;

    while (1)
    {
        gettimeofday(&start_time, NULL);
        printf("\nTask %d running\n\n", thread_id + 1);
        fflush(stdout);
        checkForTasks(thread_id + 1, array);

        do
        {
            gettimeofday(&current_time, NULL);
            elapsed_time_ms = (current_time.tv_sec - start_time.tv_sec) * 1000 + (current_time.tv_usec - start_time.tv_usec) / 1000;
        } while (elapsed_time_ms < cycleTimes[thread_id]);

        printf("\nTask %d finished in %d ms\n", thread_id + 1, elapsed_time_ms);
        fflush(stdout);
        // jitter_ms = elapsed_time_ms - cycleTimes[thread_id];
        // averageJitter += jitter_ms;
        // counter++;
        // printf("\nJitter of Task %d: %d ms  -- average Jitter: %lf\n", thread_id+1, jitter_ms, averageJitter/counter);
        fflush(stdout);
        /*if(counter >= 100){
            printf("\n\nAverage Jitter: %lf\n", averageJitter/counter);
            while(1);
        }*/
        // last_elapsed_time_ms = elapsed_time_ms;
        elapsed_time_ms = 0;
    }
    return NULL;
}

void printArray(double array[][NUM_TASKS])
{
    int i, j;
    printf("Konfiguration:\n\n");
    printf("+----------+-----------+-----------+-----------+-----------+\n");
    printf("| Funktion | Aufgabe 1 | Aufgabe 2 | Aufgabe 3 | Aufgabe 4 |\n");
    printf("+----------+-----------+-----------+-----------+-----------+\n");
    for (i = 0; i < NUM_FUNC; i++)
    {
        printf("| %8d |", i + 1);
        for (j = 0; j < NUM_TASKS; j++)
        {
            printf(" %9d |", (int)array[i][j]);
        }
        printf("\n+----------+-----------+-----------+-----------+-----------+\n");
    }
}

void init(int num_threads) {
    pthread_t threads[num_threads];
    int thread_args[num_threads];
    for (int i = 0; i < num_threads; i++) {
        thread_args[i] = i;
        pthread_create(&threads[i], NULL, thread_function, &thread_args[i]);
        wait_200ms();
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main()
{
    addition(a, b, 1);
    multiplication(a, b, 1);
    division(a, b, 1);
    modulo(a, b, 1);

    printArray(array);
    init(NUM_TASKS);
    return 0;
}
