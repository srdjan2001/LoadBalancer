// Node structure
#include <winSock2.h>

typedef struct workerNode {
    int workerSocket;
    char messageArray[10][100];
    struct workerNode* next;
} workerNode;

// List structure
typedef struct {
    workerNode* head;
} WorkerList;

extern WorkerList* freeWorkers;
extern WorkerList* busyWorkers;

//function prototypes
WorkerList* initalizeWorkerList();
void appendToWorkerList(WorkerList* list, int workerSocket);
void moveWorkerNode(WorkerList* from, WorkerList* to, int workerSocket);
int getFirstFreeWorker();
//void printList(const WorkerList* list);
//char* getLastElement(const WorkerList* list);