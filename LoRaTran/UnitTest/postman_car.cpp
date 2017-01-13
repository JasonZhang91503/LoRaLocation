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
#define CAR_STATE_-1_ERROR 9487999
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


//std function
#include <iostream>
#include <string>


using namespace std;

int e;
int state = -1;//用-1表示此車剛啟動,idle狀態
char recv_packet[256];	//車子接收資料的buffer

void GPSsetup(){
	//等著填入
}


int main(){
	int error;	//
	double *longitude = NULL;
	double *latitude = NULL;
	
	
	error = setup(1);//
	if(error != 0){
		cout << "LoRa Setup error occur" << endl;
	}
	
	GPSsetup();
	
	//car程式開始執行
	
	/*
	功能
	state0. 向 Server 詢問是否有要寄件 - 檢查是否有寄件資訊的 state 為 0 的，有的話在預定時間到寄件地點。
	state1. 抵達指定地點 - 將 state 的狀態設為 1。
	state2. 寄件者放入信件並在板子上按 Enter ，並向收件地點前進 - 將 state 的狀態設為 2 。
	state3. 打達寄件地點，產生一組取件密碼 - 將 state 設為 3 。
	state4. 使用者輸入密碼並取件 - 將 state 設為  4 。
	*/
	
	
	state = -1;
	
	//開始送貨循環
	
	cout << "Begin transport" << endl;
	
	//state -1->0 = idle - >接收到經緯度,開始送貨
	recvSenderRequest();
	
	
	//state 0->1 = 送貨->到達指定地點
	moveToSender();
	
	
	
    return 0;
}

int recvSenderRequest(double *longitude,double *latitude){
	int StateCode;
	
	StateCode = checkState(-1);
	if(StateCode != CAR_OK){
		cout << "recvSenderRequest : retrun error in method checkState before recv data" << endl;
		return StateCode;
	}
	
	
	cout<<"recvSenderRequest : Wait for SenderRequest..."<<endl;
	e = sx1272.receivePacketTimeout(10000);
	
	if(e != 0){
		cout << "recvSenderRequest : error! receive data error occur" << endl;
		return e;
	}
	
	for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
    {
        recv_packet[i] = (char)sx1272.packet_received.data[i];
    }
	
	/*task
		剖析楷甯給的字串變成資料
		存到longitude與latitude兩個pointer裡面
	*/
	
	cout << "recvSenderRequest : recv longitude = " << *longitude << ", latitude = " << *latitude << endl;
	
	
	StateCode = checkState(-1);
	if(StateCode != CAR_OK){
		cout << "recvSenderRequest : retrun error in method checkState after recv data" << endl;
		return StateCode;
	}
	
	state = 0;
	
	/*task
		send 新state資訊給gateway
	*/
	
	return CAR_OK;
}

int moveToSender(double dLon,double dLat){
	double directionInfo, distanceInfo;	//方位與距離之回傳
	double reachDistance = 0.01;	//判定多少距離內算到達(單位公里)
	double sLon, sLat;	//起始地點
	bool isCarReach;	//車子是否到達
	int StateCode;
	
	StateCode = checkState(0);
	if(StateCode != CAR_OK){
		cout << "moveToSender : retrun error in method checkState before reach destnation" << endl;
		return StateCode;
	}
	
	do {
		
		/*
			透過GPS函式取得GPS資訊
		*/
		
		isCarReach = isCarReachDestination(directionInfo, distanceInfo, reachDistance, sLon, sLat, dLon, dLat);
		
		if (isCarReach) {
			cout << "moveToSender : reach destnation!" << endl;
			break;
		}
		
		cout << "You should still go toward "<< directionInfo << " degree for " << distanceInfo << " kilometer." << endl;
		usleep(1000);
	} while ( -180 < sLon && sLon < 180);
	
	
	StateCode = checkState(0);
	if(StateCode != CAR_OK){
		cout << "moveToSender : retrun error in method checkState after reach destnation" << endl;
		return StateCode;
	}
	state = 1;
	
	/*task
		send 新state資訊給gateway
	*/
	
	return CAR_OK;
}

int checkState(int tarState){
	int returnCode;
	switch(tarState){
		case -1;
			returnCode = CAR_STATE_-1_ERROR;
			break;
		case 0;
			returnCode = CAR_STATE_0_ERROR;
			break;
		case 1;
			returnCode = CAR_STATE_1_ERROR;
			break;			
		case 2;
			returnCode = CAR_STATE_2_ERROR;
			break;
		case 3;
			returnCode = CAR_STATE_3_ERROR;
			break;
		case 4;
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
