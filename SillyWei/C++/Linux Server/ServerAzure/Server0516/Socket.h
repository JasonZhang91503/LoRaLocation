#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>


#define PORT "5555"
#define BACKLOG 10


void sigchld_handler(int s);
// get sockaddr, IPv4 or IPv6:
void* get_in_addr(struct sockaddr* sa);
int getInitializeListenSocket(struct sockaddr_storage &their_addr, socklen_t &sin_size, char s[]);
int getAcceptSocket(int intSocketListen, struct sockaddr_storage their_addr, socklen_t &sin_size, char s[]);

