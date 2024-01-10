#include <stdio.h>
#include <winsock2.h>
#include "lb_client_com.h"
#include "lb_worker_com.h"
#include <string.h>
#include <windows.h>
#include "list.h"
#include "worker_list.h"

#define PORT 5059
#define MAX_WORKERS 10
#define BUFFER_SIZE 256

#pragma comment(lib, "ws2_32.lib")

List* sharedList = initializeList();

WorkerList* freeWorkers = initalizeWorkerList();
WorkerList* busyWorkers = initalizeWorkerList();

HANDLE muteks;


int main() {

	
    HANDLE myThread, myThread2;
    DWORD threadId, threadId2;
    muteks = CreateSemaphore(0,0,4,0);
     // Example argument to be passed to the thread

    // Create a thread and pass the argument
    myThread = CreateThread(NULL, 0, handleClients, NULL, 0, &threadId);
    myThread2 = CreateThread(NULL, 0,handleWorkers , NULL, 0, &threadId2);

    if (myThread == NULL) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    
    //WaitForSingleObject(myThread2, INFINITE);
    char input[2];
    printf("Press any key to exit");
    gets_s(input, 2);
    
    CloseHandle(myThread);


	return 0;
}