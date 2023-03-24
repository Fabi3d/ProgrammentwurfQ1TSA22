#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_FUNCTIONS_PER_TASK 10
#define MAX_TASKS 4

typedef struct {
    int frequency;  // in Sekunden
    int num_functions;
    void (*functions[MAX_FUNCTIONS_PER_TASK])();  // Array von Funktionen
} Task;

Task tasks[MAX_TASKS];

void task1_functions();
void task2_functions();
void task3_functions();
void task4_functions();

void add_function1(Task *task, void (*function)()) {
    if (task->num_functions < MAX_FUNCTIONS_PER_TASK) {
        task->functions[task->num_functions++] = function;
    } else {
        printf("Max number of functions per task exceeded.\n");
    }
}

void *task_thread(void *arg) {
    Task *task = (Task *)arg;
    while (1) {
        for (int i = 0; i < task->num_functions; i++) {
            task->functions[i]();  // Funktionen nacheinander aufrufen
        }
        sleep(task->frequency);  // Pause bis zum nächsten Zyklus
    }
}

typedef struct node {
    void (*func)();
    int priority;
    struct node *next;
} Node1;

Node1 *head = NULL;

void addFunc1(void (*func)(), int priority) {
    Node1 *newNode1 = (Node1*)malloc(sizeof(Node1));
    newNode1->func = func;
    newNode1->priority = priority;
    newNode1->next = NULL;
    
    if (head == NULL) {
        head = newNode1;
    } else {
        Node1 *curr = head;
        Node1 *prev = NULL;
        while (curr != NULL && priority < curr->priority) {
            prev = curr;
            curr = curr->next;
        }
        if (prev == NULL) {
            head = newNode1;
        } else {
            prev->next = newNode1;
        }
        newNode1->next = curr;
    }
}

void runFuncs1() {
    Node1 *curr = head;
    while (curr != NULL) {
        curr->func();
        curr = curr->next;
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
    addFunc1(func1, 1);
    addFunc1(func2, 3);
    addFunc1(func3, 2);
    // Task 1
    tasks[0].frequency = 1;
   add_function1(&tasks[0], runFuncs1);

    // Task 2
    tasks[1].frequency = 5;
    add_function1(&tasks[1], task2_functions);

    // Task 3
    tasks[2].frequency = 10;
    add_function1(&tasks[2], task3_functions);

    // Task 4
    tasks[3].frequency = 100;
    add_function1(&tasks[3], task4_functions);

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

void task1_functions() {
    // Füge hier die Funktionen hinzu, die in Task 1 ausgeführt werden sollen
    //aktuelle Zeit ausgeben
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("%d:%d:%d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
    printf("Task 1\n\n");
    runFuncs1();
}

void task2_functions() {
    // Füge hier die Funktionen hinzu, die in Task 2 ausgeführt werden sollen
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("%d:%d:%d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
    printf("Task 2\n\n");
}

void task3_functions() {
    // Füge hier die Funktionen hinzu, die in Task 3 ausgeführt werden sollen
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("%d:%d:%d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
    printf("Task 3\n\n");
}

void task4_functions() {
    // Füge hier die Funktionen hinzu, die in Task 4 ausgeführt werden sollen
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("%d:%d:%d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
    printf("Task 4\n\n");
}
