#include <stdio.h>
#include <winsock2.h>
#include "worker_list.h"
#include "list.h"

#define PORT 10000
#define MAX_WORKERS 10
#define BUFFER_SIZE 256

#pragma comment(lib, "ws2_32.lib")



DWORD WINAPI handleWorkers(LPVOID lpParam) {
    WSADATA wsaData;
    SOCKET workerSockets[MAX_WORKERS];
    for (int i = 0; i < MAX_WORKERS; i++) {
        workerSockets[i] = NULL;
    }
    char buffer[BUFFER_SIZE];
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed to initialize Winsock.\n");
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        printf("Error creating socket: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Bind failed: %ld\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen failed: %ld\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);

    fd_set readSet;
    while (1) {
        FD_ZERO(&readSet);
        FD_SET(serverSocket, &readSet);

        int maxSocket = serverSocket;

        // Add client sockets to the set
        for (int i = 0; i < MAX_WORKERS; ++i) {
            int workerSocket = workerSockets[i];
            if (workerSocket > 0) {
                FD_SET(workerSocket, &readSet);
                if (workerSocket > maxSocket) {
                    maxSocket = workerSocket;
                }
            }
        }

        // Use select to check for readability of sockets
        int activity = select(maxSocket + 1, &readSet, NULL, NULL, NULL);

        if (activity == -1) {
            perror("Select error");
            exit(EXIT_FAILURE);
        }

        // Check for incoming connection
        if (FD_ISSET(serverSocket, &readSet)) {
            int newSocket = accept(serverSocket, NULL, NULL);
            printf("New connection, socket fd is %d\n", newSocket);

            // Add the new socket to the array of worker sockets
            for (int i = 0; i < MAX_WORKERS; ++i) {
                if (workerSockets[i] == 0) {
                    workerSockets[i] = newSocket;
                    appendToWorkerList(freeWorkers, newSocket);
                    break;
                }
            }
        }
        if (sharedList->head != NULL) {
            int workerSocket = getFirstFreeWorker();
            char* message = sharedList->head->data;
            int bytesRead = recv(workerSocket, message, sizeof(message), 0);
            if (bytesRead <= 0) {
                // Connection closed or error
                closesocket(workerSocket);
                printf("Failed sending the data to the worker");

            }
            else {
                removeLastElement(sharedList);
                moveWorkerNode(freeWorkers, busyWorkers, workerSocket);
            }
        }

        for (int i = 0; i < MAX_WORKERS; ++i) {
            int workerSocket = workerSockets[i];
            if (FD_ISSET(workerSocket, &readSet)) {
                int bytesRead = recv(workerSocket, buffer, sizeof(buffer), 0);

                if (bytesRead <= 0) {
                    // Connection closed or error
                    closesocket(workerSocket);
                    workerSockets[i] = 0;
                }
                else {
                    // Process the received data
                    buffer[bytesRead] = '\0';
                    printf("Received message from worker %d: %s\n", i, buffer);
                    if (!strcmp("DONE\0", buffer)) {
                        moveWorkerNode(busyWorkers, freeWorkers, workerSocket);
                    }
                    
                }
            }
        }
        
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}