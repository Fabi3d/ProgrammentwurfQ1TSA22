#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

//cycletimes in ms (1s = 1000ms)
#define CYCLE1 1000
#define CYCLE2 5000
#define CYCLE3 10000
#define CYCLE4 100000

#define NUM_THREADS 4
#define NUM_TASKS 3

double a = 5;
double b = 8;


//zweidimensionales double array mit vier spalten und drei zeilen erstellen und befüllen mit 0
double array[NUM_TASKS][NUM_THREADS] = {
//          T1  T2  T3  T4
/*add*/    {1,  0,  0,  0},
/*multi*/  {0,  1,  0,  0},
/*div*/    {0,  0,  1,  1}
};

//pointer auf den ersten eintral des arrays
double *p1 = &array[0][0];
double *p2 = &array[0][1];
double *p3 = &array[0][2];
double *p4 = &array[0][3];

double addition(double a, double b){
    double result;
    result = a+b;
    printf("%lf", result);
    return result;
}

double multiplication(double a, double b){
    double result;
    result = a*b;
    printf("%lf", result);
    return result;
}

double division(double a, double b){
    double result;
    result = a/b;
    printf("%lf", result);
    return result;
}


void* primary_task() {
    struct timeval start_time, current_time;
    int elapsed_time_ms = 0;


    while (1) {
        gettimeofday(&start_time, NULL);
        printf("Primary task running\n");
        fflush(stdout);

        printf("\n");
        printf("%lf\n", *p1);
        if (*p1 == 1){
            addition(a, b);
            printf("\n");
        }
        p1 = p1 + NUM_THREADS;

        if(*p1 == 1){
            multiplication(a, b);
            printf("\n");
        }
        p1 = p1 + NUM_THREADS;
        
        if(*p1 == 1){
            division(a, b);
            printf("\n");
        }
      
        printf("\n");
        p1 = &array[0][0];

        while(((current_time.tv_sec - start_time.tv_sec) * 1000 + (current_time.tv_usec - start_time.tv_usec) / 1000) <= CYCLE1){

            gettimeofday(&current_time, NULL);
        }
     
        gettimeofday(&current_time, NULL);
        elapsed_time_ms = (current_time.tv_sec - start_time.tv_sec) * 1000 + (current_time.tv_usec - start_time.tv_usec) / 1000;

        printf("Primary task finished in %d ms\n", elapsed_time_ms);
        fflush(stdout);
    }
}

void* secondary_task() {
    struct timeval start_time, current_time;
    int elapsed_time_ms = 0;

    printf("\n");
    p2 = &array[0][1];
        if (*p2 == 1){
            addition(a, b);
            printf("\n");
        }
        p2 = p2 + NUM_THREADS;

        if(*p2 == 1){
            multiplication(a, b);
            printf("\n");
        }
        p2 = p2 + NUM_THREADS;
        
        if(*p2 == 1){
            division(a, b);
            printf("\n");
        }
      
        printf("\n");
        p2 = &array[0][1];


    while (1) {
        gettimeofday(&start_time, NULL);
        printf("Secondary task running\n");
        fflush(stdout);

        // Sleep for 5 seconds
        //sleep(5);
        gettimeofday(&current_time, NULL);
        while(((current_time.tv_sec - start_time.tv_sec) * 1000 + (current_time.tv_usec - start_time.tv_usec) / 1000) <= CYCLE2){
            gettimeofday(&current_time, NULL);
        }

        gettimeofday(&current_time, NULL);
        elapsed_time_ms = (current_time.tv_sec - start_time.tv_sec) * 1000 + (current_time.tv_usec - start_time.tv_usec) / 1000;

        printf("Secondary task finished in %d ms\n", elapsed_time_ms);
        fflush(stdout);
    }
}

void* third_task() {
    struct timeval start_time, current_time;
    int elapsed_time_ms = 0;

        printf("\n");
        p3 = &array[0][2];
        if (*p3 == 1){
            addition(a, b);
            printf("\n");
        }
        p3 = p3 + NUM_THREADS;

        if(*p3 == 1){
            multiplication(a, b);
            printf("\n");
        }
        p3 = p3 + NUM_THREADS;
        
        if(*p3 == 1){
            division(a, b);
            printf("\n");
        }
      
        printf("\n");
        p3 = &array[0][2];


    while (1) {
        gettimeofday(&start_time, NULL);
        printf("Third task running\n");
        fflush(stdout);

        // Sleep for 10 seconds
        //sleep(10);
        gettimeofday(&current_time, NULL);
        while(((current_time.tv_sec - start_time.tv_sec) * 1000 + (current_time.tv_usec - start_time.tv_usec) / 1000) <= CYCLE3){
            gettimeofday(&current_time, NULL);
        }

        gettimeofday(&current_time, NULL);
        elapsed_time_ms = (current_time.tv_sec - start_time.tv_sec) * 1000 + (current_time.tv_usec - start_time.tv_usec) / 1000;

        printf("Third task finished in %d ms\n", elapsed_time_ms);
        fflush(stdout);
    }
}

void* fourth_task() {
    struct timeval start_time, current_time;
    int elapsed_time_ms = 0;

     
    p4 = &array[0][3];
    printf("\n");
        if (*p4 == 1){
            addition(a, b);
            printf("\n");
        }
        p4 = p4 + NUM_THREADS;

        if(*p4 == 1){
            multiplication(a, b);
            printf("\n");
        }
        p4 = p4 + NUM_THREADS;
        
        if(*p4 == 1){
            division(a, b);
            printf("\n");
        }
      
        printf("\n");
        p4 = &array[0][3];

    while (1) {
        gettimeofday(&start_time, NULL);
        printf("Fourth task running\n");
        fflush(stdout);

        // Sleep for 100 seconds
        //sleep(100);

        gettimeofday(&current_time, NULL);
        while(((current_time.tv_sec - start_time.tv_sec) * 1000 + (current_time.tv_usec - start_time.tv_usec) / 1000) <= CYCLE4){
            gettimeofday(&current_time, NULL);
        }

        gettimeofday(&current_time, NULL);
        elapsed_time_ms = (current_time.tv_sec - start_time.tv_sec) * 1000 + (current_time.tv_usec - start_time.tv_usec) / 1000;

        printf("Fourth task finished in %d ms\n", elapsed_time_ms);
        fflush(stdout);
    }
}


void init(){
    //init
    pthread_t thread_id1, thread_id2, thread_id3, thread_id4;
        // Start primary task
    pthread_create(&thread_id1, NULL, primary_task, NULL);

    // Start secondary task
    pthread_create(&thread_id2, NULL, secondary_task, NULL);

    // Start third task
    pthread_create(&thread_id3, NULL, third_task, NULL);

    // Start fourth task
    pthread_create(&thread_id4, NULL, fourth_task, NULL);

    pthread_join(thread_id1, NULL);
    pthread_join(thread_id2, NULL);
    pthread_join(thread_id3, NULL);
    pthread_join(thread_id4, NULL);

}

int main() {

    struct timeval start_time, current_time;
    int elapsed_time_ms = 0;

    init();
}

