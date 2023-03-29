// Tim Schwenzer

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

// Maximale Anzahl an Tasks
#define MAX_TASKS 4

// Strukturen --------------------------------------------------------------------------------------------------------------------------------
// Erstelle eine Struktur für die verkettete Liste 
typedef struct node {
    void (*func)();         // Funktionszeiger
    int prioritaet;         // Priorität
    struct node *next;      // Zeiger auf nächstes Element
} Node;

// Erstelle eine Struktur für eine Aufgabe 
typedef struct {
    int frequenz;          // Frequenz in Sekunden
    Node *funktionen;      // verkettete Liste von Funktionen
} Task;

// Array für Tasks -----------------------------------------------------------------------------------------------------------------------------
Task tasks[MAX_TASKS] = {
    {.frequenz = 1, .funktionen = NULL},
    {.frequenz = 5, .funktionen = NULL},
    {.frequenz = 10, .funktionen = NULL},
    {.frequenz = 100, .funktionen = NULL}
};

// Funktion um Aufgaben Tasks hinzuzufügen -----------------------------------------------------------------------------------------------
void add_function(Task *task, void (*function)(), int prioritaet) {
    Node *new_node = (Node *)malloc(sizeof(Node));
     // Erstelle neues Element
    new_node->func = function;             
    new_node->prioritaet = prioritaet;
    new_node->next = task->funktionen;
    task->funktionen = new_node;
}

// Funktion um Aufgaben in die Liste einzufügen ------------------------------------------------------------------------------------------------
void insert_sorted(Node **head_ref, Node *new_node) {
    Node *curr;
    // Falls Liste leer oder Priorität der neuen Aufgabe größer
    if (*head_ref == NULL || (*head_ref)->prioritaet <= new_node->prioritaet) {         
        new_node->next = *head_ref;                                                 // Aufgabe wird am Start eingefügt
        *head_ref = new_node; 
    // Liste wird durchlaufen bis "richtige" Stelle der Aufgabe gefunden              
    } else {                                                                        
        curr = *head_ref;   
        while (curr->next != NULL && curr->next->prioritaet > new_node->prioritaet) {
            curr = curr->next;
        }
        new_node->next = curr->next;                                                // Aufgabe wird dann eingefügt
        curr->next = new_node;
    }
}

// Funktion um Aufgaben nach ihrer Priorität zu sortieren --------------------------------------------------------------------------------------
void sort_functions(Task *task) {
    Node *curr = task->funktionen;
    task->funktionen = NULL;
    while (curr != NULL) {
        Node *next = curr->next;
        insert_sorted(&task->funktionen, curr);
        curr = next;
    }
}
// Funktion, welche es ermöglicht, dasss sich Aufgaben selber in Tasks einfügen können
void registrierung(void (*function)(), int task, int prioritaet) {
    void *func = function;
    //durchlaufe liste und suche nach funktion
    Node *curr = tasks[task].funktionen;
    while (curr != NULL)
    {
        if (curr->func == func)
        {
            curr->prioritaet = prioritaet;
            return;
        }
        curr = curr->next;
    }
    //wenn nicht gefunden, füge sie hinzu
    add_function(&tasks[task], function, prioritaet);
}

// Funktionen werden erstellt die dann von den Tasks ausgeführt werden -------------------------------------------------------------------------
// registrierung (Funktion, Task 0-3, Prioritaet (2 kommt vor 1) )
void funk_A() {
    printf("Hier ist Funktion A\n");  
    //registrierung(funk_A, 0, 0);
}

void funk_B() {
    printf("Hier ist Funktion B\n");
    //registrierung(funk_B, 0, 0);
}

void funk_C() {
    printf("Hier ist Funktion C\n");
    //registrierung(funk_C, 0, 0);
}

void funk_D() {
    printf("Hier ist Funktion D\n");
    //registrierung(funk_D, 0, 0);
}    

// Funktion, welche von den Thread ausgeführt wird----------------------------------------------------------------------------------------------
void *task_thread(void *arg) {

    Task *task = (Task *)arg;
    // Variablen zur Berechnung der Wartezeit
    struct timespec sleep_time, remaining_time;     
    long long elapsed_time_ns;

    // Startzeit des letzten Zyklus wird gespeichert
    struct timespec last_start_time;
    clock_gettime(CLOCK_REALTIME, &last_start_time);

    // Endlosschleife
    while (1) {             
        // Startzeit aktueller Zyklus            
        struct timeval start_time, end_time;
        gettimeofday(&start_time, NULL);

        // Aufgaben werden nach Prioritäten sortiert
        sort_functions(task);
        Node *curr = task->funktionen;
        // Ausgabe der Frequenz
        printf("Task %ds\n", task->frequenz);

        // Funktionen werden im akutellen Task ausgeführt
        while (curr != NULL) {
            // Startzeit Funktion erfassen
            struct timeval func_start_time, func_end_time;
            gettimeofday(&func_start_time, NULL);
            // Ausführen der Funktion
            curr->func();
            // Endzeit erfassen
            gettimeofday(&func_end_time, NULL);
            // Zeit welche die Funktion benötigt hat
            elapsed_time_ns = (func_end_time.tv_sec - func_start_time.tv_sec) * 1000000000LL +
                (func_end_time.tv_usec - func_start_time.tv_usec) * 1000LL;

            // Warte mit neuer Ausführung um die verbleibende Zeit
            sleep_time.tv_sec = (task->frequenz - elapsed_time_ns / 1000000000) - 1;
            sleep_time.tv_nsec = 1000000000 - (elapsed_time_ns % 1000000000);

            // Falls die Zeit "überfällig" ist, wird die Funktion sofort ausgeführt
            if (sleep_time.tv_nsec < 0) {
                sleep_time.tv_sec = 0;
                sleep_time.tv_nsec = 0;
            }
            // nächste Funktion
            curr = curr->next;
        }

        printf("\n");
        // Warte bis zur nächsten Ausführung der Aufgaben
        nanosleep(&sleep_time, &remaining_time);
        // Erfasse Endzeit des akutellen Zyklus
        gettimeofday(&end_time, NULL);
        // Berechne Dauer des aktuellen Zyklus
        long elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000L + (end_time.tv_usec - start_time.tv_usec) / 1000L;
        // Ausgabe der Dauer
        printf("Zeit: %ld ms\n", elapsed_time);

        // Startzeit des aktuellen Zyklus wird gespeichert
        last_start_time.tv_sec += task->frequenz;
    }
}


// Main Funktion -------------------------------------------------------------------------------------------------------------------------------
int main() {
    // Funktionen hinzufügen zu Tasks

    // Task 1 
    add_function(&tasks[0], funk_B, 1);
    add_function(&tasks[0], funk_A, 2);

    // Taks 2
    add_function(&tasks[1], funk_C, 2); 
    add_function(&tasks[1], funk_D, 1);

    // Task 3
    add_function(&tasks[2], funk_A, 1);
    add_function(&tasks[2], funk_B, 2);
    add_function(&tasks[2], funk_C, 3);

    // Task 4
    add_function(&tasks[3], funk_A, 3);
    add_function(&tasks[3], funk_B, 2);
    add_function(&tasks[3], funk_C, 1);


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

// Command f ab(rechts) und alles ersetzen