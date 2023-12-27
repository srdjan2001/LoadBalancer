
#ifndef LIST_H
#define LIST_H

// Node structure
typedef struct Node {
    char data[50];
    struct Node* next;
} Node;

// List structure
typedef struct {
    Node* head;
} List;

extern List* sharedList;

// Function prototypes
List* initializeList();
void appendToList(List* list, char* value);
void printList(const List* list);
char* getLastElement(const List* list);

#endif
#pragma once
