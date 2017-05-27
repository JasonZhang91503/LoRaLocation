#include <stdio.h>  
#include <termios.h>  
#include <unistd.h>  
#include <fcntl.h>  

#include"rs232.h"
#include<iostream>
using namespace std;

#define TTYUSB0 16

int kbhit(void)  
{  
    struct termios oldt, newt;  
    int ch;  
    int oldf;  
    tcgetattr(STDIN_FILENO, &oldt);  
    newt = oldt;  
    newt.c_lflag &= ~(ICANON | ECHO);  
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);  
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);  
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);  
    ch = getchar();  
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);  
    fcntl(STDIN_FILENO, F_SETFL, oldf);  
    if(ch != EOF)  
    {  
        ungetc(ch, stdin);  
        return 1;  
    }  
    return 0;  
}  


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


    char c;

    do{

        while(!kbhit());

        c = getchar();

        /*
        int input;
        cout << "Input :";
        cin >> input;
        */
        switch(c){
        case 'e':
            RS232_SendBuf(TTYUSB0,power,11);
            break;
        case 'h':
            RS232_SendBuf(TTYUSB0,speedHigh,11);
            break;
        case 'l':
            RS232_SendBuf(TTYUSB0,speedLow,11);
            break;
        case 'x':
            RS232_SendBuf(TTYUSB0,goX,14);
            break;
        case 'y':
            RS232_SendBuf(TTYUSB0,goY,14);
            break;
        }

        
    }while(c != 'q');
}