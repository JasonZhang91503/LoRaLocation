//
//  main.cpp
//  LoraSendNOV
//
//  Created by Keng on 08/11/2016.
//  Copyright © 2016 Keng. All rights reserved.
//

#include "arduPiLoRa.h"
#include "Lora_Setup.h"
#include "FileTran.h"
#include <iostream>
#include <string>
using namespace std;

int e;
int num = 0;
int NodeAddress;
char *recvGPSMsg;
char lastPacketMessage[100];
//用他產生得message字串記得要free掉，不然程式可能會爆掉
char* recvMessage() {
    // Receive message
    cout<<"Wait for sent..."<<endl;
    e = sx1272.receivePacketTimeout(10000);
    
    char* messageStr;
    
    if (e == 0)
    {
        messageStr = (char*)malloc(sizeof(char)*sx1272.packet_received.length);
        printf("Receive packet, state %d\n", e);
        
        for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
        {
            lastPacketMessage[i] = (char)sx1272.packet_received.data[i];
            messageStr[i] = lastPacketMessage[i];
        }
        cout<<"message:"<<endl;
        fileInput(lastPacketMessage);
        
        return messageStr;
    }
    else {
        printf("Receive packet, state %d\n", e);
        
        return NULL;
    }
}

void loop()
{
    recvGPSMsg = recvMessage();	//利用LoRa函式接收傳來的資料，並用指標存
    free(recvGPSMsg);//最後記得free掉
}

int main (int argc, char **argv){
    
    cout<<"Set Receiver NodeAddress (1=A,2=B,3=C)"<<endl;
    cin>>NodeAddress;
    
    if (argv[1] != NULL) {
        fileOpen(argv[1]);
    }
    else{
        printf("please key in txt file name\n");
        return 0;
    }
    e=setup(NodeAddress);
    while(1){
        loop();
    }
    return (0);
}
