#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define MYAIFAMILY AF_INET
#define MYAISOCKTYPE SOCK_STREAM
#define MYAIFLAGS AI_PASSIVE
#define MYPORT "5000"
#define MAXDATASIZE 1000
#define MAXBACKLOG 4
#define MAXNUMBEROFPLAYER 4

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <stdlib.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

SOCKET getInitializeListenSocket();
// Check accept and return connected socket
SOCKET getAcceptSocket(SOCKET listenSocket);
