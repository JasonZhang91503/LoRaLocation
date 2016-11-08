//
//  main.cpp
//  Lora_Setup_Nov
//
//  Created by Keng on 08/11/2016.
//  Copyright © 2016 Keng. All rights reserved.
//

#include "arduPiLoRa.h"
#include "Lora_Setup.h"
#include "Bluetooth.h"
#include <iostream>
#include <string>

using namespace std;

int e;
int num = 0;
int NodeAddress;
char *SendStr;
Position GPSInfo;

void loop(int e)
{
    num++;
    // Send packet broadcast and print the result
    sprintf(SendStr,"%d %s %s",NodeAddress,GPSInfo.latitude,GPSInfo.longitude);
    e = sx1272.sendPacketTimeout(0, SendStr);
    printf("Packet sent, state %d\n",e);
}

int main (int argc, char **argv){
    

    cout<<"Your NodeAddres is 0(Sender is going to broadcast)"<<endl;
    NodeAddress=0;
    e=setup(NodeAddress);
    BluetoothInitial();
    while(1){
        GPSInfo=readFromBluetooth();
        loop();
    }
    return (0);
}
