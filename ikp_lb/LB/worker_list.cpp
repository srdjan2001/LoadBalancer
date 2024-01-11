#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "worker_list.h"
#include <string.h>
#include "list.h"

WorkerList* initalizeWorkerList() {
    WorkerList* newList = (WorkerList*)malloc(sizeof(WorkerList)); // Allocate memory for the List structure
    if (newList != NULL) {
        newList->head = NULL; // Initialize the head pointer to NULL for an empty list
        InitializeCriticalSection(&newList->cs);
    }
    return newList;

}

void appendToWorkerList(WorkerList* workerList, int workerSocket) {

    workerNode* newNode = (workerNode*)malloc(sizeof(workerNode));
    if (newNode != NULL) {
        EnterCriticalSection(&workerList->cs);
        newNode->workerSocket = workerSocket;
        
        newNode->next = NULL;
        for (int i = 0; i < 10; i++) {
            strcpy_s(newNode->messageArray[i], 1, "\0");
        }
        if (workerList->head == NULL) {
            workerList->head = newNode;
        }
        else {
            newNode->next = workerList->head;
            workerList->head = newNode;
        }
        LeaveCriticalSection(&workerList->cs);
    }
}

void moveWorkerNode(WorkerList* from, WorkerList* to, int workerSocket) {
    

    workerNode* node_from = from->head;
    
    if (node_from != NULL) {
        EnterCriticalSection(&from->cs);
        EnterCriticalSection(&to->cs);
        struct workerNode* temp = from->head, * prev = NULL;

        // If head node itself holds the key to be deleted 
        if (temp != NULL && temp->workerSocket == workerSocket) {
            from->head = temp->next; // Changed head 
            appendToWorkerListEnd(to, temp);
            
            printf("Naso sam ga");
            
            LeaveCriticalSection(&to->cs);
            LeaveCriticalSection(&from->cs);
            return;
        }

        // Search for the key to be deleted, keep track of the 
        // previous node as we need to change 'prev->next' 
        while (temp != NULL && temp->workerSocket != workerSocket) {
            prev = temp;
            temp = temp->next;
        }

        // If key was not present in linked list 
        if (temp == NULL) {
            printf("Nisam naso sam ga");
            LeaveCriticalSection(&to->cs);
            LeaveCriticalSection(&from->cs);
            return;
        }
        
        // Unlink the node from linked list 
        prev->next = temp->next;
        appendToWorkerListEnd(to, temp);
        
        
        
        

        
        LeaveCriticalSection(&to->cs);
        LeaveCriticalSection(&from->cs);
        
    }
}

int getFirstFreeWorker() {
    

    return freeWorkers->head->workerSocket;
    
}

void appendToWorkerListEnd(WorkerList* list, workerNode* worker) {
    EnterCriticalSection(&list->cs);
    workerNode* temp = list->head;
    if (temp != NULL) {
        while (temp->next != NULL) {
            temp = temp->next;
            printf("%d", temp->workerSocket);
        }
        worker->next = NULL;
        temp->next = worker;
        
    }
    else {
        worker->next = NULL;
        list->head = worker;
    }
    LeaveCriticalSection(&list->cs);
}