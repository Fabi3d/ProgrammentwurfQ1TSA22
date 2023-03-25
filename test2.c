#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

#define MAX_TASKS 4

typedef struct node {
    void (*func)();
    int priority;
    struct node *next;
} Node;

typedef struct {
    int frequency; // in Sekunden
    Node *functions; // verkettete Liste von Funktionen
} Task;

Task tasks[MAX_TASKS] = {
    {.frequency = 1, .functions = NULL},
    {.frequency = 5, .functions = NULL},
    {.frequency = 10, .functions = NULL},
    {.frequency = 100, .functions = NULL}
};

void add_function(Task *task, void (*function)(), int priority) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->func = function;
    new_node->priority = priority;
    new_node->next = task->functions;
    task->functions = new_node;
}

void insert_sorted(Node **head_ref, Node *new_node) {
    Node *curr;
    if (*head_ref == NULL || (*head_ref)->priority <= new_node->priority) {
        new_node->next = *head_ref;
        *head_ref = new_node;
    } else {
        curr = *head_ref;
        while (curr->next != NULL && curr->next->priority > new_node->priority) {
            curr = curr->next;
        }
        new_node->next = curr->next;
        curr->next = new_node;
    }
}

void sort_functions(Task *task) {
    Node *curr = task->functions;
    task->functions = NULL;
    while (curr != NULL) {
        Node *next = curr->next;
        insert_sorted(&task->functions, curr);
        curr = next;
    }
}

void *task_thread(void *arg) {
    Task *task = (Task *)arg;
    struct timespec sleep_time, remaining_time;
    long long elapsed_time_ns;

    // initialisiere last_start_time mit der aktuellen Zeit
    struct timespec last_start_time;
    clock_gettime(CLOCK_REALTIME, &last_start_time);

    while (1) {
        struct timeval start_time, end_time;
        gettimeofday(&start_time, NULL);

        sort_functions(task);
        Node *curr = task->functions;
        printf("Task %ds\n", task->frequency);

        while (curr != NULL) {
            struct timeval func_start_time, func_end_time;
            gettimeofday(&func_start_time, NULL);
            curr->func();
            gettimeofday(&func_end_time, NULL);

            elapsed_time_ns = (func_end_time.tv_sec - func_start_time.tv_sec) * 1000000000LL +
                (func_end_time.tv_usec - func_start_time.tv_usec) * 1000LL;

            // verzögere die Ausführung der nächsten Funktion um die verbleibende Zeit
            sleep_time.tv_sec = (task->frequency - elapsed_time_ns / 1000000000) - 1;
            sleep_time.tv_nsec = 1000000000 - (elapsed_time_ns % 1000000000);

            // Wenn die verbleibende Zeit negativ ist, wird die Funktion sofort ausgeführt
            if (sleep_time.tv_nsec < 0) {
                sleep_time.tv_sec = 0;
                sleep_time.tv_nsec = 0;
            }

            

            curr = curr->next;
        }

        printf("\n");
        nanosleep(&sleep_time, &remaining_time);
        gettimeofday(&end_time, NULL);
        long elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000L + (end_time.tv_usec - start_time.tv_usec) / 1000L;
        printf("Elapsed time: %ld ms\n", elapsed_time);

        // speichere die Startzeit des aktuellen Zyklus
        last_start_time.tv_sec += task->frequency;
       // clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &last_start_time, NULL);

    }
}



void func1() {
    printf("Function 1\n");
}

void func2() {
    printf("Function 2\n");
}

void func3() {
    printf("Function 3\n");
}

void func4() {
    printf("Function 4\n");
}

int main() {
    // Funktionen hinzufügen
    add_function(&tasks[0], func1, 2);
    add_function(&tasks[0], func2, 1);

    add_function(&tasks[1], func3, 2);
    add_function(&tasks[1], func4, 1);

    add_function(&tasks[2], func1, 1);
    add_function(&tasks[2], func2, 2);
    add_function(&tasks[2], func3, 3);
    add_function(&tasks[2], func4, 4);

    add_function(&tasks[3], func1, 3);
    add_function(&tasks[3], func2, 2);
    add_function(&tasks[3], func3, 1);


    // Threads für jede Aufgabe erstellen
    pthread_t threads[MAX_TASKS];
    for (int i = 0; i < MAX_TASKS; i++) {
        pthread_create(&threads[i], NULL, task_thread, (void *)&tasks[i]);
    }

    // Warten, bis der Benutzer den Prozess beendet
    while (1) {
        sleep(1);
    }

    return 0;
}
