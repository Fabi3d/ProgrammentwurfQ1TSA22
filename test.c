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

double a;
double b;

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

//array um funktionen zu speichern
//double (*fkt[3])(double, double) = {addition, multiplication, division};

typedef double (*func_ptr)(double, double);

//funktion um dieses array auszugeben

void* primary_task() {
    struct timeval start_time, current_time;
    int elapsed_time_ms = 0;


    while (1) {
        gettimeofday(&start_time, NULL);
        printf("Primary task running\n");
        fflush(stdout);

        // Sleep for 1 second
        //sleep(1);
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
    func_ptr funcs[3] = {addition, multiplication, division};
    double a = 5.0;
    double b = 2.0;

    for (int i = 0; i < 3; i++) {
        double result = funcs[i](a, b);
        printf(" Result of function %d: %lf\n", i, result);
    }
   
    struct timeval start_time, current_time;
    int elapsed_time_ms = 0;

    init();
}

