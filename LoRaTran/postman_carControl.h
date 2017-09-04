#include <stdio.h>  
#include <termios.h>  
#include <unistd.h>  
#include <fcntl.h>  

#include"rs232.h"
#include<iostream>
using namespace std;
using namespace unistd;

#define TTYUSB0 16
#define MILI 1000

#define LEFT_T 2000
#define RIGHT_T 2000
#define R_AROUND_T 3400

unsigned char 
power[12] = {"FE FE EE FF"},
speedHigh[12] = {"FE FE HH FF"},
speedLow[12] = {"FE FE LL FF"},

leftX[15] = {"FE FE XX 23 FF"},
leftX2[15] = {"FE FE XX 43 FF"},

rightX[15] = {"FE FE XX 70 FF"},
rightX2[15] = {"FE FE XX 55 FF"},

goY[15] = {"FE FE YY 85 FF"},
goY2[15] = {"FE FE YY 55 FF"},

backY[15] = {"FE FE YY 30 FF"},
backY2[15] = {"FE FE YY 45 FF"},

mX[15] = {"FE FE XX 50 FF"},
mY[15] = {"FE FE YY 50 FF"};

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

class carControl{
public:
    static carControl* getInstance(int comport){
        if(!instance){
            instance = new carControl(comport);
        }
        return instance;
    }

    carControl(int comport){
        if(RS232_OpenComport(comport,9600,"8N1")){
            printf("OpenComport error\n");
            exit(1);
        }
        else{
            printf("carControl : car OK\n");
        }
    }
    
    void turnLeft(){
        RS232_SendBuf(TTYUSB0,leftX,14);
        usleep(LEFT_T * MILI);
        stop();
    }

    void turnRight(){
        RS232_SendBuf(TTYUSB0,rightX,14);
        usleep(RIGHT_T * MILI);
        stop();
    }

    void turnAround(){
        RS232_SendBuf(TTYUSB0,rightX,14);
        usleep(R_AROUND_T * MILI);
        stop();
    }

    void goStraight(){
        RS232_SendBuf(TTYUSB0,goY,14);
        usleep(100 * MILI);
    }

    void leftSmall(){
        RS232_SendBuf(TTYUSB0,leftX2,14);
    }

    void rightSmall(){
        RS232_SendBuf(TTYUSB0,rightX2,14);
    }


    void stop(){
        RS232_SendBuf(TTYUSB0,mX,14);
        usleep(100 * MILI);
        RS232_SendBuf(TTYUSB0,mY,14);
        usleep(100 * MILI);
        forward = false;
    }

    void setDir(int dir){
        currentDir = dir;
    }

    int getDir(){
        return currentDir;
    }

    void setDirAngle(float Angle){
        dirAngle = Angle;
    }

    float getDirAngle(){
        return dirAngle;
    }

    void setForward(int f){
        forward = f;
    }

    int getForward(){
        return forward;
    }


private:
    static carControl* instance;
    int currentDir = 1;
    float dirAngle = 0;
    bool forward = false;
};

carControl* carControl::instance = 0;