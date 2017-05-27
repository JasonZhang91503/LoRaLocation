#include"rs232.h"
#include<iostream>
using namespace std;

#define TTYUSB0 16


int main(){
    //open ttyUSB0 ,16 = ttyUSB0;
    if(RS232_OpenComport(TTYUSB0,9600,"8N1")){
        cout << "OpenComport error" << endl;
        exit(1);
    }

    unsigned char 
    power[12] = {"FE FE EE FF"},
    speedHigh[12] = {"FE FE HH FF"},
    speedLow[12] = {"FE FE LL FF"},
    goX[15] = {"FE FE XX 00 FF"},
    goY[15] = {"FE FE YY 00 FF"};

    int input;
    cout << "Input :";
    cin >> input;

    switch(input){
    case 1:
        RS232_SendBuf(TTYUSB0,power,11);
        break;
    case 2:
        RS232_SendBuf(TTYUSB0,speedHigh,11);
        break;
    case 3:
        RS232_SendBuf(TTYUSB0,speedLow,11);
        break;
    case 4:
        RS232_SendBuf(TTYUSB0,goX,14);
        break;
    case 5:
        RS232_SendBuf(TTYUSB0,goY,14);
        break;
    }

    
    

    
}