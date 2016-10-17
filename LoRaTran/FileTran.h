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

void fileClose(){
    file.close();
    cout << "file close" << endl;
}
