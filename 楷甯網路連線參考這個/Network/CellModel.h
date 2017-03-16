struct cellModel { //棋盤結構
	int mark;
	int number;
};

struct outCellModel {
    struct cellModel cell[2][26];
    char check[26];
};
