#define WIN32_LEAN_AND_MEAN      // Exclude rarely-used stuff from Windows headers

#include <windows.h>

#include <stdio.h>
#include <winsock2.h>
#include "lb_client_com.h"
#include "lb_worker_com.h"
#include <string.h>
#include <synchapi.h>

#include "list.h"
#include "worker_list.h"


HANDLE muteks;
HANDLE workerMuteks;

#pragma comment(lib, "ws2_32.lib")

List* sharedList = initializeList();

WorkerList* freeWorkers = initalizeWorkerList();
WorkerList* busyWorkers = initalizeWorkerList();




int main() {

	
    HANDLE ThreadArray[2];
    DWORD threadId, threadId2;
    muteks = CreateMutex(NULL, FALSE, NULL);
    workerMuteks = CreateMutex(NULL, FALSE, NULL);
    
     // Example argument to be passed to the thread

    // Create a thread and pass the argument
    ThreadArray[0] = CreateThread(NULL, 0, handleWorkers, NULL, 0, &threadId2);
    ThreadArray[1] = CreateThread(NULL, 0, handleClients, NULL, 0, &threadId);
    

    if (ThreadArray[0] == NULL) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }
    
    if (ThreadArray[1] == NULL) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }
    
    
    WaitForMultipleObjects(2, ThreadArray, TRUE, INFINITE);
    
        
    
   
    
    char input[2];
    printf("Press any key to exit");
    gets_s(input, 2);
    
    CloseHandle(ThreadArray[0]);
    CloseHandle(ThreadArray[1]);
    

	return 0;
}