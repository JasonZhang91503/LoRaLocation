#include <string.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <queue>
#include <errno.h>
#include<sys/time.h>

#ifndef NO_CAR_MODE
//Include arduPi library
#include "arduPi.h"
#include "arduPiLoRa.h"
#include "arduPiClasses.h"
#include "Lora_Setup.h"
#endif

#define BUFFSIZE 256

enum PacketType{ Gateway = 1, car, ACK0, ACK1 };

pthread_mutex_t timerMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t timerCond;

pthread_mutex_t queueMutex;

void* asyncTimer(void* param);

class Packet{
public:
    Packet(){
        retry_times = 0;
    }

    char send_buffer[BUFFSIZE];	//車子送出資料的buffer
private:
    int retry_times;
};

class PacketManager{
public:
    static PacketManager* getInstance(){
        if(!instance){
            instance = new PacketManager(1);
        }
        return instance;
    }
    

    bool isGatewayPacket(){
        return recv_buffer[0] == Gateway;
    }

    bool isMyCarIDPacket(){
        return recv_buffer[1] == carID;
    }

    int recvData(){

        #ifndef NO_CAR_MODE
        errorCode = sx1272.receivePacketTimeout(receiveTime);
        #endif

		if(errorCode != 0){
            printf("asyncRecv : no data received, receive again\n");
			return errorCode;
		}
		
        #ifndef NO_CAR_MODE
		for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
		{
			recv_buffer[i] = (char)sx1272.packet_received.data[i];
		}
        #endif

        return 0;
    }

    bool isACK(){
        return getEventCode() == 1;
        //return ( recv_buffer[0] == ACK0 ) || ( recv_buffer[0] == ACK1 );
    }

    bool isCorrectACK(){
        int rACK;

/*
        if(recv_buffer[2] == 1) { rACK = 0; }
        else if(recv_buffer[2] == 2){ rACK = 1; }
        else { return false; }
*/

        return recv_buffer[2] == currentRecvACK;
    }

    bool isCorrectPackNum(){
        return recv_buffer[2] == recvWaitingPacNum;
    }

    int getEventCode(){
        return recv_buffer[3];
    }

    int sendState(int state){
        Packet *newPac = new Packet();
        sprintf(newPac->send_buffer,"%d%d%d%d%d\0",1,carID,currentRecvACK,2,state);


        enqueuePacket(newPac);
        #ifndef NO_CAR_MODE
        errorCode = sx1272.sendPacketTimeout(0, newPac->send_buffer);
        #endif
        return errorCode;
    }

    int sendError(){

    }

    bool sendBackACK(){
        int BackACK;
        /*
        if(currentRecvACK == 1){ BackACK = ACK0; }
        else { BackACK = 2; }
        */
        sprintf(send_buffer,"%d%d%d%d",1,carID,currentSendACK,1);
        #ifndef NO_CAR_MODE
        errorCode = sx1272.sendPacketTimeout(0, send_buffer);
        #endif
    }

    void enqueuePacket(Packet* pac){
        sendPacQueue.push(pac);
    }

    void dequeuePacket(){
        sendPacQueue.pop();
    }

    bool hasPacket(){
        return !sendPacQueue.empty();
    }

    int sendQueuePacket(){
        Packet* pac = sendPacQueue.front();
        cout << "PacManager : sendQueuePacket " << pac->send_buffer << endl;
        #ifndef NO_CAR_MODE
        errorCode = sx1272.sendPacketTimeout(0, pac->send_buffer);
        #endif
        return errorCode;
    }
    



    void switchSendACK(){
        if(currentSendACK == 1) currentSendACK = 2;
        else currentSendACK = 1;
    }

    void switchRecvACK(){
        if(currentRecvACK == 1) currentRecvACK = 2;
        else currentRecvACK = 1;
    }

    void switch_recvWaitingPacNum(){
        if(recvWaitingPacNum == 1) recvWaitingPacNum = 2;
        else recvWaitingPacNum = 1;
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
        if(isTimerAlive()){
            cout << "PacManager : reject setTimer , timer is alive\n";
            return;
        }
        pthread_create(&timerThread,NULL,asyncTimer,&timeout);
    }

    void initTimer(){
        int initTime = 1;
        pthread_create(&timerThread,NULL,asyncTimer,&initTime);
    }

    void setIsGetACK(bool ack){ isGetACK = ack; }
    bool getIsGetACK(){ return isGetACK; }

    bool stopTimer(){
        
        pthread_mutex_lock(&timerMutex);
        if(isTimerAlive()){
            pthread_cond_signal(&timerCond); 
        }
        pthread_mutex_unlock(&timerMutex);
        
    }

    bool isTimerAlive(){
        int rv = pthread_kill(timerThread,0);
        if(rv == ESRCH){
            printf("timer thread not exist\n");
            return false;
        }
            
        else if(rv == EINVAL){
            printf("pthread_kill signal is invalid/n");
            return false;
        }
        else{
            return true;
        }
    }

    char recv_buffer[BUFFSIZE];	//車子接收資料的buffer
    char send_buffer[BUFFSIZE];	//車子送出資料的buffer
private:
    static PacketManager *instance;
    PacketManager(int CarID){
        //currentSendACK = 0;
        //currentRecvACK = 0;
        currentSendACK = 2;
        currentRecvACK = 1;
        recvWaitingPacNum = 1;
        errorCode = 0;
        receiveTime = 1000;
        timeout = 5;
        carID = CarID;
        clearBuffer();
    }
    
    int currentSendACK;
    int currentRecvACK;
    int recvWaitingPacNum;
    int errorCode;
    int carID;
    int receiveTime;
    int timeout;
    bool isGetACK;
    pthread_t timerThread;
    std::queue<Packet*> sendPacQueue;
};

void* asyncTimer(void* param){
    struct timeval now;
    struct timespec outtime;

    PacketManager *pm = PacketManager::getInstance();
    int* timeout = (int*)param;

    pthread_mutex_lock(&timerMutex);
    gettimeofday(&now, NULL);
    outtime.tv_sec = now.tv_sec + *timeout;
    int result = pthread_cond_timedwait(&timerCond, &timerMutex, &outtime);
    if(result == 0){
        pm->dequeuePacket();
    }
    pthread_mutex_unlock(&timerMutex);

    
    pthread_exit((void*)result);
}

PacketManager* PacketManager::instance = 0;