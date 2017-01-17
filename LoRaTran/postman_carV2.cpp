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

 
//Include arduPi library
#include "arduPi.h"
#include "arduPiLoRa.h"
#include "arduPiClasses.h"
#include "Lora_Setup.h"

//Include Navigation library
#include "Navigation.h"

//Include gps library
#include <gps.h>

//std function
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <iostream>
#include <string>



using namespace std;

int e;
int state = -1;//用-1表示此車剛啟動,idle狀態
char recv_packet[100];	//車子接收資料的buffer
char send_packet[100];	//車子送出資料的buffer
int rc; //GPS的return code
//typedef struct gps_data_t GPS_Data;	//GPS的端口
struct gps_data_t myGPS_Data;	//GPS的端口
//GPS_Data gps_data;
int pw_size = 4;

bool isCarReachDestination(double &directionInfo, double &distanceInfo, double reachDistance, double sourceLon, double sourceLat, double destinationLon, double destinationLat);

void init(double *sLon,double *sLat,double *dLon,double *dLat,string *packetKey);
int recvSenderRequest(double *sLon,double *sLat,double *dLon,double *dLat);
int moveToSender(double dLon,double dLat);
int beginTransport(double *longitude,double *latitude);
int moveToReceiver(double dLon,double dLat,string *packetKey);
int endTransport(string *packetKey);
int checkState(int tarState);
int getGPSLocation(double &sLon,double &sLat);
int sendPacket();
int parseRequestData(double *sLon,double *sLat,double *dLon,double *dLat);
int parseStateData();
string parsePassword();

int GPSsetup(){
	if ((rc = gps_open("localhost", "2947", &myGPS_Data)) == -1) {
        printf("code: %d, reason: %s\n", rc, gps_errstr(rc));
        return EXIT_FAILURE;
    }
	printf("GPSsetup : success!\n");
    gps_stream(&myGPS_Data, WATCH_ENABLE | WATCH_JSON, NULL);
	
	return 0;
}


int main(){
	int error;	//
	double *src_longitude = NULL;
	double *src_latitude = NULL;
	double *dest_longitude = NULL;
	double *dest_latitude = NULL;
	string *packetKey = NULL;
	
	
	error = setup(1);//
	if(error != 0){
		cout << "LoRa error occur" << endl;
		return error;
	}
	
	error = GPSsetup();
	if(error != 0){
		cout << "GPSSetup error occur" << endl;
		return error;
	}
	
	//car程式開始執行
	
	/*
	功能
	state0. 向 Server 詢問是否有要寄件 - 檢查是否有寄件資訊的 state 為 0 的，有的話在預定時間到寄件地點。
	state1. 抵達指定地點 - 將 state 的狀態設為 1。
	state2. 寄件者放入信件並在板子上按 Enter ，並向收件地點前進 - 將 state 的狀態設為 2 。
	state3. 打達寄件地點，產生一組取件密碼 - 將 state 設為 3 。
	state4. 使用者輸入密碼並取件 - 將 state 設為  4 。
	*/
	
	//init(src_longitude,src_latitude,dest_longitude,dest_latitude,packetKey);
	src_longitude = new double;
	src_latitude = new double;
	dest_longitude = new double;
	dest_latitude = new double;
	packetKey = new string;
	state = -1;
	
	//開始送貨循環
	
	cout << "Begin transport" << endl;
	
	//state -1->0 = idle - >接收到經緯度,開始到sender地點
	e = recvSenderRequest(src_longitude,src_latitude,dest_longitude,dest_latitude);
	if(e != CAR_OK){
		cout << "main : recvSenderRequest method error, code = " << e << endl;
	}
	
	//state 0->1 = 行走->到達sender指定地點
	moveToSender(*src_longitude,*src_latitude);
	if(e != CAR_OK){
		cout << "main : moveToSender method error, code = " << e << endl;
	}
	
	//state 1->2 = 抵達->sender放入文件，開始前往recv點
	beginTransport(dest_longitude,dest_latitude);
	if(e != CAR_OK){
		cout << "main : beginTransport method error, code = " << e << endl;
	}
	
	//state 2->3 = 抵達->告知抵達，並且接收packetKey
	moveToReceiver(*dest_longitude,*dest_latitude,packetKey);
	if(e != CAR_OK){
		cout << "main : moveToReceiver method error, code = " << e << endl;
	}
	
	//state 3->4 = 等待領或->輸入密碼成功，取貨完畢
	endTransport(packetKey);
	if(e != CAR_OK){
		cout << "main : endTransport method error, code = " << e << endl;
	}
	
    return 0;
}

void init(double *sLon,double *sLat,double *dLon,double *dLat,string *packetKey){
	sLon = new double;
	sLat = new double;
	dLon = new double;
	dLat = new double;
	packetKey = new string;
	state = -1;
}

int recvSenderRequest(double *sLon,double *sLat,double *dLon,double *dLat){
	int StateCode;
	
	//確認state是否正確
	StateCode = checkState(-1);
	if(StateCode != CAR_OK){
		cout << "recvSenderRequest : return error in method checkState before recv data" << endl;
		return StateCode;
	}
	
	
	cout<<"recvSenderRequest : Wait for SenderRequest..."<<endl;
	
	
	while(true){
		e = sx1272.receivePacketTimeout(10000);
	
		if(e != 0){
			cout << "recvSenderRequest : no data received, receive again" << endl;
			continue;
		}
		
		for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
		{
			recv_packet[i] = (char)sx1272.packet_received.data[i];
			cout << recv_packet[i];
		}
		cout << endl;
	
	}

	//剖析state 與 經緯度
	int rState = parseRequestData(sLon,sLat,dLon,dLat);
	if(rState != 0){
		cout << "recvSenderRequest : recv state data error! tarState = " << 0 << ",Recv_state = " << rState << endl;
		return CAR_STATE_0_ERROR;
	}
	
	cout << "recvSenderRequest : recv source longitude = " << *sLon << ", latitude = " << *sLat << endl;
	cout << "recvSenderRequest : recv destnation longitude = " << *dLon << ", latitude = " << *dLat << endl;
	cout << "recvSenderRequest : recv state = " << rState << endl;
	
	
	//確認state是否正確
	StateCode = checkState(-1);
	if(StateCode != CAR_OK){
		cout << "recvSenderRequest : retrun error in method checkState after recv data" << endl;
		return StateCode;
	}
	
	state = rState;
	
	return CAR_OK;
}

int moveToSender(double dLon,double dLat){
	double directionInfo, distanceInfo;	//方位與距離之回傳
	double reachDistance = 0.1;	//判定多少距離內算到達(單位公里)
	double sLon, sLat;	//起始地點
	bool isCarReach;	//車子是否到達
	int StateCode;
    
	
	//確認state是否正確
	StateCode = checkState(0);
	if(StateCode != CAR_OK){
		cout << "moveToSender : retrun error in method checkState before reach destnation" << endl;
		return StateCode;
	}
	
	
	do {
		//取得車子本身GPS座標
		getGPSLocation(sLon,sLat);
		
		//去計算是否到達目標
		isCarReach = isCarReachDestination(directionInfo, distanceInfo, reachDistance, sLon, sLat, dLon, dLat);
		
		if (isCarReach) {
			cout << "moveToSender : reach destnation!" << endl;
			break;
		}
		
		cout << "You should still go toward "<< directionInfo << " degree for " << distanceInfo << " kilometer." << endl;
		unistd::usleep(1000);
	} while ( -180 < sLon && sLon < 180);
	
	
	//確認state是否正確
	StateCode = checkState(0);
	if(StateCode != CAR_OK){
		cout << "moveToSender : retrun error in method checkState after reach destnation" << endl;
		return StateCode;
	}
	state = 1;
	
	sendPacket();
	
	return CAR_OK;
}

int beginTransport(double *longitude,double *latitude){
	int StateCode;
	
	//確認state是否正確
	StateCode = checkState(1);
	if(StateCode != CAR_OK){
		cout << "beginTransport : retrun error in method checkState before recv data" << endl;
		return StateCode;
	}
	
	
	cout<<"beginTransport : Wait for Receiver location..."<<endl;
	while(true){
		e = sx1272.receivePacketTimeout(10000);
	
		if(e != 0){
			cout << "recvSenderRequest : no data received, receive again" << endl;
			continue;
		}
		
		for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
		{
			recv_packet[i] = (char)sx1272.packet_received.data[i];
			cout << recv_packet[i];
		}
		cout << endl;
	
	}
	
	//分析state	
	int rState = parseStateData();
	if(rState != 2){
		cout << "beginTransport : recv state data error! tarState = " << 2 << ",Recv_state = " << rState << endl;
		return CAR_STATE_2_ERROR;
	}
	
	//判定使用者放入文件，目前使用enter做為判定
	cout << "beginTransport : Wait for sender place the file" << endl;
	cout << "beginTransport :Press enter to start transport" << endl;
	getchar();
	
	cout << "beginTransport : goto longitude = " << *longitude << ", latitude = " << *latitude << endl;
	
	//確認state是否正確
	StateCode = checkState(1);
	if(StateCode != CAR_OK){
		cout << "recvSenderRequest : retrun error in method checkState after recv data" << endl;
		return StateCode;
	}
	
	state = 2;
	
	return CAR_OK;
}

int moveToReceiver(double dLon,double dLat,string *packetKey){
	double directionInfo, distanceInfo;	//方位與距離之回傳
	double reachDistance = 0.1;	//判定多少距離內算到達(單位公里)
	double sLon, sLat;	//起始地點
	bool isCarReach;	//車子是否到達
	int StateCode;
	
	//確認state是否正確
	StateCode = checkState(2);
	if(StateCode != CAR_OK){
		cout << "moveToReceiver : retrun error in method checkState before reach destnation" << endl;
		return StateCode;
	}
	
	do {
		
		//取得車子本身GPS座標
		getGPSLocation(sLon,sLat);
		
		//去計算是否到達目標
		isCarReach = isCarReachDestination(directionInfo, distanceInfo, reachDistance, sLon, sLat, dLon, dLat);
		
		if (isCarReach) {
			cout << "moveToReceiver : reach destnation!" << endl;
			break;
		}
		
		cout << "You should still go toward "<< directionInfo << " degree for " << distanceInfo << " kilometer." << endl;
		unistd::usleep(1000);
	} while ( -180 < sLon && sLon < 180);
	
	//確認state是否正確
	StateCode = checkState(2);
	if(StateCode != CAR_OK){
		cout << "moveToReceiver : retrun error in method checkState after reach destnation" << endl;
		return StateCode;
	}
	state = 3;
	
	sendPacket();
	
	//recv一組密碼
	cout<<"moveToReceiver : Wait for PacketKey..."<<endl;
	e = sx1272.receivePacketTimeout(10000);
	
	if(e != 0){
		cout << "moveToReceiver : error! receive data error occur" << endl;
		return e;
	}
	
	for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
    {
        recv_packet[i] = (char)sx1272.packet_received.data[i];
		cout << recv_packet[i];
    }
	cout << endl;
	
	/*task
		將recv的密碼分析成字串
		存入packetKey內
	*/
	*packetKey = parsePassword();
	
	//目前先用9487來代替
	//*packetKey = "9487";
	
	
	
	
	/*task
		send 新state資訊給gateway(主動)
	*/
	
	
	return CAR_OK;
}

int endTransport(string *packetKey){
	string input;
	bool isCorrect = false;
	int StateCode;
	
	//確認state是否正確
	StateCode = checkState(3);
	if(StateCode != CAR_OK){
		cout << "endTransport : retrun error in method checkState after reach destnation" << endl;
		return StateCode;
	}
	
	
	cout << "endTransport : Wait for password input" << endl << endl;
	do{
		cout << "Password :";
		getline(cin,input,'\n');
		
		if(input != *packetKey){
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
	*/
	getchar();
	
	
	//確認state是否正確
	StateCode = checkState(3);
	if(StateCode != CAR_OK){
		cout << "endTransport : retrun error in method checkState after reach destnation" << endl;
		return StateCode;
	}
	state = 4;
	
	/*task
		send 新state資訊給gateway(主動)
	*/
	sendPacket();
	
	return CAR_OK;
}


int checkState(int tarState){
	int returnCode;
	switch(tarState){
		case -1:
			returnCode = CAR_STATE_999_ERROR;
			break;
		case 0:
			returnCode = CAR_STATE_0_ERROR;
			break;
		case 1:
			returnCode = CAR_STATE_1_ERROR;
			break;			
		case 2:
			returnCode = CAR_STATE_2_ERROR;
			break;
		case 3:
			returnCode = CAR_STATE_3_ERROR;
			break;
		case 4:
			returnCode = CAR_STATE_4_ERROR;
			break;			
	}
	
	if(state != tarState){
		cout << "checkState : error! state not correct, currentState = " << state << ", targetState = " << tarState << endl;
		return returnCode;
	}
	else{
		return CAR_OK;
	}
}

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
                    !isnan(myGPS_Data.fix.latitude) && 
                    !isnan(myGPS_Data.fix.longitude)) {
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

int sendPacket(){
	sprintf(send_packet,"%d",state);
    e = sx1272.sendPacketTimeout(0, send_packet);
    printf("Packet sent, state :%d, code :%d\n",state,e);
}

int parseRequestData(double *sLon,double *sLat,double *dLon,double *dLat){
	const char *d = " ,";
	char* statePtr;
	char* sLonPtr;
	char* sLatPtr;
	char* dLonPtr;
	char* dLatPtr;
	
	statePtr = strtok(recv_packet,d);	//state
	printf("split state :%s",statePtr);
	sLonPtr = strtok(recv_packet,d);
	printf("split sLon :%s",sLonPtr);
	sLatPtr = strtok(recv_packet,d);
	printf("split sLat :%s",sLatPtr);
	dLonPtr = strtok(recv_packet,d);
	printf("split dLon :%s",dLonPtr);
	dLatPtr = strtok(recv_packet,d);
	printf("split dLat :%s",dLatPtr);
	
	int rState = atoi(statePtr);
	printf("rState :%d",rState);
	*sLon = atof(sLonPtr);
	printf("rState :%f",*sLon);
	*sLat = atof(sLatPtr);
	printf("rState :%f",*sLat);
	*dLon = atof(dLonPtr);
	printf("rState :%f",*dLon);
	*dLat = atof(dLatPtr);
	printf("rState :%f",*dLat);
	
	/*
	int rState = 0;
	printf("rState :%d",rState);
	*sLon = 121.369862;
	printf("rState :%f",*sLon);
	*sLat = 24.944185;
	printf("rState :%f",*sLat);
	*dLon = 121.369862;
	printf("rState :%f",*dLon);
	*dLat = 24.944185;
	printf("rState :%f",*dLat);
	*/
	
	return rState;
}

int parseStateData(){
	int rState = atoi(recv_packet);
	printf("rState :%d",rState);
	
	return rState;
}

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
