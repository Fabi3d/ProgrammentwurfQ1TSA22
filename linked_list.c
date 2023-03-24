#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struktur für eine Funktion mit Priorität
typedef struct Function {
    int priority;
    char name[20];
    void (*function)();
    struct Function* next;
} Function;

// Beispiel-Funktionen, die hinzugefügt werden können
void function1() {
    printf("Function 1 wurde aufgerufen.\n");
}

void function2() {
    printf("Function 2 wurde aufgerufen.\n");
}

void function3() {
    printf("Function 3 wurde aufgerufen.\n");
}

// Funktion zum Einfügen einer Funktion in eine Liste nach Priorität
void insertFunction(Function** head, int priority, void (*function)()) {
    Function* newFunction = (Function*)malloc(sizeof(Function));
    newFunction->priority = priority;
    newFunction->next = NULL;

    switch (priority) {
        case 1:
            strcpy(newFunction->name, "Function 1");
            break;
        case 2:
            strcpy(newFunction->name, "Function 2");
            break;
        case 3:
            strcpy(newFunction->name, "Function 3");
            break;
        default:
            strcpy(newFunction->name, "Unknown Function");
            break;
    }

    newFunction->function = function;

    if (*head == NULL) {
        *head = newFunction;
    } else if ((*head)->priority > priority) {
        newFunction->next = *head;
        *head = newFunction;
    } else {
        Function* current = *head;
        while (current->next != NULL && current->next->priority <= priority) {
            current = current->next;
        }
        newFunction->next = current->next;
        current->next = newFunction;
    }
}

// Funktion zum Sortieren der Liste nach Priorität
void sortFunctions(Function** head) {
    if (*head == NULL || (*head)->next == NULL) {
        return;
    }

    Function* current = *head;
    Function* temp;
    int swapped;

    do {
        swapped = 0;
        current = *head;

        while (current->next != NULL) {
            if (current->priority > current->next->priority) {
                temp = current;
                current = current->next;
                temp->next = current->next;
                current->next = temp;
                if (temp == *head) {
                    *head = current;
                }
                swapped = 1;
            } else {
                current = current->next;
            }
        }
    } while (swapped);
}

// Funktion zum Ausführen der Funktionen in der Liste
void runFunctions(Function* head) {
    Function* current = head;
    while (current != NULL) {
        printf("Funktion %d (%s):\n", current->priority, current->name);
        current->function();
        current = current->next;
    }
}

int main() {
    Function* head = NULL;

    insertFunction(&head, 3, function3);
    insertFunction(&head, 2, function1);
    insertFunction(&head, 1, function2);

    printf("Unsortierte Liste:\n\n");
    runFunctions(head);

    sortFunctions(&head);

    printf("Sortierte Liste:\n\n");
    runFunctions(head);

    return 0;
}
