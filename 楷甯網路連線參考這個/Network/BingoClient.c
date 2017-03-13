#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "Bingo.h"
#define PORT "3490" // the port client will be connecting to
#define MAXDATASIZE 1000 // max number of bytes we can get at once
#define Max (size * size)
#define Position (i * size + j)
// get sockaddr, IPv4 or IPv6:

void *get_in_addr(struct sockaddr *sa){

	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[]){
	srand(time(NULL));
	int sockfd, numbytes;
	char buff[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];

	if (argc != 2) {
		fprintf(stderr,"usage: client hostname\n");
		exit(1);
	}
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo("0.0.0.0", PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
			p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}
		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			continue;
		}
		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
	printf("client: connecting to %s\n", s);
	freeaddrinfo(servinfo); // all done with this structure

//程式開始
	
	struct outCellModel oCell, *oCellP; //cell[0][]是玩家, cell[1][]是電腦
	int checkerboardSize, //設定棋盤大小
		now, //用來延遲時間
		
		winner; //記錄誰是贏家
	char *charP, c = 0;

	recv(sockfd, buff, MAXDATASIZE - 1, 0);
	checkerboardSize = *(int*)buff;
	
	printf("\e[2J");
	
	printf("大家來玩賓果 !!! \n\n");

	recv(sockfd, buff, MAXDATASIZE - 1, 0);
	oCell = *(struct outCellModel*)buff;

	displayClient(oCell.cell, checkerboardSize); //印出棋盤初始畫面

	recv(sockfd, buff, MAXDATASIZE - 1, 0);
	oCell = *(struct outCellModel*)buff;

	setClientBoard(oCell.cell, checkerboardSize); //填入玩家賓果數字

	oCellP = &oCell;
	charP = (char*)oCellP;
	send(sockfd, charP, sizeof(oCell), 0);

	printf("\e[2J");

	printf("大家來玩賓果 !!!  Go...\n\n");
	displayClient(oCell.cell, checkerboardSize);

	while (1) {
		recv(sockfd, buff, MAXDATASIZE - 1, 0);
		oCell = *(struct outCellModel*)buff;

		printf("\e[2J");

		printf("大家來玩賓果 !!!  Go...\n\n");
		displayClient(oCell.cell, checkerboardSize);
		if (winner = checkLine(oCell.cell, checkerboardSize)){
			break;
		}
		playerChoseNumber(oCell.cell, oCell.check, checkerboardSize, 1); //玩家選一數字
		oCellP = &oCell;
		charP = (char*)oCellP;
		send(sockfd, charP, sizeof(oCell), 0);
		if (winner = checkLine(oCell.cell, checkerboardSize)){
			break;
		}
	}

	printf("\e[2J");

	printf("大家來玩賓果 !!!  遊戲結束...\n\n");
	displayClient(oCell.cell, checkerboardSize);

	switch (winner){
	case 2:
		printf("你贏了!!\n\n");
		break;
	case 1:
		printf("你輸了!! 哈哈哈哈\n\n");
		break;
	default:
		printf("平手!!\n\n");
		break;
	}

	close(sockfd);
	return 0;
}
