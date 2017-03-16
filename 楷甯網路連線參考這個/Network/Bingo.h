#include "CellModel.h"
#define Max (size * size)
#define Position (i * size + j)

int setCheckerboardSize(); //設定棋盤的大小
void initializeBoard(struct cellModel[], int); //初始化棋盤
void setCheck(char []); //初始化check[26];
void displayServer(struct cellModel[][26], int); //印出玩家及電腦棋盤
void displayPlayer(struct cellModel[][26], int); //印出玩家及電腦棋盤
void setServerBoard(struct cellModel[][26], int); //設定玩家的棋盤
void setClientBoard(struct cellModel[][26], int); //設定玩家的棋盤
void randomFill(struct cellModel[], int); //亂數重填棋盤
void manuallyFill(struct cellModel[][26], int, int); //手動設置棋盤
void playerChoseNumber(struct cellModel[][26], char[], int, int); //玩家選擇賓果數字
int checkLine(struct cellModel[][26], int); //檢查勝利，0: 還沒結束，1: 玩家勝利，2: 電腦勝利，3: 平手

int setCheckerboardSize() {
	int size;
	printf("請選擇棋盤大小，有四乘四或五乘五(輸入數字 4 or 5) -> ");
	while (1) {	//檢查是否為合法棋盤大小
		scanf("%d", &size);

		if (size == 4 || size == 5) {
			break;
		}
		printf("沒有這大小的棋盤喔 ! 請重新選擇 ->");
	}
	return size;
}

void initializeBoard(struct cellModel cell[], int size) {
	int i;
	for (i = 1; i <= Max; i++) {
		cell[i].mark = 0;
		cell[i].number = 0;
	}
}

void setCheck(char check[]){
    int i;
    for(i = 1; i<= 26; i ++){
        check[i] = 0;
    }
}

void displayServer(struct cellModel cell[][26], int size) {
    int i, j;
	printf("這是棋盤的樣子\n\n");
	printf("我:                                對手:");
	if (size == 5) {
		printf("\n－－－－－－－－－－－－－－－     －－－－－－－－－－－－－－－\n");
	}
	else {
		printf("\n－－－－－－－－－－－－     －－－－－－－－－－－－\n");
	}
	for (i = 0; i < size; i++) {
		for (j = 1; j <= size; j++) { //玩家
			if (j % size == 1) {
				printf("|");
			}
			if (!cell[0][Position].mark) { //檢查是否有被選取
				printf(" %3d |", cell[0][Position].number);
			}else {
				printf("  ●  |");
			}
		}
		if (size == 5) { //分隔
			printf("    ");
		}
		else {
			printf("    ");
		}
		for (j = 1; j <= size; j++) { //電腦
			if (j % size == 1) {
				printf("|");
			}
			if (!cell[1][Position].mark) { //檢查是否有被選取
				printf("  ○  |");
			}
			else {
				printf("  ●  |");
			}
			if (j % size == 0 && i * j != Max - size) {
				if (size == 5) {
					printf("\n－－－＋－－＋－－＋－－＋－－     －－－＋－－＋－－＋－－＋－－\n");
				}
				else {
					printf("\n－－－＋－－＋－－＋－－     －－－＋－－＋－－＋－－\n");
				}
			}
		}
	}
	if (size == 5) {
		printf("\n－－－－－－－－－－－－－－－     －－－－－－－－－－－－－－－\n\n");
	}
	else {
		printf("\n－－－－－－－－－－－－     －－－－－－－－－－－－\n\n");
	}
}

void displayClient(struct cellModel cell[][26], int size) {
    int i, j;
	printf("這是棋盤的樣子\n\n");
	printf("我:                                對手:");
	if (size == 5) {
		printf("\n－－－－－－－－－－－－－－－     －－－－－－－－－－－－－－－\n");
	}
	else {
		printf("\n－－－－－－－－－－－－     －－－－－－－－－－－－\n");
	}
	for (i = 0; i < size; i++) {
		for (j = 1; j <= size; j++) { //玩家
			if (j % size == 1) {
				printf("|");
			}
			if (!cell[1][Position].mark) { //檢查是否有被選取
				printf(" %3d |", cell[1][Position].number);
			}else {
				printf("  ●  |");
			}
		}
		if (size == 5) { //分隔
			printf("    ");
		}
		else {
			printf("    ");
		}
		for (j = 1; j <= size; j++) { //電腦
			if (j % size == 1) {
				printf("|");
			}
			if (!cell[0][Position].mark) { //檢查是否有被選取
				printf("  ○  |");
			}
			else {
				printf("  ●  |");
			}
			if (j % size == 0 && i * j != Max - size) {
				if (size == 5) {
					printf("\n－－－＋－－＋－－＋－－＋－－     －－－＋－－＋－－＋－－＋－－\n");
				}
				else {
					printf("\n－－－＋－－＋－－＋－－     －－－＋－－＋－－＋－－\n");
				}
			}
		}
	}
	if (size == 5) {
		printf("\n－－－－－－－－－－－－－－－     －－－－－－－－－－－－－－－\n\n");
	}
	else {
		printf("\n－－－－－－－－－－－－     －－－－－－－－－－－－\n\n");
	}
}

void setServerBoard(struct cellModel cell[][26], int size) {
	int choice = -1; //用來檢查輸入
	printf("想自己填好數字請輸入1，由電腦自動產生輸入0 -> ");
	while (choice == -1) {
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			manuallyFill(cell, size, 0);
			printf("\e[2J"); //清理銀幕
			printf("大家來玩賓果 !!! \n\n");
			displayServer(cell, size);
			printf("\n已填好玩家棋盤!!");
			break;
		case 0:
			randomFill(cell[0], size);
			printf("\e[2J"); //清理銀幕
			printf("大家來玩賓果 !!! \n\n");
			displayServer(cell, size);
			printf("\n已亂數填好玩家棋盤!!");
			break;
		default:
			printf("輸入錯囉! 請重新選擇 ->");

			choice = -1;
			break;
		}
	}
}

void setClientBoard(struct cellModel cell[][26], int size) {
	int choice = -1; //用來檢查輸入
	printf("想自己填好數字請輸入1，由電腦自動產生輸入0 -> ");
	while (choice == -1) {
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			manuallyFill(cell, size, 1);
			printf("\e[2J"); //清理銀幕
			printf("大家來玩賓果 !!! \n\n");
			displayClient(cell, size);
			printf("\n已填好玩家棋盤!!");
			break;
		case 0:
			randomFill(cell[1], size);
			printf("\e[2J"); //清理銀幕
			printf("大家來玩賓果 !!! \n\n");
			displayClient(cell, size);
			printf("\n已亂數填好玩家棋盤!!");
			break;
		default:
			printf("輸入錯囉! 請重新選擇 ->");

			choice = -1;
			break;
		}
	}
}

void randomFill(struct cellModel cell[], int size) {
	int temp, a, i;
	for (i = 1; i <= Max; i++) {
		cell[i].number = i;
	}
	for (i = 1; i <= Max; i++) {  //洗牌交換法，產生不重複的亂數取法
		a = rand() % Max + 1;
		temp = cell[i].number;
		cell[i].number = cell[a].number;
		cell[a].number = temp;
	}
}

void manuallyFill(struct cellModel cell[][26], int size, int player) {
	int choice, check[26] = { 0 }, i; //設置棋盤用

	printf("\e[2J"); //清理銀幕
	printf("大家來玩賓果 !!! \n\n");
	if (player) {
        displayClient(cell, size);
	}else {
        displayServer(cell, size);
	}
	printf("請依序填入你中意的數字 -> ");

	for (i = 1; i <= Max; i++) {
		scanf("%d", &choice); //選擇想填入的號碼
		if (0 < choice && choice <= Max) {
			if (check[choice]) {
				printf("這數字已經被選過囉，請重新選擇 -> ");
				i--;
			}else {
				cell[player][i].number = choice;
				check[choice] = 1;

				printf("\e[2J"); //清理銀幕
				printf("大家來玩賓果 !!! \n\n");
				if (player) {
                    displayClient(cell, size);
                }else {
                    displayServer(cell, size);
                }
				if (i == Max) {
					break;
				}
				printf("下個數字 -> ");
			}
		}else {
			printf("超過棋盤大小囉，請重新選擇 -> ");
			i--;
		}
	}
}

void playerChoseNumber(struct cellModel cell[][26], char check[], int size, int player) {
	int choice, i;
	printf("請選一號碼 -> ");

	while (1){
		scanf("%d", &choice);
		if (!(0 < choice && choice <= Max)) {
            printf("此號碼超出範圍囉，請重選 -> ");
		}else if ((int)check[choice]){ //檢查randomNumber是否有被選過
			printf("此號碼已被選過，請重選 -> ");
		}else {
			for (i = 1; i <= Max; i++) {
				if (cell[0][i].number == choice) { //在玩家棋盤上標記選到的數字
					cell[0][i].mark = 1;
				}
				if (cell[1][i].number == choice) { //在電腦棋盤上標記選到的數字
					cell[1][i].mark = 1;
				}
			}
			check[choice] = 1; //標記已選過的數字
			break;
		}
	}
	printf("\e[2J");
	printf("大家來玩賓果 !!!  遊戲開始...\n\n");

    if (player) {
        displayClient(cell, size);
	}else {
        displayServer(cell, size);
	}
}

int checkLine(struct cellModel cell[][26], int size) {
	int playerCountLine = 0, //玩家計數用
		computerCountLine = 0, //電腦計數用
		playerAmount = 0,
		computerAmount = 0, i, j;

	for (i = 0; i < size; i++) { //計算玩家及電腦的row有幾條線
		for (j = 1; j <= size; j++) {
			playerAmount += cell[0][Position].mark;
			computerAmount += cell[1][Position].mark;
		}
		if (playerAmount == size) {
			playerCountLine++;
		}
		if (computerAmount == size) {
			computerCountLine++;
		}
		playerAmount = 0;
		computerAmount = 0;
	}

	for (i = 1; i <= size; i++) { //計算玩家及電腦的column有幾條線
		for (j = 0; j < size; j++) {
			playerAmount += cell[0][j * size + i].mark;
			computerAmount += cell[1][j * size + i].mark;
		}
		if (playerAmount == size) {
			playerCountLine++;
		}
		if (computerAmount == size) {
			computerCountLine++;
		}
		playerAmount = 0;
		computerAmount = 0;
	}

	for (i = 0; i < size; i++) { //計算玩家及電腦的diagonal有幾條線
		playerAmount += cell[0][1 + (size + 1) * i].mark;
		computerAmount += cell[1][1 + (size + 1) * i].mark;
	}
	if (playerAmount == size) {
		playerCountLine++;
	}
	if (computerAmount == size) {
		computerCountLine++;
	}
	playerAmount = 0;
	computerAmount = 0;

	for (i = 0; i < size; i++) {
		playerAmount += cell[0][1 + size * (size - 1) - (size - 1) * i].mark;
		computerAmount += cell[1][1 + size * (size - 1) - (size - 1) * i].mark;
	}
	if (playerAmount == size) {
		playerCountLine++;
	}
	if (computerAmount == size) {
		computerCountLine++;
	}

	if (playerCountLine >= 3 && computerCountLine >= 3){
		return 3;
	}else if (playerCountLine >= 3){
		return 1;
	}else if(computerCountLine >= 3) {
		return 2;
	}else {
		return 0;
	}
}
