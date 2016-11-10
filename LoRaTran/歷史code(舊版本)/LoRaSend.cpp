//
//  main.cpp
//  Lora_Setup_Nov
//
//  Created by Keng on 08/11/2016.
//  Copyright © 2016 Keng. All rights reserved.
//

#include "arduPiLoRa.h"
#include "Lora_Setup.h"
//#include "Bluetooth.h"
#include <iostream>
#include <string>

using namespace std;

int e;
int num = 0;
int NodeAddress;
char SendStr[100];
//Position GPSInfo;

/*void loop_GPS(int e)
{
    num++;
    // Send packet broadcast and print the result
    sprintf(SendStr,"%d %s %s",NodeAddress,GPSInfo.latitude,GPSInfo.longitude);
    e = sx1272.sendPacketTimeout(0, SendStr);
    printf("Packet sent, state %d\n",e);
}*/
void loop_Msg(int e)
{
    // Send packet broadcast and print the result
    sprintf(SendStr,"%d %d",NodeAddress,++num);
    e = sx1272.sendPacketTimeout(0, SendStr);
    printf("Packet sent, state %d\n",e);
}
int main (int argc, char **argv){
    
    int op;
    cout<<"Your NodeAddres is 0(Sender is going to broadcast)"<<endl;
    NodeAddress=0;
    e=setup(NodeAddress);
    cout<<"Select mode :"<<endl;
    cout<<"(1)Send message"<<endl;
    cout<<"(2)Send Gps (unsuccessful)"<<endl;
    cin>>op;
    ClearScreen();
    //BluetoothInitial();
    while(1){
        switch (op) {
            case 1:
                loop_Msg(e);
                break;
            case 2:
                cout<<"It cannot use now"<<endl;
                return 0;
            default:
                cout<<"Please input correct mode"<<endl;
                return 0;
        }
        //GPSInfo=readFromBluetooth();
    }
    return (0);
}
