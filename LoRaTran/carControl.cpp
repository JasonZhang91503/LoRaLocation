#include <stdio.h>  
#include <termios.h>  
#include <unistd.h>  
#include <fcntl.h>  

#include"rs232.h"
#include<iostream>
using namespace std;

#define TTYUSB0 16
#define MILI 1000

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
    leftX[15] = {"FE FE XX 30 FF"},
    rightX[15] = {"FE FE XX 70 FF"},
    goY[15] = {"FE FE YY 70 FF"},
    backY[15] = {"FE FE YY 30 FF"},
    mX[15] = {"FE FE XX 50 FF"},
    mY[15] = {"FE FE YY 50 FF"};


    char c;
    bool stop = true;

    do{

        while(!kbhit()){
            if(!stop){
                RS232_SendBuf(TTYUSB0,mX,14);
                usleep(100 * MILI);
                RS232_SendBuf(TTYUSB0,mY,14);
                stop = true;
            }
            usleep(200 * MILI);
        }

        

        c = getchar();

        printf("INPUT : %d\n",(int)c);

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
        case 27:
            c = getchar();
            printf("a : %d\n",(int)c);
            c = getchar();
            printf("b : %d\n",(int)c);
            if(c == 65){ 
                RS232_SendBuf(TTYUSB0,goY,14);
                printf("send");
            }
            else if(c == 66){ 
                RS232_SendBuf(TTYUSB0,backY,14);
                printf("send");
            }
            else if(c == 68){ 
                RS232_SendBuf(TTYUSB0,leftX,14);
                printf("send");
            }
            else if(c == 67){ 
                RS232_SendBuf(TTYUSB0,rightX,14);
                printf("send");
            }

            stop = false;
            break;
        case 'z':
            RS232_SendBuf(TTYUSB0,mX,14);
            break;
        case 'x':
            RS232_SendBuf(TTYUSB0,mY,14);
            break;
        case ' ':
            RS232_SendBuf(TTYUSB0,mX,14);
            usleep(100 * MILI);
            RS232_SendBuf(TTYUSB0,mY,14);
            break;
        }
        

        
    }while(c != 'q');


    usleep(200 * MILI);
}