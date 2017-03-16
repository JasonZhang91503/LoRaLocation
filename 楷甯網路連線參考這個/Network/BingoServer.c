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
#include "Bingo.h"
#define PORT "3490"
#define BACKLOG 10
#define MAXDATASIZE 1000
#define Max (size * size)
#define Position (i * size + j)

void sigchld_handler(int s){
	while(waitpid(-1, NULL, WNOHANG) > 0);
}
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa){

	if (sa->sa_family == AF_INET) {

		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void){
	srand(time(NULL));
	int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;
	char s[INET6_ADDRSTRLEN], buff[MAXDATASIZE];
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo("0.0.0.0", PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}
		break;
	}
	if (p == NULL) {
		fprintf(stderr, "server: failed to bind\n");
		return 2;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	printf("server: waiting for connections...\n");

	sin_size = sizeof their_addr;
	new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
	if (new_fd == -1) {
		perror("accept");
	}

	inet_ntop(their_addr.ss_family,	get_in_addr((struct sockaddr *)&their_addr),s, sizeof s);
	printf("server: got connection from %s\n", s);

//程式開始
	
	struct outCellModel oCell, *oCellP; //cell[0][]是玩家, cell[1][]是電腦
	int checkerboardSize = setCheckerboardSize(), //設定棋盤大小
		now, //用來延遲時間
		winner, //記錄誰是贏家
		*intP;
	char *charP;

	intP = &checkerboardSize;
	charP = (char*)intP;
	send(new_fd, charP, sizeof(checkerboardSize), 0);
	
	printf("\e[2J");

	printf("大家來玩賓果 !!! \n\n");
	initializeBoard(oCell.cell[0], checkerboardSize); //初始化玩家棋盤
	initializeBoard(oCell.cell[1], checkerboardSize); //初始化電腦棋盤
	setCheck(oCell.check); //初始化check[26]

	oCellP = &oCell;
	charP = (char*)oCellP;
	send(new_fd, charP, sizeof(oCell), 0);
	
	displayServer(oCell.cell, checkerboardSize); //印出棋盤初始畫面

	setServerBoard(oCell.cell, checkerboardSize); //填入玩家賓果數字
	
	oCellP = &oCell;
	charP = (char*)oCellP;
	send(new_fd, charP, sizeof(oCell), 0);

	recv(new_fd, buff, MAXDATASIZE - 1, 0);
	oCell = *(struct outCellModel*)buff;

	printf("\e[2J");

	printf("大家來玩賓果 !!!  Go...\n\n");
	displayServer(oCell.cell, checkerboardSize);

	while (1) {
		playerChoseNumber(oCell.cell, oCell.check, checkerboardSize, 0); //玩家選一數字
		oCellP = &oCell;
		charP = (char*)oCellP;
		send(new_fd, charP, sizeof(oCell), 0);
		if (winner = checkLine(oCell.cell, checkerboardSize)){
			break;
		}
		recv(new_fd, buff, MAXDATASIZE - 1, 0);
		oCell = *(struct outCellModel*)buff;

		printf("\e[2J");

		printf("大家來玩賓果 !!!  Go...\n\n");
		displayServer(oCell.cell, checkerboardSize);
		if (winner = checkLine(oCell.cell, checkerboardSize)){
			break;
		}
	}


	printf("\e[2J");

	printf("大家來玩賓果 !!!  遊戲結束...\n\n");
	displayServer(oCell.cell, checkerboardSize);

	switch (winner){
	case 1:
		printf("你贏了!!\n\n");
		break;
	case 2:
		printf("你輸了!! 哈哈哈哈\n\n");
		break;
	default:
		printf("平手!!\n\n");
		break;
	}

	close(new_fd); // parent doesn't need this
	return 0;
}