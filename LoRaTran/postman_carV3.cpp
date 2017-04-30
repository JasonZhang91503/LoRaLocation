/*
#                       _oo0oo_
#                      o8888888o
#                      88" . "88
#                      (| -_- |)
#                      0\  =  /0
#                    ___/`---'\___
#                  .' \\|     |# '.
#                 / \\|||  :  |||# \
#                / _||||| -:- |||||- \
#               |   | \\\  -  #/ |   |
#               | \_|  ''\---/''  |_/ |
#               \  .-\__  '-'  ___/-. /
#             ___'. .'  /--.--\  `. .'___
#          ."" '<  `.___\_<|>_/___.' >' "".
#         | | :  `- \`.;`\ _ /`;.`/ - ` : | |
#         \  \ `_.   \_ __\ /__ _/   .-` /  /
#     =====`-.____`.___ \_____/___.-`___.-'=====
#                       `=---='
#
#
#     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#
#               佛祖保佑         永无BUG
#
#
#
*/


//#define NO_CAR_MODE

/*

postcar定義的error code皆為9487為開頭以區分error code來源

9487 + 以下error code查表:

000:車子狀態錯誤(需為0)
001:車子狀態錯誤(需為1)
002:車子狀態錯誤(需為2)
003:車子狀態錯誤(需為3)
004:車子狀態錯誤(需為4)


487:代表沒有問題太棒了

*/
#define CAR_STATE_999_ERROR 9487999
#define CAR_STATE_0_ERROR 9487000
#define CAR_STATE_1_ERROR 9487001
#define CAR_STATE_2_ERROR 9487002
#define CAR_STATE_3_ERROR 9487003
#define CAR_STATE_4_ERROR 9487004
#define CAR_OK 9487487


#ifndef NO_CAR_MODE
//Include arduPi library
#include "arduPi.h"
#include "arduPiLoRa.h"
#include "arduPiClasses.h"
#include "Lora_Setup.h"
#endif

//Include Navigation library
#include "Navigation.h"

#ifndef NO_CAR_MODE
//Include gps library
#include <gps.h>
#endif

//std function
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <math.h>
#include <iostream>
#include <pthread.h>

//class library
#include"postman_request.h"
#include"postman_packet.h"
//#include"postman_GPS.h"

#ifndef NO_CAR_MODE
#include <string>
#else
#include <cstring>
#endif

using namespace std;

int e;
char recv_packet[100];	//車子接收資料的buffer
char send_packet[100];	//車子送出資料的buffer
int rc; //GPS的return code
//typedef struct gps_data_t GPS_Data;	//GPS的端口
#ifndef NO_CAR_MODE
struct gps_data_t myGPS_Data;	//GPS的端口
#endif
//GPS_Data gps_data;
int pw_size = 4;
double dest_range = 0.02;
int LoRa_address = 1;
int carStatus = 0;	//carStatus代表車子本身狀態，0 = 停止, 1 = 暫停 , 2 = 行進中
int carID = 0;



RequestManager ReqManger;

bool isCarReachDestination(double &directionInfo, double &distanceInfo, double reachDistance, double sourceLon, double sourceLat, double destinationLon, double destinationLat);
UserRequest* waitRequest(RequestObserver *reqObserver);
int recvSenderRequest(UserRequest*);
int moveToSender(UserRequest*);
int beginTransport(UserRequest*);
int moveToReceiver(UserRequest*);
int endTransport(UserRequest*);
int checkState(int tarState);
int getGPSLocation(double &sLon,double &sLat);
int sendPacket(UserRequest*);
int parseRequestData(UserRequest*);
int parseStateData();
string parsePassword();

void pauseCar(){
	if(carStatus == 0) return;
	carStatus = 1;
}
void restartCar(){
	if(carStatus == 0) return;
	carStatus = 2;
}
void stopCar(){
	carStatus = 0;
}

#ifndef NO_CAR_MODE
int GPSsetup(){
	if ((rc = gps_open("localhost", "2947", &myGPS_Data)) == -1) {
        printf("code: %d, reason: %s\n", rc, gps_errstr(rc));
        return EXIT_FAILURE;
    }
	printf("GPSsetup : success!\n");
    gps_stream(&myGPS_Data, WATCH_ENABLE | WATCH_JSON, NULL);
	
	return 0;
}
#endif

void* asyncRecv(void *arg){
	UserRequest *req;
	int result;

	PacketManager *PacManager = PacketManager::getInstance();

	if(PacManager == NULL){
		cout << "asyncRecv : PacketManager instance is NULL\n";
		cout << "asyncRecv : thread close";
		pthread_exit(NULL);
	}
	
	cout << "asyncRecv : thread success\n";
	
	PacManager->initTimer();	//為了讓Timer可以被找到
	while(true){


#ifndef NO_CAR_MODE

		
		do{
			if(!PacManager->isTimerAlive()){
				if(PacManager->hasPacket()){
					cout << "asyncRecv : Send packet which in queue, meaning that packet will be re-sended\n";
					PacManager->sendQueuePacket();
				}
			}
		}while(PacManager->recvData());

		//unistd::sleep(1);
		//PacManager->sendBackACK();
		
		cout << "asyncRecv : recvive data > " <<  PacManager->recv_buffer+4 << endl;
//continue;
		/*
		*	規則：Packet 代表 Socket 所傳送的封包，用 Index[] 表示封包中資料的位置
		*	Index[0] : 類別碼, 1=gateway,2=車子
		*	Index[1] : 車子ID, 車子所代表ID
		*	Index[2] : packetNum, 以1與2表示, 也分別對應ACK1與ACK2
		*	Index[3] : 事件識別碼, 1 = 為ACK事件,2以後為其他事件定義
		*	Index[4] : 事件資料, 事件所傳送的資料
		*
		*/
		
		//若收到gateway的資料則捨棄
		if(PacManager->isGatewayPacket()){
			cout << "asyncRecv : recvive gateway data, discard it\n";
			continue;
		}

		//若收到不正確ID則丟棄
		if(!PacManager->isMyCarIDPacket()){
			cout << "asyncRecv : recvive another car's data, discard it\n";
			continue;
		}

		if(PacManager->isACK()){
			//isCorrectACK
			if(!PacManager->isCorrectACK()){
				cout << "asyncRecv : recvive incorrect ACK, discard it\n";
				//cout << "asyncRecv : resend last sended packet\n";
			}
			else{
				if(PacManager->isTimerAlive()){
					PacManager->stopTimer();
					PacManager->switchRecvACK();
					cout << "asyncRecv : recvive correct ACK\n";
				}
				else{
					cout << "asyncRecv : timer correct ACK\n";
				}
			}
			continue;
		}
		else{
			if(!PacManager->isCorrectPackNum()){
				cout << "asyncRecv : recvive incorrect packNum, discard it\n";
				result = PacManager->sendBackACK();
				//cout << "asyncRecv : resend last sended packet\n";
			}
			else{
				cout << "asyncRecv : recvive data > " <<  PacManager->recv_buffer+4 << endl;
				//判斷封包的識別碼
				switch(PacManager->getEventCode()){
					case 1:
					
						break;
					//case 2 = Gateway更改State(給車子訂單)，Index[4]後為訂單id、state、寄件經度、寄件緯度、收件經度、收件緯度、密碼，各資料間用 ',' 隔開
					case 2:
						cout << "asyncRecv : recvive UserRequest, begin parse"<< endl;
						
						req = new UserRequest;
						parseRequestData(req);
						ReqManger.add(req);
						break;
					//case 3 = Gateway傳遞密碼，Index[2]後為密碼
					case 3:
						cout << "asyncRecv : recvive data > " <<  recv_packet << endl;

						//需要決定丟到哪個訂單上面
						parsePassword();
						break;
					//case 4 = 暫停車子送件
					case 4:
						cout << "asyncRecv : recvive data > " <<  recv_packet << endl;

						pauseCar();
						break;
					//case 5 = 開始車子送件
					case 5:
						cout << "asyncRecv : recvive data > " <<  recv_packet << endl;

						restartCar();
						break;
					//case 6 = 終止車子送件
					case 6:
						cout << "asyncRecv : recvive data > " <<  recv_packet << endl;

						stopCar();
						break;
				}

				
				PacManager->switch_recvWaitingPacNum();
				PacManager->switchSendACK();
				result = PacManager->sendBackACK();
			}
		}


		
		

#else
		cin.get();
		UserRequest *req = new UserRequest;
		ReqManger.add(req);
		cin.get();
		while(true){
			
		}
#endif
	}
	
}



int main(int argc, const char * argv[]){
	int error;	//

	UserRequest *req;

	#ifndef NO_CAR_MODE
	//有車子則確認天線與LoRa狀態

	//LoRa天線的設置
	error = setup(LoRa_address);//給入值為LoRa環境Address
	if(error != 0){
		cout << "LoRa error occur" << endl;
		return error;
	}
	
	//GPS感測器的設置
	error = GPSsetup();
	if(error != 0){
		cout << "GPSSetup error occur" << endl;
		return error;
	}
	#endif

	//car程式開始執行
	
	/*
	功能
	state0. gateway主動打來用戶請求 -  state 被Gateway設為 0 ，並且前往寄件地點。
	state1. 抵達指定地點 - 將 state 的狀態設為 1 ，並且等待寄件者放入文件。
	state2. 寄件者放入信件並在板子上按 Enter - 將 state 的狀態設為 2 並向收件地點前進。
	state3. 到達寄件地點， - 將 state 設為 3 ，並且接收到Gateway傳送來的密碼後，等待使用者來領貨。
	state4. 使用者輸入密碼並取件 - 將 state 設為  4 。
	註:只有state 0 是 gateway主動打過來的
		剩下的state都是車子本身修改自身state並且傳送出去給gateway和server知道
	*/
	
	RequestObserver *reqObserver = new RequestObserver(1);
	PacketManager *pac = PacketManager::getInstance();
	ReqManger.addReqestListener(reqObserver);

	pthread_mutex_init(&timerMutex, NULL);
	pthread_cond_init(&timerCond, NULL);

	//建造一條用作recv的thread
	pthread_t recvThread;
	pthread_create(&recvThread,NULL,asyncRecv,NULL);

	//開始送貨循環
	while(1){
		cout << "Begin transport\n";
		
		req = waitRequest(reqObserver);

		//state -1->0 = idle - >接收到經緯度,開始到sender地點
		e = recvSenderRequest(req);
		if(e != CAR_OK){
			cout << "main : recvSenderRequest method error, code = " << e << endl;
		}
		
		//state 0->1 = 行走->到達sender指定地點
		e = moveToSender(req);
		if(e != CAR_OK){
			cout << "main : moveToSender method error, code = " << e << endl;
		}
		
		//state 1->2 = 抵達->sender放入文件，開始前往recv點
		e = beginTransport(req);
		if(e != CAR_OK){
			cout << "main : beginTransport method error, code = " << e << endl;
		}
		
		//state 2->3 = 抵達->告知抵達，並且接收packetKey
		e = moveToReceiver(req);
		if(e != CAR_OK){
			cout << "main : moveToReceiver method error, code = " << e << endl;
		}
		
		//state 3->4 = 等待領貨->輸入密碼成功，取貨完畢
		e = endTransport(req);
		if(e != CAR_OK){
			cout << "main : endTransport method error, code = " << e << endl;
		}	
	}

	return 0;
}

UserRequest* waitRequest(RequestObserver *reqObserver){
	cout<<"waitRequest : Wait for SenderRequest..."<<endl;
	while(!reqObserver->HasRequest()){
		
	}
	return reqObserver->RequestPop();
}


int recvSenderRequest(UserRequest* req){
	int StateCode;

	//剖析state 與 經緯度
	int rState = parseRequestData(req);
	if(rState != 0){
		cout << "recvSenderRequest : recv state data error! tarState = " << 0 << ",Recv_state = " << rState << endl;
		return CAR_STATE_0_ERROR;
	}
	
	cout << "recvSenderRequest : recv source longitude = " << req->src_lon << ", latitude = " << req->src_lat << endl;
	cout << "recvSenderRequest : recv destnation longitude = " << req->dest_lon << ", latitude = " << req->dest_lat << endl;
	cout << "recvSenderRequest : recv state = " << req->state << endl;
	
	return CAR_OK;
}

int moveToSender(UserRequest* req){
	double directionInfo, distanceInfo;	//方位與距離之回傳
	double reachDistance = dest_range;	//判定多少距離內算到達(單位公里)
	double sLon, sLat;	//起始地點
	bool isCarReach;	//車子是否到達
	int StateCode;
	
	cout << "moveToSender : Begin go to sender location" << endl;

	do {
		//取得車子本身GPS座標
		#ifndef NO_CAR_MODE
		getGPSLocation(sLon,sLat);
		#else
		sLon = req->dest_lon;
		sLat = req->dest_lat;
		#endif
		
		//去計算是否到達目標
		isCarReach = isCarReachDestination(directionInfo, distanceInfo, reachDistance, sLon, sLat, req->dest_lon, req->dest_lat);
		
		if (isCarReach) {
			cout << "moveToSender : reach destnation!" << endl;
			break;
		}
		
		cout << "You should still go toward "<< directionInfo << " degree for " << distanceInfo << " kilometer." << endl;
		#ifndef NO_CAR_MODE
		unistd::usleep(1000);
		#endif
	} while ( -180 < sLon && sLon < 180);
	
	req->state = 1;
	
#ifndef NO_CAR_MODE
	pac->sendState(req->state);
#endif
	
	return CAR_OK;
}

int beginTransport(UserRequest* req){
	
	cout << "beginTransport : Wait for sender place the file" << endl;
	
	//改成偵測是否recv到收到寄件?
	
	//判定使用者放入文件，目前使用enter做為判定
	getchar();
	
	cout << "beginTransport : Sender placed file" << endl;
	cout << "beginTransport : goto longitude = " << req->dest_lon << ", latitude = " << req->dest_lat << endl;
	
	req->state = 2;

#ifndef NO_CAR_MODE
	pac->sendState(req->state);
#endif

	return CAR_OK;
}

int moveToReceiver(UserRequest* req){
	double directionInfo, distanceInfo;	//方位與距離之回傳
	double reachDistance = dest_range;	//判定多少距離內算到達(單位公里)
	double sLon, sLat;	//起始地點
	bool isCarReach;	//車子是否到達
	int StateCode;
	
	do {
		//取得車子本身GPS座標
		#ifndef NO_CAR_MODE
		getGPSLocation(sLon,sLat);
		#else
		sLon = req->dest_lon;
		sLat = req->dest_lat;
		#endif
		
		//去計算是否到達目標
		isCarReach = isCarReachDestination(directionInfo, distanceInfo, reachDistance, sLon, sLat, req->dest_lon, req->dest_lat);
		
		if (isCarReach) {
			cout << "moveToReceiver : reach destnation!" << endl;
			break;
		}
		
		cout << "You should still go toward "<< directionInfo << " degree for " << distanceInfo << " kilometer." << endl;
		#ifndef NO_CAR_MODE
		unistd::usleep(1000);
		#endif
	} while ( -180 < sLon && sLon < 180);
	

	req->state = 3;
	
#ifndef NO_CAR_MODE
	pac->sendState(req->state);
#endif
	
	return CAR_OK;
}

int endTransport(UserRequest* req){
	string input;
	bool isCorrect = false;
	int StateCode;

	//等packetKey
	//!!!以後可能直接用Request直接有
	req->packetKey = parsePassword();
	
	cout << "endTransport : Wait for password input" << endl << endl;
	do{
		cout << "Password :";
		getline(cin,input,'\n');
		
		if(input != req->packetKey){
			cout << "Error! Please enter password again" << endl;
		}
		else{isCorrect = true;}
	}while(!isCorrect);
	
	cout << "Password correct!" << endl;
	cout << "Press enter to end transport!" << endl;
	
	/*
		密碼箱打開後
		要判定東西被拿走
		這裡用按下enter來當作拿了
		~~~~以後要改成RFID
	*/
	getchar();
	
	req->state = 4;
	
	#ifndef NO_CAR_MODE
	pac->sendState(req->state);
	#endif
	
	return CAR_OK;
}

#ifndef NO_CAR_MODE
int getGPSLocation(double &sLon,double &sLat){
	while (1) {
        /* wait for 2 seconds to receive data */
        if (gps_waiting (&myGPS_Data, 2000000)) {
            /* read data */
            if ((rc = gps_read(&myGPS_Data)) == -1) {
                printf("error occured reading gps data. code: %d, reason: %s\n", rc, gps_errstr(rc));
            } else {
                /* Display data from the GPS receiver. */
                if ((myGPS_Data.status == STATUS_FIX) && 
                    (myGPS_Data.fix.mode == MODE_2D || myGPS_Data.fix.mode == MODE_3D) &&
                    !::isnan(myGPS_Data.fix.latitude) && 
                    !::isnan(myGPS_Data.fix.longitude)) {
                        //gettimeofday(&tv, NULL); EDIT: tv.tv_sec isn't actually the timestamp!
                        printf("latitude: %f, longitude: %f, speed: %f, timestamp: %ld\n", myGPS_Data.fix.latitude, myGPS_Data.fix.longitude, myGPS_Data.fix.speed, myGPS_Data.fix.time); //EDIT: Replaced tv.tv_sec with gps_data.fix.time
						break;
               } else {
                    printf("no GPS data available\n");
                }
            }
        }
		else{
			printf("wait for 2 seconds to receive data again\n");
		}

        unistd::sleep(3);
    }
	
	sLon = myGPS_Data.fix.longitude;
	sLat = myGPS_Data.fix.latitude;
	
	return CAR_OK;
}
#endif

#ifndef NO_CAR_MODE
int sendPacket(UserRequest *req){
	sprintf(send_packet,"%d",req->state);
    e = sx1272.sendPacketTimeout(0, send_packet);
    printf("Packet sent, state :%d, code :%d\n",req->state,e);
}
#endif

int parseRequestData(UserRequest* req){
	const char *d = " ,";
	char* statePtr;
	char* sLonPtr;
	char* sLatPtr;
	char* dLonPtr;
	char* dLatPtr;
	char* packetKeyPtr;
	
	#ifndef NO_CAR_MODE

	/*
	statePtr = strtok(recv_packet,d);	//state
	printf("split state :%s\n",statePtr);
	sLonPtr = strtok(NULL,d);
	printf("split sLon :%s\n",sLonPtr);
	sLatPtr = strtok(NULL,d);
	printf("split sLat :%s\n",sLatPtr);
	dLonPtr = strtok(NULL,d);
	printf("split dLon :%s\n",dLonPtr);
	dLatPtr = strtok(NULL,d);
	printf("split dLat :%s\n",dLatPtr);
	
	req->state = atoi(statePtr);
	printf("rState :%d\n",req->state);
	req->src_lon = atof(sLonPtr);
	printf("sLon :%f\n",req->src_lon);
	req->src_lat = atof(sLatPtr);
	printf("sLat :%f\n",req->src_lat);
	req->dest_lon = atof(dLonPtr);
	printf("dLon :%f\n",req->dest_lon);
	req->dest_lat = atof(dLatPtr);
	printf("dLat :%f\n",req->dest_lat);
	*/

	PacketManager *PacManager = PacketManager::getInstance();

	//有識別碼版
	statePtr = strtok( PacManager->(recv_buffer + 4) ,d);	//index[2]後開始為資料
	printf("split state :%s\n",statePtr);
	sLonPtr = strtok(NULL,d);
	printf("split sLon :%s\n",sLonPtr);
	sLatPtr = strtok(NULL,d);
	printf("split sLat :%s\n",sLatPtr);
	dLonPtr = strtok(NULL,d);
	printf("split dLon :%s\n",dLonPtr);
	dLatPtr = strtok(NULL,d);
	printf("split dLat :%s\n",dLatPtr);
	packetKeyPtr = strtok(NULL,d);
	printf("split packetKey :%s\n",packetKeyPtr);
	
	req->state = atoi(statePtr);
	printf("rState :%d\n",req->state);
	req->src_lon = atof(sLonPtr);
	printf("sLon :%f\n",req->src_lon);
	req->src_lat = atof(sLatPtr);
	printf("sLat :%f\n",req->src_lat);
	req->dest_lon = atof(dLonPtr);
	printf("dLon :%f\n",req->dest_lon);
	req->dest_lat = atof(dLatPtr);
	printf("dLat :%f\n",req->dest_lat);
	req->packetKey.assign(packetKeyPtr);
	printf("packetKey :%s\n",req->packetKey.c_str());
	#else
	req->state = 0;
	printf("rState :%d\n",req->state);
	req->src_lon = 121.369862;
	printf("sLon :%f\n",req->src_lon);
	req->src_lat = 24.944185;
	printf("sLat :%f\n",req->src_lat);
	req->dest_lon = 121.369862;
	printf("dLon :%f\n",req->dest_lon);
	req->dest_lat = 24.944185;
	printf("dLat :%f\n",req->dest_lat);
	req->dest_lat = 24.944185;
	printf("dLat :%f\n",req->dest_lat);
	char* pw = new char[pw_size];
	pw[0] = '\0';
	req->packetKey.assign(pw);
	printf("packetKey :%s\n",req->packetKey.c_str());
	#endif
	
	return req->state;
}

int parseStateData(){
	int rState = atoi(recv_packet);
	printf("parseStateData: rState :%d\n",rState);
	
	return rState;
}




//無識別碼版
string parsePassword(){
	char* pw = new char[pw_size];
	char* p;
	int i = 0;
	for(p = recv_packet,i = 0; i < pw_size;i++){
		pw[i] = p[i];
	}
	pw[i] = '\0';
	string pwStr(pw);
	return pwStr;
}


/*
//有識別碼版
string parsePassword(){
	char* pw = new char[pw_size];
	char* p;
	int i = 0;

	//index[1...4]為密碼部分
	for(p = recv_packet,i = 1; i <= pw_size;i++){
		pw[i] = p[i];
	}
	pw[i] = '\0';
	string pwStr(pw);
	return pwStr;
}
*/