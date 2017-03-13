#include "CellModel.h"
#define Max (size * size)
#define Position (i * size + j)

int setCheckerboardSize(); //�]�w�ѽL���j�p
void initializeBoard(struct cellModel[], int); //��l�ƴѽL
void setCheck(char []); //��l��check[26];
void displayServer(struct cellModel[][26], int); //�L�X���a�ιq���ѽL
void displayPlayer(struct cellModel[][26], int); //�L�X���a�ιq���ѽL
void setServerBoard(struct cellModel[][26], int); //�]�w���a���ѽL
void setClientBoard(struct cellModel[][26], int); //�]�w���a���ѽL
void randomFill(struct cellModel[], int); //�üƭ���ѽL
void manuallyFill(struct cellModel[][26], int, int); //��ʳ]�m�ѽL
void playerChoseNumber(struct cellModel[][26], char[], int, int); //���a��ܻ��G�Ʀr
int checkLine(struct cellModel[][26], int); //�ˬd�ӧQ�A0: �٨S�����A1: ���a�ӧQ�A2: �q���ӧQ�A3: ����

int setCheckerboardSize() {
	int size;
	printf("�п�ܴѽL�j�p�A���|���|�Τ�����(��J�Ʀr 4 or 5) -> ");
	while (1) {	//�ˬd�O�_���X�k�ѽL�j�p
		scanf("%d", &size);

		if (size == 4 || size == 5) {
			break;
		}
		printf("�S���o�j�p���ѽL�� ! �Э��s��� ->");
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
	printf("�o�O�ѽL���ˤl\n\n");
	printf("��:                                ���:");
	if (size == 5) {
		printf("\n�СССССССССССССС�     �СССССССССССССС�\n");
	}
	else {
		printf("\n�ССССССССССС�     �ССССССССССС�\n");
	}
	for (i = 0; i < size; i++) {
		for (j = 1; j <= size; j++) { //���a
			if (j % size == 1) {
				printf("|");
			}
			if (!cell[0][Position].mark) { //�ˬd�O�_���Q���
				printf(" %3d |", cell[0][Position].number);
			}else {
				printf("  ��  |");
			}
		}
		if (size == 5) { //���j
			printf("    ");
		}
		else {
			printf("    ");
		}
		for (j = 1; j <= size; j++) { //�q��
			if (j % size == 1) {
				printf("|");
			}
			if (!cell[1][Position].mark) { //�ˬd�O�_���Q���
				printf("  ��  |");
			}
			else {
				printf("  ��  |");
			}
			if (j % size == 0 && i * j != Max - size) {
				if (size == 5) {
					printf("\n�СССϡССϡССϡССϡС�     �СССϡССϡССϡССϡС�\n");
				}
				else {
					printf("\n�СССϡССϡССϡС�     �СССϡССϡССϡС�\n");
				}
			}
		}
	}
	if (size == 5) {
		printf("\n�СССССССССССССС�     �СССССССССССССС�\n\n");
	}
	else {
		printf("\n�ССССССССССС�     �ССССССССССС�\n\n");
	}
}

void displayClient(struct cellModel cell[][26], int size) {
    int i, j;
	printf("�o�O�ѽL���ˤl\n\n");
	printf("��:                                ���:");
	if (size == 5) {
		printf("\n�СССССССССССССС�     �СССССССССССССС�\n");
	}
	else {
		printf("\n�ССССССССССС�     �ССССССССССС�\n");
	}
	for (i = 0; i < size; i++) {
		for (j = 1; j <= size; j++) { //���a
			if (j % size == 1) {
				printf("|");
			}
			if (!cell[1][Position].mark) { //�ˬd�O�_���Q���
				printf(" %3d |", cell[1][Position].number);
			}else {
				printf("  ��  |");
			}
		}
		if (size == 5) { //���j
			printf("    ");
		}
		else {
			printf("    ");
		}
		for (j = 1; j <= size; j++) { //�q��
			if (j % size == 1) {
				printf("|");
			}
			if (!cell[0][Position].mark) { //�ˬd�O�_���Q���
				printf("  ��  |");
			}
			else {
				printf("  ��  |");
			}
			if (j % size == 0 && i * j != Max - size) {
				if (size == 5) {
					printf("\n�СССϡССϡССϡССϡС�     �СССϡССϡССϡССϡС�\n");
				}
				else {
					printf("\n�СССϡССϡССϡС�     �СССϡССϡССϡС�\n");
				}
			}
		}
	}
	if (size == 5) {
		printf("\n�СССССССССССССС�     �СССССССССССССС�\n\n");
	}
	else {
		printf("\n�ССССССССССС�     �ССССССССССС�\n\n");
	}
}

void setServerBoard(struct cellModel cell[][26], int size) {
	int choice = -1; //�Ψ��ˬd��J
	printf("�Q�ۤv��n�Ʀr�п�J1�A�ѹq���۰ʲ��Ϳ�J0 -> ");
	while (choice == -1) {
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			manuallyFill(cell, size, 0);
			printf("\e[2J"); //�M�z�ȹ�
			printf("�j�a�Ӫ����G !!! \n\n");
			displayServer(cell, size);
			printf("\n�w��n���a�ѽL!!");
			break;
		case 0:
			randomFill(cell[0], size);
			printf("\e[2J"); //�M�z�ȹ�
			printf("�j�a�Ӫ����G !!! \n\n");
			displayServer(cell, size);
			printf("\n�w�üƶ�n���a�ѽL!!");
			break;
		default:
			printf("��J���o! �Э��s��� ->");

			choice = -1;
			break;
		}
	}
}

void setClientBoard(struct cellModel cell[][26], int size) {
	int choice = -1; //�Ψ��ˬd��J
	printf("�Q�ۤv��n�Ʀr�п�J1�A�ѹq���۰ʲ��Ϳ�J0 -> ");
	while (choice == -1) {
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			manuallyFill(cell, size, 1);
			printf("\e[2J"); //�M�z�ȹ�
			printf("�j�a�Ӫ����G !!! \n\n");
			displayClient(cell, size);
			printf("\n�w��n���a�ѽL!!");
			break;
		case 0:
			randomFill(cell[1], size);
			printf("\e[2J"); //�M�z�ȹ�
			printf("�j�a�Ӫ����G !!! \n\n");
			displayClient(cell, size);
			printf("\n�w�üƶ�n���a�ѽL!!");
			break;
		default:
			printf("��J���o! �Э��s��� ->");

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
	for (i = 1; i <= Max; i++) {  //�~�P�洫�k�A���ͤ����ƪ��üƨ��k
		a = rand() % Max + 1;
		temp = cell[i].number;
		cell[i].number = cell[a].number;
		cell[a].number = temp;
	}
}

void manuallyFill(struct cellModel cell[][26], int size, int player) {
	int choice, check[26] = { 0 }, i; //�]�m�ѽL��

	printf("\e[2J"); //�M�z�ȹ�
	printf("�j�a�Ӫ����G !!! \n\n");
	if (player) {
        displayClient(cell, size);
	}else {
        displayServer(cell, size);
	}
	printf("�Ш̧Ƕ�J�A���N���Ʀr -> ");

	for (i = 1; i <= Max; i++) {
		scanf("%d", &choice); //��ܷQ��J�����X
		if (0 < choice && choice <= Max) {
			if (check[choice]) {
				printf("�o�Ʀr�w�g�Q��L�o�A�Э��s��� -> ");
				i--;
			}else {
				cell[player][i].number = choice;
				check[choice] = 1;

				printf("\e[2J"); //�M�z�ȹ�
				printf("�j�a�Ӫ����G !!! \n\n");
				if (player) {
                    displayClient(cell, size);
                }else {
                    displayServer(cell, size);
                }
				if (i == Max) {
					break;
				}
				printf("�U�ӼƦr -> ");
			}
		}else {
			printf("�W�L�ѽL�j�p�o�A�Э��s��� -> ");
			i--;
		}
	}
}

void playerChoseNumber(struct cellModel cell[][26], char check[], int size, int player) {
	int choice, i;
	printf("�п�@���X -> ");

	while (1){
		scanf("%d", &choice);
		if (!(0 < choice && choice <= Max)) {
            printf("�����X�W�X�d���o�A�Э��� -> ");
		}else if ((int)check[choice]){ //�ˬdrandomNumber�O�_���Q��L
			printf("�����X�w�Q��L�A�Э��� -> ");
		}else {
			for (i = 1; i <= Max; i++) {
				if (cell[0][i].number == choice) { //�b���a�ѽL�W�аO��쪺�Ʀr
					cell[0][i].mark = 1;
				}
				if (cell[1][i].number == choice) { //�b�q���ѽL�W�аO��쪺�Ʀr
					cell[1][i].mark = 1;
				}
			}
			check[choice] = 1; //�аO�w��L���Ʀr
			break;
		}
	}
	printf("\e[2J");
	printf("�j�a�Ӫ����G !!!  �C���}�l...\n\n");

    if (player) {
        displayClient(cell, size);
	}else {
        displayServer(cell, size);
	}
}

int checkLine(struct cellModel cell[][26], int size) {
	int playerCountLine = 0, //���a�p�ƥ�
		computerCountLine = 0, //�q���p�ƥ�
		playerAmount = 0,
		computerAmount = 0, i, j;

	for (i = 0; i < size; i++) { //�p�⪱�a�ιq����row���X���u
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

	for (i = 1; i <= size; i++) { //�p�⪱�a�ιq����column���X���u
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

	for (i = 0; i < size; i++) { //�p�⪱�a�ιq����diagonal���X���u
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
