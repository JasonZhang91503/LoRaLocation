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

    unsigned char buff[100] = "FE FE EE FF";
    //do{ 
        cout << "Input :";
        //cin.getline(buff,100);

        RS232_SendBuf(TTYUSB0,buff,12);
   // }while(buff != "q");
    

}