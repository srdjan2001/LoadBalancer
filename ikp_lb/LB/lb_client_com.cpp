#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include "list.h"
#include <string.h>
#define PORT 5059
#define MAX_CLIENTS 10
#define BUFFER_SIZE 256

#pragma comment(lib, "ws2_32.lib")



DWORD WINAPI handleClients(LPVOID lpParam) {

    



    WSADATA wsaData;
    SOCKET clientSockets[MAX_CLIENTS];
    for (int i = 0; i < MAX_CLIENTS; i++) {
        clientSockets[i] = NULL;
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
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            int clientSocket = clientSockets[i];
            if (clientSocket > 0) {
                FD_SET(clientSocket, &readSet);
                if (clientSocket > maxSocket) {
                    maxSocket = clientSocket;
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

            // Add the new socket to the array of client sockets
            for (int i = 0; i < MAX_CLIENTS; ++i) {
                if (clientSockets[i] == 0) {
                    clientSockets[i] = newSocket;
                    break;
                }
            }
        }

        // Check for data from clients
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            int clientSocket = clientSockets[i];
            if (FD_ISSET(clientSocket, &readSet)) {
                int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
                
                if (bytesRead <= 0) {
                    // Connection closed or error
                    closesocket(clientSocket);
                    clientSockets[i] = 0;
                }
                else {
                    // Process the received data
                    buffer[bytesRead] = '\0';
                    appendToList(sharedList, buffer);
                    /*
                    
                    //Samo provjera da li radi iscitavanje poslednjeg elementa
                    
                    char *retValue;
                    retValue = getLastElement(sharedList);
                    
                    printf("ISCITANA POSLEDNJA POREKA %s", retValue);
                    */
                    printf("Received message from client %d: %s\n", i, buffer);
                    send(clientSocket, "ODGOVOR", 8, 0);
                    
                }
            }
        }
        
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
