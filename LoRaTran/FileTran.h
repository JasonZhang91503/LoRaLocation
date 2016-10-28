#include<fstream>
#include<iostream>
#include<string>
using namespace std;


fstream file;

int fileOpen(char textName[]){
    string textStr = textName;
    file.open(textName,ios::out | ios::app);
    if(!file){
	cout<<"file not found!:" + textStr << endl;
	return 1;
    }
    cout << "Open file:" + textStr << endl;
}

void fileInput(char line[]){
    string lineStr = line;
    if(!file){
        cout << "file not open!" << endl;
	return;
    }
    file << lineStr << endl;
}

void filePrint(char line[]){
	for(int i = 0; line[i] != '\0'; i++){	
		printf("%c",line[i]);
	}
	printf("\n");

	if (!file) {
		cout << "file not open!" << endl;
		return;
	}
	fileInput(line);
}

void fileClose(){
	if (!file) {
		cout << "file not open!" << endl;
		return;
	}

    file.close();
    cout << "file close" << endl;
}


