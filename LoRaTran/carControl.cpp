#include <stdio.h>  
#include <termios.h>  
#include <unistd.h>  
#include <fcntl.h>  

#include"rs232.h"
#include<iostream>
using namespace std;

#define TTYUSB0 16  //USB孔的TTY
#define MILI 1000   //1000us

#define LEFT_T 2000 //2000us
#define RIGHT_T 2000    //2000us

unsigned char 
power[12] = {"FE FE EE FF"},    //開關封包
speedHigh[12] = {"FE FE HH FF"},    //速度up
speedLow[12] = {"FE FE LL FF"},     //速度down

leftX[15] = {"FE FE XX 28 FF"}, //大左
leftX2[15] = {"FE FE XX 47 FF"},    //小左

rightX[15] = {"FE FE XX 70 FF"}, //大右
rightX2[15] = {"FE FE XX 55 FF"},//小右

goY[15] = {"FE FE YY 85 FF"},//大前
goY2[15] = {"FE FE YY 55 FF"},//小前

backY[15] = {"FE FE YY 30 FF"},//大後
backY2[15] = {"FE FE YY 45 FF"},//小後

mX[15] = {"FE FE XX 50 FF"},//X回到中心
mY[15] = {"FE FE YY 50 FF"};//Y回到中心

//這個函式是用來偵測鍵盤是否有被按下
//有的話回傳1
//沒有回傳0
//這個網路上找的，我也看不懂QQ，但能用
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
        return;
    }


    char c;

    do{

        //若沒有鍵盤按鍵被按下，就無限迴圈，按下則離開迴圈
        while(!kbhit()){
        }


        //用getChar得知被按下的ASKII
        c = getchar();

        printf("INPUT : %d\n",(int)c);

        switch(c){
            //開關機
        case 'e':
            RS232_SendBuf(TTYUSB0,power,11);
            break;
            //加速
        case 'h':
            RS232_SendBuf(TTYUSB0,speedHigh,11);
            break;
            //減速
        case 'l':
            RS232_SendBuf(TTYUSB0,speedLow,11);
            break;
            //27這個是上下左右的判斷
            //通常按下上下左右，回傳的是三個ASKII的組合，而不是單一ASKII值
            //所以首先會讀取到第一個ASKII是27
        case 27:
            //接著在讀取第二個值，這個值會是91，但可以忽略不理她
            c = getchar();
            printf("a : %d\n",(int)c);
            //接下來第三個值比較重要，他分別是 上 = 65，下 = 66， 左 = 68， 右 = 67
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
                RS232_SendBuf(TTYUSB0,leftX2,14);
                printf("send");
            }
            else if(c == 67){ 
                RS232_SendBuf(TTYUSB0,rightX2,14);
                printf("send");
            }
            break;
            //按下z ， 則x軸回到中心
        case 'z':
            RS232_SendBuf(TTYUSB0,mX,14);
            break;
            //按下x , 則Y軸回到中心
        case 'x':
            RS232_SendBuf(TTYUSB0,mY,14);
            break;
            //按下空白建，兩軸回到中心
            //按下5也是
        case ' ':
        case '5':
            RS232_SendBuf(TTYUSB0,mX,14);
            usleep(100 * MILI);
            RS232_SendBuf(TTYUSB0,mY,14);
            usleep(100 * MILI);
            break;
            //就是8
        case '8':
            RS232_SendBuf(TTYUSB0,goY,14);
            break;
            //就是2
        case '2':
            RS232_SendBuf(TTYUSB0,backY,14);
            break;
            //就是4
        case '4':
            RS232_SendBuf(TTYUSB0,leftX,14);
            break;
            //就是6
        case '6':
            RS232_SendBuf(TTYUSB0,rightX,14);
            break;

        }
    }while(c != 'q');
}