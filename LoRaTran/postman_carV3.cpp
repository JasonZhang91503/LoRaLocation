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
#define CAR_NOT_FOUND_ROAD 9487005
#define GPS_NOT_IN_CARMAP 9487006
#define CAR_OK 9487487

#define MAP_WIDTH 135
#define MAP_HEIGHT 40
#define ROAD_WIDTH 5

#define SH_SIZE 20

#ifndef NO_CAR_MODE
//Include arduPi library
#include "arduPi.h"
#include "arduPiLoRa.h"
#include "arduPiClasses.h"
#include "Lora_Setup.h"
#endif


//Include Navigation library
#include "Navigation.h"

//Include CarMapSystem library
#include "CarMapSystem.h"

#include "FileTran.h"

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

#ifndef NO_CAR_MODE
using namespace unistd;
#endif

//class library
#include"postman_request.h"
#include"postman_packet.h"
#include"postman_consoleMap.h"
//#include"postman_websocket.h"

#ifndef NO_CAR_MODE
#include"postman_carControl.h"
#endif

#include"confTest.h"
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
Coor mapNode;
//GPS_Data gps_data;
Coor init,xMax,yMax;
int pw_size = 4;
double dest_range = 0.002;
int LoRa_address = 1;
int carStatus = 0;	//carStatus代表車子本身狀態，0 = 停止, 1 = 暫停 , 2 = 行進中
int carID = 0;
int NOGPS = 0;
int receivePeriod = 700;
int carLog = 0;
float** adj;
int LocCount;
int carOpen = 0;

int pipeFds[2];
int rpipeFds[2];
int pchild;

int carPipeFds[2];

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
void initCGMS();

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
	
    gps_stream(&myGPS_Data, WATCH_ENABLE | WATCH_JSON, NULL);
	

	double testLon,testLat;

	if(NOGPS != 2){
		getGPSLocation(testLon,testLat);
	}
	
	printf("GPSsetup : success!\n");

	return 0;
}
#endif

void* asyncRecv(void *arg){
	UserRequest *req;
	int result;

	PacketManager *PacManager = PacketManager::getInstance(receivePeriod);

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
					if(carLog){cout << "asyncRecv : Send packet which in queue, meaning that packet will be re-sended\n";}
					PacManager->sendQueuePacket();
					PacManager->setTimer();
				}
			}
		}while(PacManager->recvData());

		cout << "asyncRecv : recvive data > " <<  PacManager->recv_buffer+4 << endl;
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
				//result = PacManager->sendBackACK();
				cout << "asyncRecv : resend last sended packet\n";
			}
			else{
				cout << "asyncRecv : recvive data > " <<  PacManager->recv_buffer+4 << endl;
				//判斷封包的識別碼
//0
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
		cin.get();
		UserRequest *req = new UserRequest;
		req->state = 0;
		req->src_lon = 121.369862;
		req->src_lat = 24.944185;
		req->dest_lon = 121.369862;
		req->dest_lat = 24.944185;
		req->dest_lat = 24.944185;
		char* pw = new char[pw_size];
		pw[0] = '\0';
		req->packetKey.assign(pw);
		ReqManger.add(req);
		cin.get();
		while(true){
			
		}
#endif
	}
	
}


#ifndef NO_CAR_MODE
void buildWebSocket(){
	if(unistd::pipe(pipeFds)){
		perror("Pipe build failed\n");
		exit(1);
	}
	if(unistd::pipe(rpipeFds)){
		perror("readPipe build failed\n");
		exit(1);
	}
	
	char argPipe1[10];
	char argPipe2[10];
	char read_argPipe1[10];
	char read_argPipe2[10];

	sprintf(argPipe1,"%d",pipeFds[0]);
	sprintf(argPipe2,"%d",pipeFds[1]);
	sprintf(read_argPipe1,"%d",rpipeFds[0]);
	sprintf(read_argPipe2,"%d",rpipeFds[1]);

	pchild = unistd::fork();

	if(pchild < 0){
		perror("Fork error\n");
		exit(1);
	}
	else if(pchild == 0){
		unistd::execl("/usr/bin/xterm","xterm","-e","LoRaLocation/LoRaTran/websocketServer_exe",argPipe1,argPipe2,read_argPipe1,read_argPipe2,NULL);
	}
	unistd::close(pipeFds[0]);
	unistd::close(rpipeFds[1]);

}

void buildCarControl(){
	if(unistd::pipe(carPipeFds)){
		perror("Pipe build failed\n");
		exit(1);
	}

	//unistd::close(carPipeFds[0]);
}




#endif


void* asyncCarControl(void* prarm){

	cout <<"asdkasdkasfsakfjjskfjask" << endl;
	char readBuff[256];
	read(carPipeFds[0],readBuff,sizeof(readBuff));

	cout << "HAHHHHHHHHHHHHHHHHHHHHHHHA" << endl;
}

int main(int argc, const char * argv[]){
	int error;	//

	cout << "Has GPS? (1 Yes, 2No):";
	cin >> NOGPS;

	cout << "Receive period?:";
	cin >> receivePeriod;

	char fileName[256];
	cout << "File name?:";
	cin >> fileName;
	fileOpen(fileName);

	cout << "Has Log?(1 Yes, 0No):";
	cin >> carLog;

	CarMapSystemLog = carLog;
	postman_packetLog = carLog;


	int rrc;

	cout << "ReadRequestConf?(1 Yes, 0No):";
	cin >> rrc;

	cout << "CarOpen?(1 yes,0No):";
	cin >> carOpen;

	cin.get();

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

	#ifndef NO_CAR_MODE
	buildWebSocket();
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

	LocCount = 1;

    init.x = 121.370842;
    init.y = 24.943536;
    xMax.x = 121.373122;
    xMax.y = 24.944833;
    yMax.x = 121.370393;
    yMax.y = 24.944141;
	
	RequestObserver *reqObserver = new RequestObserver(1);
	ReqManger.addReqestListener(reqObserver);
	
	PacketManager *pac = PacketManager::getInstance(receivePeriod);
	
	CarGpsMapSystem* cgms = CarGpsMapSystem::getInstance(MAP_WIDTH,MAP_HEIGHT,init,xMax,yMax);
	//cgms->setRectangleWall( 0 + ROAD_WIDTH, 0 + ROAD_WIDTH,MAP_WIDTH/2 - ROAD_WIDTH ,MAP_HEIGHT - ROAD_WIDTH);
    //cgms->setRectangleWall(MAP_WIDTH/2 + ROAD_WIDTH, 0 + ROAD_WIDTH,MAP_WIDTH - ROAD_WIDTH,MAP_HEIGHT - ROAD_WIDTH);
	cgms->setRectangleWall( 0 + ROAD_WIDTH, 0 + ROAD_WIDTH,(MAP_WIDTH - ROAD_WIDTH)/2,MAP_HEIGHT - ROAD_WIDTH);
	cgms->setRectangleWall((MAP_WIDTH +  ROAD_WIDTH)/2 + 1, 0 + ROAD_WIDTH,MAP_WIDTH-1,MAP_HEIGHT - ROAD_WIDTH);
	initCGMS();
	


//--------------------------------
/*
	double directionInfo, distanceInfo;	//方位與距離之回傳
	double reachDistance = dest_range;	//判定多少距離內算到達(單位 : 格數)
	double sLon, sLat;	//起始地點
	double dLon, dLat;
	bool isCarReach;	//車子是否到達
	int StateCode;
	Coor ss,ee;
	bool firstFind = true;
	vec_CMnode traceVec;
	vec_CMnode::iterator traIt;
	vec_CMnode::iterator printIt;
	int count = 0;

	isCarReach = isCarReachDestination(directionInfo, distanceInfo, reachDistance,sLon,sLat,dLon,dLat);

	char buff[256];
	sprintf(buff,"go toward %lf degree for %lf meter.\n",directionInfo,distanceInfo*1000);
	printf(buff);

	*/

	double gpsData[SH_SIZE]={
        24.943584,121.370883,  //1.圖書公院轉角
        24.943888,121.370672,  //2.圖書館
		24.944148,121.370493,  //3.圖書行政轉角
        24.943946,121.371511,  //4.公院
		24.944436,121.371015,  //5.行政
        24.944188,121.371931,  //6.法商大道法院測
        24.944480,121.371801,  //7.法商大道
        24.944757,121.371559,  //8.法商大道商院測
        24.944498,121.372605,   //9.法院
        24.945091,121.372150  //10.商院
        //24.945390,121.372683,  //10.正門商院轉角
       // 24.945161,121.372851,  //11.正門
        //24.944835,121.373112,  //12.正門法院
    };

	for(int i = 0; i < SH_SIZE/2;i++){
		Coor gps = { gpsData[i*2+1],gpsData[i*2] };
		Coor coor = cgms->gpsToCoordinate(gps);

		printf("Test data : %d,%d\n",(int)coor.x,(int)coor.y);
	}
	

//---------------------------------

	pthread_mutex_init(&timerMutex, NULL);
	pthread_cond_init(&timerCond, NULL);

	//建造一條用作recv的thread
	pthread_t recvThread,carControlThread;
	pthread_create(&recvThread,NULL,asyncRecv,NULL);
	if(carOpen == 1){
		pthread_create(&carControlThread,NULL,asyncCarControl,NULL);
	}


	//pthread_create(&webSocketThread,NULL,asyncWebSocketServer,NULL);

	//開始送貨循環
	while(1){
		cout << "Begin transport\n";

		if(rrc){
			req = new UserRequest;
			readRequestConfig(req);
			ReqManger.add(req);
		}

		req = waitRequest(reqObserver);


		//state -1->0 = idle - >接收到經緯度,開始到sender地點
		e = recvSenderRequest(req);
		if(e != CAR_OK){
			cout << "main : recvSenderRequest method error, code = " << e << endl;
			exit(1);
		}

		//state 0->1 = 行走->到達sender指定地點
		e = moveToSender(req);
		if(e != CAR_OK){
			cout << "main : moveToSender method error, code = " << e << endl;
			exit(1);
		}
	
		//state 1->2 = 抵達->sender放入文件，開始前往recv點
		e = beginTransport(req);
		if(e != CAR_OK){
			cout << "main : beginTransport method error, code = " << e << endl;
			exit(1);
		}
		
		//state 2->3 = 抵達->告知抵達，並且接收packetKey
		e = moveToReceiver(req);
		if(e != CAR_OK){
			cout << "main : moveToReceiver method error, code = " << e << endl;
			exit(1);
		}
		
		//state 3->4 = 等待領貨->輸入密碼成功，取貨完畢
		e = endTransport(req);
		if(e != CAR_OK){
			cout << "main : endTransport method error, code = " << e << endl;
			exit(1);
		}	
	}

	return 0;
}

int goToLocation(double lon,double lat){
	double directionInfo, distanceInfo;	//方位與距離之回傳
	double reachDistance = dest_range;	//判定多少距離內算到達(單位 : 格數)
	double sLon, sLat;	//起始地點
	bool isCarReach;	//車子是否到達
	int StateCode;
	Coor ss,ee;
	bool firstFind = true;
	vec_CMnode traceVec;
	vec_CMnode::iterator traIt;
	vec_CMnode::iterator printIt;
	int count = 0;
	
	CarGpsMapSystem* cgms = CarGpsMapSystem::getInstance(MAP_WIDTH,MAP_HEIGHT,init,xMax,yMax);

	ee.x = lon;
	ee.y = lat;

	
	mapPrint();

	if(!cgms->isInsideMap(ee.x,ee.y)){
			mapgotoxy(17,1);
			printf("goToLocation :cgms -> dest out ! lon:%lf, lat:%lf, mapLon:%lf ,mapLat:%lf\n",ee.x,ee.y,mapNode.x,mapNode.y);
			

			mapNode = cgms->gpsToCoordinate(ee);
			cgms->fixOutNode(mapNode);

			ee = cgms->coordinateToGps(mapNode);
			mapNode = cgms->gpsToCoordinate(ee);

			printf("fix to : lon:%lf, lat:%lf,mapLon:%lf,mapLat:%lf\n",ee.x,ee.y,mapNode.x,mapNode.y);

			//return GPS_NOT_IN_CARMAP;
	}

	do {
		//取得車子本身GPS座標
		#ifndef NO_CAR_MODE
		if(NOGPS == 2){
			//公院
			if(LocCount == 1){
				ss.x = 121.371028;
				ss.y = 24.944502;
				LocCount=3;
			}
			else if (LocCount ==2){
				ss.x = 121.372155;
				ss.y = 24.945151;
				LocCount=3;
			}
			//法院
		}
		else{
			getGPSLocation(ss.x,ss.y);
			
		}
		#else
		//leftdown
		ss.x = 121.370905;
		ss.y = 24.943603;
		#endif

		if(!cgms->isInsideMap(ss.x,ss.y)){
			//if(carLog){printf("goToLocation : cgms detect gps not in map region, lon:%lf, lat:%lf, mapLon:%lf ,mapLat:%lf\n",ss.x,ss.y,mapNode.x,mapNode.y);}
			mapgotoxy(17,1);
			printf("goToLocation : cgms ->  gps out! lon:%lf, lat:%lf, mapLon:%lf ,mapLat:%lf\n",ss.x,ss.y,mapNode.x,mapNode.y);
			
			mapNode = cgms->gpsToCoordinate(ss);
			cgms->fixOutNode(mapNode);

			ss = cgms->coordinateToGps(mapNode);
			mapNode = cgms->gpsToCoordinate(ss);

			printf("fix to : lon:%lf, lat:%lf,mapLon:%lf,mapLat:%lf\n",ss.x,ss.y,mapNode.x,mapNode.y);

			#ifndef NO_CAR_MODE
			//unistd::usleep(1000000);
			#endif
			//continue;
		}


		if(firstFind){

			cgms->findPath(traceVec,ss,ee,adj);

			char bufferN[256];
			sprintf(bufferN,"Debug info\n");
			if(carLog){cout << bufferN;}
			fileInput(bufferN);

			int recX = -1 ,recY = -1;
            for(traIt = traceVec.begin(); traIt != traceVec.end();traIt++){
				char buff1[256];
				sprintf(buff1,"Node : %d,%d\n",(*traIt)->GetCor_x(),(*traIt)->GetCor_y());
				fileInput(buff1);

				mapGoTo((*traIt)->GetCor_y(),(*traIt)->GetCor_x());

				cout << "●";

				if((*traIt)->GetstrongholdMark()){
					if(recX != (*traIt)->GetCor_x() || recY != (*traIt)->GetCor_y()){
						recX = (*traIt)->GetCor_x();
						recY = (*traIt)->GetCor_y();
						sprintf(buff1,"1,%d,%d",(*traIt)->GetCor_x(),(*traIt)->GetCor_y());
						write(pipeFds[1],buff1,sizeof(buff1));
					}
				}
            }

			traIt--;
			mapGoTo((*traIt)->GetCor_y(),(*traIt)->GetCor_x());
			cout << "★";

			if(traceVec.size()==0){
				return CAR_NOT_FOUND_ROAD;
			}

			traIt = traceVec.begin();
			firstFind = false;
		}

		Coor traCoor;
		traCoor.x = (*traIt)->GetCor_x();
		traCoor.y = (*traIt)->GetCor_y();
		Coor traGPS = cgms->coordinateToGps(traCoor);


		if((*traIt)->getDir() == 1 || (*traIt)->getDir() == 2){
			isCarReach = isCarReachDestination(directionInfo, distanceInfo, reachDistance, ss.x, ss.y, ss.x, traGPS.y);
		}
		else{
			isCarReach = isCarReachDestination(directionInfo, distanceInfo, reachDistance, ss.x, ss.y, traGPS.x, ss.y);
		}
		
		
		if(NOGPS == 2){
			ss.x = traGPS.x;
			ss.y = traGPS.y;
			isCarReach = true;
			#ifndef NO_CAR_MODE
			unistd::usleep(1000000);
			#endif
		}
		#ifndef NO_CAR_MODE
		//isCarReach = true;
		#endif
		
//		cout << ",go toward "<< directionInfo << " degree for " << distanceInfo / 10 << " meter." << endl;
		char buff[256];
		sprintf(buff,"%d,%d,%d,%d,go toward %lf degree for %lf kilometer.\n",(*traIt)->GetCor_x(),(*traIt)->GetCor_y(),count,traceVec.size(),directionInfo,distanceInfo);
		//if(carLog){printf(buff);}
		mapgotoxy(16,0);
		printf(buff);
		fileInput(buff);

		mapNode = cgms->gpsToCoordinate(ss);
		int dirInfo = (int)directionInfo;
		int newDirInfo,reachSH;
		if(dirInfo == 0){ newDirInfo = 1;}
		else if(dirInfo == 90){ newDirInfo = 4; }
		else if(dirInfo == 180){ newDirInfo = 2; }
		else if(dirInfo == 270){ newDirInfo = 3; }

		bool isReachSH = false;
		isReachSH = cgms->carMapNode[(int)traCoor.x][(int)traCoor.y].GetstrongholdMark();
		if(isReachSH){ 
			reachSH = cgms->carMapNode[(int)traCoor.x][(int)traCoor.y].GetStronghold(); 
		}else{reachSH = 100;}
		sprintf(buff,"%c%c%c%c%c",2,(int)mapNode.x,(int)mapNode.y,newDirInfo,reachSH);
		write(pipeFds[1],buff,sizeof(buff));


		if (isCarReach) {
			mapGoTo((*traIt)->GetCor_y(),(*traIt)->GetCor_x());
			cout << "○";

			count++;
			traIt++;
			if(count == traceVec.size()){
				//break;
			}
		}


		#ifndef NO_CAR_MODE
		//unistd::usleep(100);
		#endif
	} while ( traIt != traceVec.end() || firstFind == true );

	return CAR_OK;
}

UserRequest* waitRequest(RequestObserver *reqObserver){
	cout<<"waitRequest : Wait for SenderRequest..."<<endl;
	while(!reqObserver->HasRequest()){
		
	}
	
//cin.get();

	return reqObserver->RequestPop();
}


int recvSenderRequest(UserRequest* req){
	int StateCode;


	//剖析state 與 經緯度
	//int rState = parseRequestData(req);
	if(req->state != 0){
		cout << "recvSenderRequest : recv state data error! tarState = " << 0 << ",Recv_state = " << req->state << endl;
		return CAR_STATE_0_ERROR;
	}
	

	printf("recvSenderRequest : recv source longitude = %lf ,latitude = %lf\n", req->src_lon, req->src_lat);
	printf("recvSenderRequest : recv destnation longitude = %lf,latitude = %lf\n",req->dest_lon,req->dest_lat);
	printf("recvSenderRequest : recv state = %d\n",req->state);
	
	return CAR_OK;
}

int moveToSender(UserRequest* req){
	printf("moveToSender : Begin go to sender location\n");

	PacketManager *pac = PacketManager::getInstance(receivePeriod);

	LocCount =1;

	int e = goToLocation(req->src_lon,req->src_lat);
	if(e != CAR_OK){
		printf("moveToSender : goToLocation error\n");
		return e;
	}

	mapPrint();

	mapgotoxy(17,0);
	printf("moveToSender : reach destnation!\n");
	#ifndef NO_CAR_MODE
	unistd::usleep(2000);
	#endif

	char buff[256];
	sprintf(buff,"%c",3);
	write(pipeFds[1],buff,sizeof(buff));

	req->state = 1;
	
#ifndef NO_CAR_MODE
	pac->sendState(req->packetNum,req->state);
#endif
	
	return CAR_OK;
}

int beginTransport(UserRequest* req){
	
	printf("beginTransport : Wait for sender place the file\n");
	
	PacketManager *pac = PacketManager::getInstance(receivePeriod);

	//改成偵測是否recv到收到寄件?
	
	//判定使用者放入文件，目前使用enter做為判定
	getchar();
	
	printf("beginTransport : Sender placed file\n");
	printf("beginTransport : goto longitude =  %lf, latitude = %lf\n",req->dest_lon, req->dest_lat);
	
	req->state = 2;

#ifndef NO_CAR_MODE
	pac->sendState(req->packetNum,req->state);
#endif

	return CAR_OK;
}

int moveToReceiver(UserRequest* req){
	PacketManager *pac = PacketManager::getInstance(receivePeriod);
	
	LocCount =2;

	int e = goToLocation(req->dest_lon,req->dest_lat);
	if(e != CAR_OK){
		printf("moveToSender : goToLocation error\n");
		return e;
	}

	mapclear();

	
	mapgotoxy(17,1);
	printf("moveToReceiver : reach destnation!\n");
	#ifndef NO_CAR_MODE
	unistd::usleep(2000);
	#endif

	char buff[256];
	sprintf(buff,"%c",3);
	write(pipeFds[1],buff,sizeof(buff));

	req->state = 3;
	
#ifndef NO_CAR_MODE
	pac->sendState(req->packetNum,req->state);
#endif
	
	return CAR_OK;
}

int endTransport(UserRequest* req){
	string input;
	bool isCorrect = false;
	int StateCode;

	//等packetKey
	//!!!以後可能直接用Request直接有
	//req->packetKey = parsePassword();
	
	printf("endTransport : Wait for password input\n");

	PacketManager *pac = PacketManager::getInstance(receivePeriod);

	int key[4];

	fot(int i = 0; i < 4; i++){
		key[i] = (int)(req->packetKey[i]);
	}


	char buff[256];
	sprintf(buff,"%c%c%c%c%c",4,key[0],key[1],key[2],key[3]);
	write(pipeFds[1],buff,sizeof(buff));

	do{
		read(rpipeFds[0],buff,sizeof(buff));
		printf("rpipeFds : %d",buff[0]);
	}while(buff[0] != 1);
	
/*
	do{
		printf("Password :");
		getline(cin,input,'\n');
		
		if(input != req->packetKey){
			printf("Error! Please enter password again\n");
		}
		else{isCorrect = true;}
	}while(!isCorrect);
*/


	printf("Password correct!\n");
	printf("Press enter to end transport!\n");
	
	/*
		密碼箱打開後
		要判定東西被拿走
		這裡用按下enter來當作拿了
		~~~~以後要改成RFID
	*/
	getchar();
	
	req->state = 4;
	
	#ifndef NO_CAR_MODE
	pac->sendState(req->packetNum,req->state);
	#endif
	
	return CAR_OK;
}

#ifndef NO_CAR_MODE
int getGPSLocation(double &sLon,double &sLat){
	Coor init,xMax,yMax;
	CarGpsMapSystem* cgms = CarGpsMapSystem::getInstance(MAP_WIDTH,MAP_HEIGHT,init,xMax,yMax);
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
                        //printf("latitude: %f, longitude: %f, speed: %f, timestamp: %ld\n", myGPS_Data.fix.latitude, myGPS_Data.fix.longitude, myGPS_Data.fix.speed, myGPS_Data.fix.time); //EDIT: Replaced tv.tv_sec with gps_data.fix.time
						break;
               } else {
				   	mapgotoxy(16,1);
                    printf("no GPS data available\n");
                }
            }
        }
		else{
			mapgotoxy(16,1);
			printf("wait for 2 seconds to receive data again\n");
		}

        unistd::sleep(3);
    }
	
	sLon = myGPS_Data.fix.longitude;
	sLat = myGPS_Data.fix.latitude;

	Coor gpsData = {myGPS_Data.fix.longitude, myGPS_Data.fix.latitude};
	mapNode = cgms->gpsToCoordinate(gpsData);
	
	return CAR_OK;
}
#endif

void initCGMS(){
	CarGpsMapSystem* cgms = CarGpsMapSystem::getInstance(MAP_WIDTH,MAP_HEIGHT,init,xMax,yMax);

	Stronghold sArr[5][3];
    int value = 0;
    int xScale = (MAP_WIDTH - ROAD_WIDTH)/4;
    int yScale = (MAP_HEIGHT - ROAD_WIDTH)/2;
    /*
	for(int i = 0 ; i < 5;i++){
        for(int j = 0; j < 3; j++){
            value++;
            sArr[i][j].value = value;
            sArr[i][j].x = i * xScale + ROAD_WIDTH/2;
            sArr[i][j].y = j * yScale + ROAD_WIDTH/2;
            cgms->addStronghold(sArr[i][j]);
        }
    }
	*/

	double gpsData[SH_SIZE]={
        24.943584,121.370883,  //1.圖書公院轉角
        24.943888,121.370672,  //2.圖書館
		24.944148,121.370508,  //3.圖書行政轉角
        24.943946,121.371511,  //4.公院
		24.944436,121.371015,  //5.行政
        24.944188,121.371931,  //6.法商大道法院測
        24.944480,121.371801,  //7.法商大道
        24.944757,121.371559,  //8.法商大道商院測
        24.944498,121.372605,   //9.法院
        24.945091,121.372150  //10.商院
        //24.945390,121.372683,  //10.正門商院轉角
       // 24.945161,121.372851,  //11.正門
        //24.944835,121.373112,  //12.正門法院
        
    };


	for(int i = 0; i < SH_SIZE/2;i++){
		Coor gps,coor;
		gps.x = gpsData[i*2 + 1];
		gps.y = gpsData[i*2];
		coor = cgms->gpsToCoordinate(gps);

		Stronghold s;
		s.value = i+1;
		s.x = coor.x;
		s.y = coor.y;
		cgms->addStronghold(s);

/*


		if(!cgms->isInsideMap(gps.x,gps.y)){
			cgms->fixOutNode(coor);

			
		}
		*/
	}
	

    cgms->fillNodeStronghold();
    //cgms->printNodeStronghold();

    int n = cgms->getStrongholdNum();

    adj = new float*[n + 1];
    for (int i = 0; i <= n; i++) {
        adj[i] = new float[n + 1];
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            adj[i][j] = MAX_FLOAT;
        }
    }

	adj[1][4] = 75;adj[4][1] = 75;
	adj[4][6] = 50;adj[6][4] = 50;
	adj[6][9] = 75;adj[9][6] = 75;

	adj[1][2] = 35;adj[2][1] = 35;
	adj[2][3] = 35;adj[3][2] = 35;
	
	adj[6][7] = 35;adj[7][6] = 35;
	adj[7][8] = 35;adj[8][7] = 35;


	adj[3][5] = 75;adj[5][3] = 75;
	adj[5][8] = 50;adj[8][5] = 50;
	adj[8][10] = 75;adj[10][8] = 75;
	
	

	/*
    for(int i = 1; i <= 3 ;i+=2){
        for(int j = 0; j <= 9; j+=3){
            adj[i+j][i+j+3] = yScale;
            adj[i+j+3][i+j] = yScale;
        }
    }
    for(int i = 1; i <= 13;i+=6){
        for(int j = 0; j < 2; j++){
            adj[i+j][i+j+1] = xScale;
            adj[i+j+1][i+j] = xScale;
        }
    }
	*/
	
}


#ifndef NO_CAR_MODE
int sendPacket(UserRequest *req){
	sprintf(send_packet,"%d",req->state);
    e = sx1272.sendPacketTimeout(0, send_packet);
    printf("Packet sent, state :%d, code :%d\n",req->state,e);
}
#endif

int parseRequestData(UserRequest* req){
	const char *d = " ,";
	char* pacIdPtr;
	char* senderIdPtr;
	char* receiverIdPtr;
	char* packetKeyPtr;
	//char* statePtr;
	char* timePtr;
	char* sLonPtr;
	char* sLatPtr;
	char* dLonPtr;
	char* dLatPtr;

	#ifndef NO_CAR_MODE

	PacketManager *PacManager = PacketManager::getInstance(receivePeriod);

	//有識別碼版
	pacIdPtr = strtok( (PacManager->recv_buffer) + 4 ,d);	//index[4]後開始為資料
	printf("Split => pacId :%s",pacIdPtr);
	senderIdPtr = strtok(NULL,d);
	printf("senderId :%s,",senderIdPtr);
	receiverIdPtr = strtok(NULL,d);
	printf("receiverId :%s,",receiverIdPtr);
	packetKeyPtr = strtok(NULL,d);
	printf("packetKey :%s,",packetKeyPtr);
	timePtr = strtok(NULL,d);
	printf("time :%s,",timePtr);
	sLonPtr = strtok(NULL,d);
	printf("sLon :%s,",sLonPtr);
	sLatPtr = strtok(NULL,d);
	printf("sLat :%s,",sLatPtr);
	dLonPtr = strtok(NULL,d);
	printf("dLon :%s,",dLonPtr);
	dLatPtr = strtok(NULL,d);
	printf("dLat :%s\n",dLatPtr);
	
	
	req->packetNum = atoi(pacIdPtr);
	printf("Paese => packetNum :%d,",req->packetNum);
	req->senderID = atoi(senderIdPtr);
	printf("senderID :%d,",req->senderID);
	req->receiverID = atoi(receiverIdPtr);
	printf("receiverID :%d,",req->receiverID);
	req->packetKey.assign(packetKeyPtr);
	printf("packetKey :%s,",req->packetKey.c_str());
	req->sendTime.assign(timePtr);
	printf("sendTime :%s,",req->sendTime.c_str());

/*
	if(NOGPS == 2){
		req->src_lon = 121.370889;
		printf("sLon :%f\n",req->src_lon);
		req->src_lat = 24.943592;
		printf("sLat :%f\n",req->src_lat);
		req->dest_lon = 121.370522;
		printf("dLon :%f\n",req->dest_lon);
		req->dest_lat = 24.944187;
		printf("dLat :%f\n",req->dest_lat);
	}
	else{
		req->src_lon = atof(sLonPtr);
		printf("sLon :%f\n",req->src_lon);
		req->src_lat = atof(sLatPtr);
		printf("sLat :%f\n",req->src_lat);
		req->dest_lon = atof(dLonPtr);
		printf("dLon :%f\n",req->dest_lon);
		req->dest_lat = atof(dLatPtr);
		printf("dLat :%f\n",req->dest_lat);
		
	}
	*/

	req->src_lon = atof(sLonPtr);
	printf("sLon :%f,",req->src_lon);
	req->src_lat = atof(sLatPtr);
	printf("sLat :%f,",req->src_lat);
	req->dest_lon = atof(dLonPtr);
	printf("dLon :%f,",req->dest_lon);
	req->dest_lat = atof(dLatPtr);
	printf("dLat :%f,",req->dest_lat);

	req->state = 0;
	printf("state :%d\n",req->state);

	
	#else
	req->packetNum = 1;
	printf("packetNum :%d\n",req->packetNum);
	req->senderID = 1;
	printf("senderID :%d\n",req->senderID);
	req->receiverID = 2;
	printf("receiverID :%d\n",req->receiverID);
	char* pw = new char[pw_size];
	pw[0] = '\0';
	req->packetKey.assign(pw);
	printf("packetKey :%s\n",req->packetKey.c_str());
	req->sendTime.assign("time is money");
	printf("sendTime :%s\n",req->sendTime.c_str());
	req->src_lon = 121.371575;
	printf("sLon :%f\n",req->src_lon);
	req->src_lat = 24.944779;
	printf("sLat :%f\n",req->src_lat);
	req->dest_lon = 121.371575;
	printf("dLon :%f\n",req->dest_lon);
	req->dest_lat = 24.944779;
	printf("dLat :%f\n",req->dest_lat);

	req->state = 0;
	printf("state :%f\n",req->state);
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
