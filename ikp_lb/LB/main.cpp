#include <stdio.h>
#include <winsock2.h>
#include "lb_client_com.h"
#include <string.h>

#include "list.h"

#define PORT 5059
#define MAX_WORKERS 10
#define BUFFER_SIZE 256

#pragma comment(lib, "ws2_32.lib")

List* sharedList = initializeList();


int main() {

	
	

	
	handleClients();
	char msg[100] = "poruka2";
	appendToList(sharedList, msg);
	printf("LISTA:\n");
	printList(sharedList);





	return 0;
}