#include<iostream>
using namespace std;

#include"postman_packet.h"

//#define NO_CAR_MODE

int main(){
    PacketManager *pac = PacketManager::getInstance();


    pac->sendState(0);

    //if(!pac->isTimerAlive()){
        if(pac->hasPacket()){
            cout << "asyncRecv : Send packet which in queue, meaning that packet will be re-sended\n";
            pac->sendQueuePacket();
            //pac->setTimer();
        }
   //}


    cout << "GOOD\n";

    cin.get();
}