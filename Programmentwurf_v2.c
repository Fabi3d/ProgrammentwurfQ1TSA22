#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>

#define MAX_TASKS 4

//Structure for the lists
typedef struct node
{
    void (*func)();
    int priority;
    struct node *next;
} Node;

typedef struct
{
    int frequency;   // frequency in seconds
    Node *functions; // list of functions
} Task;

//Array of tasks
Task tasks[MAX_TASKS] = {
    {.frequency = 1, .functions = NULL},
    {.frequency = 5, .functions = NULL},
    {.frequency = 10, .functions = NULL},
    {.frequency = 100, .functions = NULL}};

void swap(Node *a, Node *b)
{
    int temp = a->priority;
    a->priority = b->priority;
    b->priority = temp;
}


void bubble_sort(Node **head_ref) {
    int swapped;
    Node *ptr1;
    Node *lptr = NULL;
    
    /* Checking for empty list */
    if (*head_ref == NULL)
        return;
 
    do
    {
        swapped = 0;
        ptr1 = *head_ref;
 
        while (ptr1->next != lptr)
        {
            if (ptr1->priority > ptr1->next->priority)
            {
                swap(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    }
    while (swapped);
}

/* Funktion zum Swappen von zwei Nodes */

void sort_functions(Task *task)
{
    bubble_sort(&task->functions);
}
//Function prototypes
void add_function(Task *task, void (*function)(), int priority)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->func = function;
    new_node->priority = priority;
    new_node->next = task->functions;
    task->functions = new_node;
    sort_functions(task);
}


pthread_mutex_t mutex;

void *task_thread(void *arg)
{
    Task *task = (Task *)arg;
    struct timespec sleep_time, remaining_time;
    long long elapsed_time_ns;

    // initialisiere last_start_time mit der aktuellen Zeit
    struct timespec last_start_time;
    clock_gettime(CLOCK_REALTIME, &last_start_time);

    // Mutex initialisieren
    if (pthread_mutex_init(&mutex, NULL) != 0)
    {
        printf("Mutex-Initialisierung fehlgeschlagen\n");
        exit(1);
    }
    
    while (1)
    {
        
        Node *curr = task->functions;
        printf("\033[32mTask %ds\033[0m\n", task->frequency);

        while (curr != NULL)
        {
            struct timespec func_start_time, func_end_time;
            clock_gettime(CLOCK_REALTIME, &func_start_time);

            // Mutex sperren, bevor der kritische Abschnitt ausgeführt wird
            pthread_mutex_lock(&mutex);
            curr->func();
            // Mutex entsperren, nachdem der kritische Abschnitt ausgeführt wurde
            pthread_mutex_unlock(&mutex);

            clock_gettime(CLOCK_REALTIME, &func_end_time);

            long long elapsed_time_ms = (func_end_time.tv_sec - func_start_time.tv_sec) * 1000LL +
                                        (func_end_time.tv_nsec - func_start_time.tv_nsec) / 1000000LL;
            sleep_time.tv_sec = (task->frequency - elapsed_time_ms / 1000L) - 1;
            curr = curr->next;
        }
        printf("\n");

        // speichere die Startzeit des aktuellen Zyklus
        struct timespec current_time;
        clock_gettime(CLOCK_REALTIME, &current_time);
        long long elapsed_time_ms = (current_time.tv_sec - last_start_time.tv_sec) * 1000LL +
                                    (current_time.tv_nsec - last_start_time.tv_nsec) / 1000000LL;
        last_start_time.tv_sec += task->frequency;

        sleep_time.tv_nsec = (1000L - (elapsed_time_ms % 1000L)) * 1000000L;
        long long remaining_time_ms = remaining_time.tv_sec * 1000LL + remaining_time.tv_nsec / 1000000LL;
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &last_start_time, &remaining_time);
    }
}

void selfRegister(void (*function)(), int priority, int task)
{
    void *func = function;
    //durchlaufe liste und suche nach funktion
    Node *curr = tasks[task].functions;
    while (curr != NULL)
    {
        if (curr->func == func)
        {
            curr->priority = priority;
            return;
        }
        curr = curr->next;
    }
    //wenn nicht gefunden, füge sie hinzu
    add_function(&tasks[task], function, priority);
}

void func1()
{
    printf("Function 1\n");
}

void func2()
{
    printf("Function 2\n");
}

void func3()
{
    printf("Function 3\n");
}

void func4()
{
    printf("Function 4\n");
    selfRegister(func4, 3, 0);
}

void cleanup()
{
    // Mutex zerstören
    pthread_mutex_destroy(&mutex);

    // Speicher für die verketteten Listen freigeben
    for (int i = 0; i < MAX_TASKS; i++)
    {
        Node *curr = tasks[i].functions;
        while (curr != NULL)
        {
            Node *next = curr->next;
            free(curr);
            curr = next;
        }
    }
}

void exit_handler(int sig)
{
    printf("Exiting...\n");
    cleanup();
    exit(0);
}

int main()
{
    // Funktionen hinzufügen

    signal(SIGINT, exit_handler);

    add_function(&tasks[0], func1, 2);
    add_function(&tasks[0], func2, 2);

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
    for (int i = 0; i < MAX_TASKS; i++)
    {
        pthread_create(&threads[i], NULL, task_thread, (void *)&tasks[i]);
    }

    // Warten, bis der Benutzer den Prozess beendet
    while (1)
    {
        sleep(1);
    }

    return 0;
}
