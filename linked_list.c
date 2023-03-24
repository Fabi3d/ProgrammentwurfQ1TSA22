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

void task_functions_1();
void task_functions_2();
void task_functions_3();
void task_functions_4();

void add_function(Task *task, void (*function)()) {
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
} Node;

Node *head = NULL;

void add_func(void (*func)(), int priority) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->func = func;
    newNode->priority = priority;
    newNode->next = NULL;
    
    if (head == NULL) {
        head = newNode;
    } else {
        Node *curr = head;
        Node *prev = NULL;
        while (curr != NULL && priority < curr->priority) {
            prev = curr;
            curr = curr->next;
        }
        if (prev == NULL) {
            head = newNode;
        } else {
            prev->next = newNode;
        }
        newNode->next = curr;
    }
}

void run_funcs() {
    Node *curr = head;
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

void task1_functions() {
    // Füge hier die Funktionen hinzu, die in Task 1 ausgeführt werden sollen
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("%d:%d:%d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
    printf("Task 1\n\n");
    runFuncs1();
}

int main() {
    // Funktionen hinzufügen
    add_func(func1, 1);
    add_func(func2, 3);
    add_func(func3, 2);
    add_func(func4, 4);

    // Task 1
    tasks[0].frequency = 1;
    add_function(&tasks[0], run_funcs);

    // Task 2
    tasks[1].frequency = 5;
    for (Node *curr = head; curr != NULL; curr = curr->next) {
        add_function(&tasks[1], curr->func);
    }

    // Task 3
    tasks[2].frequency = 10;
    add_function(&tasks[2], task_functions_3);

    // Task 4
    tasks[3].frequency = 100;
    add_function(&tasks[3], task_functions_4);

    // Threads für jede Aufgabe erstellen
    pthread_t threads[MAX_TASKS];
    for (int i = 0; i < MAX_TASKS; i++) {
        pthread_create(&threads[i], NULL, task_thread, (void *)&tasks[i]);
    }
    return 0;
}