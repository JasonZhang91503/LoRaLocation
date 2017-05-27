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

    unsigned char buff[11];
    buff[0] = 'F';
    buff[1] = 'E';
    buff[2] = ' ';
    buff[3] = 'F';
    buff[4] = 'E';
    buff[5] = ' ';
    buff[6] = 'E';
    buff[7] = 'E';
    buff[8] = ' ';
    buff[9] = 'F';
    buff[10] = 'F';

    //do{ 
        cout << "Input :";
        //cin.getline(buff,100);

        RS232_SendBuf(TTYUSB0,buff,11);
   // }while(buff != "q");
    

}