#include <stdio.h>
#include <stdlib.h>
#include "worker_list.h"
#include <string.h>
#include "list.h"

WorkerList* initalizeWorkerList() {
    WorkerList* newList = (WorkerList*)malloc(sizeof(WorkerList)); // Allocate memory for the List structure
    if (newList != NULL) {
        newList->head = NULL; // Initialize the head pointer to NULL for an empty list
    }
    return newList;

}

void appendToWorkerList(WorkerList* workerList, int workerSocket) {

    workerNode* newNode = (workerNode*)malloc(sizeof(workerNode));
    if (newNode != NULL) {
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
    }
}

void moveWorkerNode(WorkerList* from, WorkerList* to, int workerSocket) {
    

    workerNode* node_from = from->head;
    
    if (node_from != NULL) {
        WaitForSingleObject(muteks, INFINITE);
        while (node_from != NULL) {
            if (node_from->workerSocket == workerSocket) {
                break;
            }
            from->head = from->head->next;
        }
        

        if (to->head == NULL) {
            
            to->head = node_from;
        }
        else {
            node_from->next = to->head;
            to->head = node_from;
        }
    }
    ReleaseMutex(muteks);
}

int getFirstFreeWorker() {
    return freeWorkers->head->workerSocket;
}