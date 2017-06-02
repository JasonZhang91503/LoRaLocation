#include <string.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <queue>
#include <errno.h>
#include<sys/time.h>
#include<string>

#ifndef NO_CAR_MODE
//Include arduPi library
#include "arduPi.h"
#include "arduPiLoRa.h"
#include "arduPiClasses.h"
#include "Lora_Setup.h"
#endif

#define BUFFSIZE 256

int postman_packetLog = 0;

enum PacketType{ Gateway = 1, car, ACK0, ACK1 };

pthread_mutex_t timerMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t timerCond;

pthread_mutex_t queueMutex = PTHREAD_MUTEX_INITIALIZER;

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
    static PacketManager* getInstance(int period){
        if(!instance){
            instance = new PacketManager(1,period);
        }
        return instance;
    }
    

    bool isGatewayPacket(){
        return recv_buffer[0] == Gateway;
    }

    bool isMyCarIDPacket(){
        //printf("Aasdasdsadsa : %d\n",recv_buffer[1]);
        return recv_buffer[1] == carID;
    }

    int recvData(){

        #ifndef NO_CAR_MODE
        errorCode = sx1272.receivePacketTimeout(receiveTime);
        #endif

		if(errorCode != 0){
            //printf("asyncRecv : no data received, receive again\n");
			return errorCode;
		}
		
        #ifndef NO_CAR_MODE
		for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
		{
			recv_buffer[i] = (char)sx1272.packet_received.data[i];
		}

        int role = recv_buffer[0];
        int carID = recv_buffer[1];
        int packNum = recv_buffer[2];
        int eventNum = recv_buffer[3];
        if(postman_packetLog){printf("role = %d, carID = %d, packNum = %d, eventNum = %d\n",role,carID,packNum,eventNum);}

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

    int sendState(int id,int state){
        Packet *newPac = new Packet();

        string idStr = std::to_string(id);
        sprintf(newPac->send_buffer,"%c%c%c%c%s,%c,\0",1,carID,sendPacNum,2,idStr.c_str(),state + 48);

        enqueuePacket(newPac);

        switch_sendPacNum();
        /*
        if(isTimerAlive()){
            return 0;
        }
        else{
            sendQueuePacket();  
			setTimer();
        }
*/
/*
        #ifndef NO_CAR_MODE
        errorCode = sx1272.sendPacketTimeout(0, newPac->send_buffer);
        #endif
        */

        return 0;
    }

    int sendError(){

    }

    bool sendBackACK(){
        int BackACK;
        /*
        if(currentRecvACK == 1){ BackACK = ACK0; }
        else { BackACK = 2; }
        */
        sprintf(send_buffer,"%c%c%c%c",1,carID,currentSendACK,1);
        int role = send_buffer[0];
        int carID = send_buffer[1];
        int packNum = send_buffer[2];
        int eventNum = send_buffer[3];
        if(postman_packetLog){printf("PacManager : sendBackACK -> role = %d, carID = %d, packNum = %d, eventNum = %d\n",role,carID,packNum,eventNum);}
        
        #ifndef NO_CAR_MODE
        printf("CACSACASCASC\n");
        if(postman_packetLog){printf("PacManager : sendBackACK -> role = %d, carID = %d, packNum = %d, eventNum = %d\n",role,carID,packNum,eventNum);}
        errorCode = sx1272.sendPacketTimeout(0, send_buffer);
        printf("1231231232123\n");
        #endif
    }

    void enqueuePacket(Packet* pac){
        pthread_mutex_lock(&queueMutex);
        sendPacQueue.push(pac);
        pthread_mutex_unlock(&queueMutex);
    }

    void dequeuePacket(){
        pthread_mutex_lock(&queueMutex);
        sendPacQueue.pop();
        pthread_mutex_unlock(&queueMutex);
    }

    bool hasPacket(){
        bool rv;
        pthread_mutex_lock(&queueMutex);
        rv = !sendPacQueue.empty();
        pthread_mutex_unlock(&queueMutex);
        return rv;
    }

    int packetNum(){
        int rv;
        pthread_mutex_lock(&queueMutex);
        rv = sendPacQueue.size();
        pthread_mutex_unlock(&queueMutex);
        return rv;
    }

    int sendQueuePacket(){
        pthread_mutex_lock(&queueMutex);
        Packet* pac = sendPacQueue.front();
        pthread_mutex_unlock(&queueMutex);
        int role = pac->send_buffer[0];
        int carID = pac->send_buffer[1];
        int packNum = pac->send_buffer[2];
        int eventNum = pac->send_buffer[3];
        if(postman_packetLog){printf("PacManager : sendQueuePacket -> role = %d, carID = %d, packNum = %d, eventNum = %d, payload = %s\n",role,carID,packNum,eventNum,pac->send_buffer+4);}
        #ifndef NO_CAR_MODE
        errorCode = sx1272.sendPacketTimeout(0, pac->send_buffer);
        #endif
        return errorCode;
    }
    



    void switchSendACK(){
        if(currentSendACK == 1) currentSendACK = 2;
        else currentSendACK = 1;
    }

    void setCurrentSendACK(int SendACK){
        currentSendACK = SendACK;
    }

    void switchRecvACK(){
        if(currentRecvACK == 1) currentRecvACK = 2;
        else currentRecvACK = 1;
    }

    void switch_recvWaitingPacNum(){
        if(recvWaitingPacNum == 1) recvWaitingPacNum = 2;
        else recvWaitingPacNum = 1;
    }

    void switch_sendPacNum(){
        if(sendPacNum == 1) sendPacNum = 2;
        else sendPacNum = 1;
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
            if(postman_packetLog){cout << "PacManager : reject setTimer , timer is alive\n";}
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

    if(postman_packetLog){cout << "stopTimer : timerMutex lock\n";}
        //pthread_mutex_lock(&timerMutex);
        if(isTimerAlive()){
            pthread_cond_signal(&timerCond); 
        }
        //pthread_mutex_unlock(&timerMutex);

    if(postman_packetLog){cout << "stopTimer : timerMutex unlock\n";}
        
    }

    bool isTimerAlive(){
        int rv = pthread_kill(timerThread,0);
        if(rv == ESRCH){
            //printf("timer thread not exist\n");
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
    PacketManager(int CarID,int peroid){
        //currentSendACK = 0;
        //currentRecvACK = 0;
        currentSendACK = 2;
        currentRecvACK = 1;
        recvWaitingPacNum = 1;
        sendPacNum = 1;
        errorCode = 0;
        receiveTime = peroid; //1000、700、
        timeout = 10;
        carID = CarID;
        clearBuffer();
    }
    
    int currentSendACK;
    int currentRecvACK;
    int recvWaitingPacNum;
    int sendPacNum;
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

    PacketManager *pm = PacketManager::getInstance(0);
    int* timeout = (int*)param;


    if(postman_packetLog){cout << "asyncTimer : timerMutex lock\n";}
    pthread_mutex_lock(&timerMutex);
    gettimeofday(&now, NULL);
    outtime.tv_sec = now.tv_sec + *timeout;
    int result = pthread_cond_timedwait(&timerCond, &timerMutex, &outtime);
    if(result == 0){
        if(postman_packetLog){printf("asyncTimer : stopTimer!\n");}
        pm->dequeuePacket();
    }
    pthread_mutex_unlock(&timerMutex);

    if(postman_packetLog){cout << "asyncTimer : timerMutex unlock\n";}
    
    pthread_exit((void*)result);
}

PacketManager* PacketManager::instance = 0;