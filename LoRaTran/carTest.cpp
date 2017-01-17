//
//  main.cpp
//  gateway
//
//  Created by Keng on 15/01/2017.
//  Copyright © 2017 Keng. All rights reserved.
//

#include <iostream>
#include <string.h>
#include "arduPiLoRa.h"
#include "Lora_Setup.h"

using namespace std;
int e,NodeAddress=1;

void send(string s,int e){
    char send_str[100];
    sprintf(send_str,"%s",s.c_str());
    e = sx1272.sendPacketTimeout(0,send_str);
    printf("Packet sent, state %d\n",e);
    
}
char* recvMessage() {
    // Receive message
    int e;
    char *messageStr;
    char lastPacketMessage[100];
    cout<<"Wait for sent..."<<endl;
    e = sx1272.receivePacketTimeout(10000);
    
    
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
        cout<<lastPacketMessage<<endl;
        return messageStr;
    }
    else {
        printf("Receive packet, state %d\n", e);
        return NULL;
    }
}
int main(int argc, const char * argv[]) {

    string data[3];
    string start_point[2];
    string des_point[2];
    char *recvMsg;
    string sendMsg;
    string key;
    e=setup(NodeAddress);
    
    sendMsg="0"+" "+"24.5"+","+"24.6"+" "+"24.7"+","+"24.8";
    cout<<"Send_Msg:"<<endl;
    cout<<sendMsg<<endl;
    send(sendMsg,e);//還要給經緯度
    

    return 0;
}
