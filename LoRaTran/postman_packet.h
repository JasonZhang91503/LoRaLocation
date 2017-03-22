#include <string.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

#ifndef NO_CAR_MODE
//Include arduPi library
#include "arduPi.h"
#include "arduPiLoRa.h"
#include "arduPiClasses.h"
#include "Lora_Setup.h"
#endif

#define BUFFSIZE 256

enum PacketType{ Gateway = 1, car, ACK0, ACK1 };

class PacketManager{
public:
    PacketManager(int CarID){
        currentSendACK = 0;
        currentRecvACK = 0;
        errorCode = 0;
        receiveTime = 10000;
        carID = CarID;
        clearBuffer();
    }

    bool isGatewayPacket(){
        return recv_buffer[0] == Gateway;
    }

    int recvData(){
        errorCode = sx1272.receivePacketTimeout(receiveTime);
	
		if(errorCode != 0){
			return errorCode;
		}
		
		for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
		{
			recv_packet[i] = (char)sx1272.packet_received.data[i];
		}

        return 0;
    }

    bool isACK(){
        return ( recv_buffer[0] == ACK0 ) || ( recv_buffer[0] == ACK1 );
    }

    bool isCorrectACK(){
        int rACK;

        if(recv_buffer[0] == ACK0) { rACK = 0; }
        else if(recv_buffer[0] == ACK1){ rACK = 1; }
        else { return false; }

        return rACK == currentRecvACK;
    }

    int sendState(int state){ 
        sprintf(send_buffer,"%d%d%d%d%d\0",1,carID,currentSendACK,1,state);
        #ifndef NO_CAR_MODE
        errorCode = sx1272.sendPacketTimeout(0, send_buffer);
        #endif
        return errorCode;
    }

    int sendError(){

    }

    bool sendBackACK(){
        int BackACK;
        if(currentRecvACK == 0){ BackACK = ACK0; }
        else { BackACK = ACK1; }
        sprintf(send_buffer,"%d%d",BackACK,carID);
        #ifndef NO_CAR_MODE
        errorCode = sx1272.sendPacketTimeout(0, send_buffer);
        #endif
    }



    void switchSendACK(){
        if(currentSendACK == 0) currentSendACK = 1;
        else currentSendACK = 0;
    }

    void switchRecvACK(){
        if(currentRecvACK == 0) currentRecvACK = 1;
        else currentRecvACK = 0;
    }



    void clearBuffer(){
        clearRecvBuffer();
        clearSendBuffer();
    }

    void clearRecvBuffer(){
        memset(recv_buffer,0,sizeof(recv_buffer));
    }
    void clearSendBuffer(){
        memset(send_buffer,0,sizeof(send_buffer));
    }

    void setTimer(){
        pthread_create(&timerThread,NULL,asyncRecv,NULL);
    }


    char recv_buffer[BUFFSIZE];	//車子接收資料的buffer
    char send_buffer[BUFFSIZE];	//車子送出資料的buffer
private:
    void asyncTimer();
    int currentSendACK;
    int currentRecvACK;
    int errorCode;
    int carID;
    int receiveTime;
    clock_t timer;
    pthread_t timerThread;
};

void PacketManager::asyncTimer(){
    timer = clock();
}