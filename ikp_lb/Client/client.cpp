#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")



#define PORT 5059


int main() {
    WSADATA wsaData;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    // Create a socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        printf("Error creating socket: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Connect to the server
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &(serverAddr.sin_addr));

    iResult = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (iResult == SOCKET_ERROR) {
        printf("Connection failed: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Receive data from the server
    char buffer[1024];

    while (true) {

        printf("Posaljite poruku za cuvanje \n");
        printf("Za izlazak X\n");
        char message[100];
        // scanf_s("%s", message);
        fgets(message, sizeof(message), stdin);
        if (!strcmp(message, "X\n")) {
            printf("A");
            break;
            
        }

        int bytesSent = send(clientSocket, message, strlen(message), 0);

        if (bytesSent == SOCKET_ERROR) {
            printf("Send failed: %ld\n", WSAGetLastError());
            break;
        }

        printf("Message sent to server: %s\n", message);

        iResult = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (iResult > 0) {
            buffer[iResult] = '\0';
            printf("Received message from server: %s\n", buffer);
        }
        else if (iResult == 0) {
            printf("Connection closed by the server\n");
            break;
        }
        else {
            printf("Receive failed: %d\n", WSAGetLastError());
            break;
        }
    }

    // Close the socket
    closesocket(clientSocket);

    // Cleanup Winsock
    WSACleanup();

    return 0;
    
}