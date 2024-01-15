#define WIN32_LEAN_AND_MEAN      // Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <string.h>
#include <synchapi.h>
#include <WinBase.h>


/*
// Create a new empty list
void createList(List* l) {
    l = (List*)malloc(sizeof(List));
    l->head = NULL;
    return;
}

// Append a new node with the given value to the list


void appendToList(List* list,  char* value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode != NULL) {
        newNode->data = _strdup(value); // Use strdup to duplicate the string
        newNode->next = NULL;

        if (list->head == NULL) {
            list->head = newNode;
        }
        else {
            Node* current = list->head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newNode;
        }
    }
}
*/
// Print the elements of the list
void printList(const List* list) {
    Node* current = list->head;
    while (current != NULL) {
        printf("%s ", current->data);
        current = current->next;
    }
    printf("\n");
}


List* initializeList() {
    List* newList = (List*)malloc(sizeof(List)); // Allocate memory for the List structure
    if (newList != NULL) {
        newList->head = NULL;
        InitializeCriticalSection(&newList->cs);// Initialize the head pointer to NULL for an empty list
    }
    
    return newList;
    
}

// Function to append a new node with the given value to the list
void appendToList(List* list, char* value) {
    Node* newNode = (Node*)malloc(sizeof(Node)); // Allocate memory for the new node
    if (newNode != NULL) {
        
        EnterCriticalSection(&list->cs);
        strcpy_s(newNode->data, sizeof(value),  value);
        newNode->next = NULL;
        
        if (list->head == NULL) {
            list->head = newNode; // If the list is empty, set the new node as the head
        }
        else {
            Node* current = list->head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newNode; // Append the new node to the end of the list
        }
        LeaveCriticalSection(&list->cs);
    }
}

// Function to print the elements of the list



char* getLastElement( List* list) {
    
    if (list->head == NULL) {
        return NULL;
    }
    else {
        EnterCriticalSection(&list->cs);
        Node* current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        LeaveCriticalSection(&list->cs);
        return current->data;
    }
    
}

void removeLastElement(List* l) {
    EnterCriticalSection(&l->cs);
    if (l->head == NULL) {
        LeaveCriticalSection(&l->cs);
        return;
    }
    
    Node* temp = l->head;
    l->head = l->head->next;
    free(temp);
   
   LeaveCriticalSection(&l->cs);
}