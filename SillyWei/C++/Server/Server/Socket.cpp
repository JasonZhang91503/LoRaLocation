#include "Socket.h"

SOCKET getInitializeListenSocket() {
	SOCKET listenSocket = INVALID_SOCKET;
	WSADATA wsaData;
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		cout << "WSAStartup failed: " << iResult << endl;
		system("PAUSE");
		exit(1);
	}

	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = MYAIFAMILY;
	hints.ai_socktype = MYAISOCKTYPE;
	hints.ai_flags = MYAIFLAGS;

	// Resolve the local address and port to be used by the server
	iResult = getaddrinfo(NULL, MYPORT, &hints, &result);
	if (iResult != 0) {
		cout << "getaddringo failed: " << iResult << endl;
		WSACleanup();
		system("PAUSE");
		exit(1);
	}

	// Setup the TCP listening socket
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (listenSocket == INVALID_SOCKET) {
			continue;
		}
		iResult = bind(listenSocket, result->ai_addr, result->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(listenSocket);
			continue;
		}
		break;
	}

	if (ptr == NULL) {
		cout << "bind failed with error: " << WSAGetLastError() << endl;
		freeaddrinfo(result);
		closesocket(listenSocket);
		WSACleanup();
		freeaddrinfo(result);
		system("PAUSE");
		exit(1);
	}
	freeaddrinfo(result);

	iResult = listen(listenSocket, MAXBACKLOG);
	if (iResult == SOCKET_ERROR) {
		cout << "listen failed with error: " << WSAGetLastError() << endl;
		closesocket(listenSocket);
		WSACleanup();
		freeaddrinfo(result);
		system("PAUSE");
		exit(1);
	}

	return listenSocket;
}

SOCKET getAcceptSocket(SOCKET listenSocket) {
	SOCKET acceptSocket;
	acceptSocket = INVALID_SOCKET;

	// Accept a client socket
	acceptSocket = accept(listenSocket, NULL, NULL);
	if (acceptSocket == INVALID_SOCKET) {
		cout << "accept failed: " << WSAGetLastError() << endl;
		closesocket(listenSocket);
		WSACleanup();
		system("PAUSE");
		exit(1);
	}

	return	acceptSocket;
}