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


//std function
#include <stdio>
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
	string *packetKey = NULL;
	
	
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
	
	
	init(longitude,latitude,packetKey);
	
	//開始送貨循環
	
	cout << "Begin transport" << endl;
	
	//state -1->0 = idle - >接收到經緯度,開始到sender地點
	recvSenderRequest();
	
	
	//state 0->1 = 行走->到達sender指定地點
	moveToSender();
	
	
	//state 1->2 = 抵達->sender放入文件，開始前往recv點
	beginTransport();
	
	
	//state 2->3 = 抵達->告知抵達，並且接收packetKey
	moveToReceiver();
	
	
	//state 3->4 = 等待領或->輸入密碼成功，取貨完畢
	endTransport();
	
	
    return 0;
}

void init(double *longitude,double *latitude,string *packetKey){
	longitude = new double;
	latitude = new double;
	packetKey = new string;
	state = -1;
}

int recvSenderRequest(double *longitude,double *latitude){
	int StateCode;
	
	//確認state是否正確
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
	
	
	//確認state是否正確
	StateCode = checkState(-1);
	if(StateCode != CAR_OK){
		cout << "recvSenderRequest : retrun error in method checkState after recv data" << endl;
		return StateCode;
	}
	
	state = 0;
	
	/*task
		send 新state資訊給gateway(被動)
	*/
	
	return CAR_OK;
}

int moveToSender(double dLon,double dLat){
	double directionInfo, distanceInfo;	//方位與距離之回傳
	double reachDistance = 0.01;	//判定多少距離內算到達(單位公里)
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
	
	
	//確認state是否正確
	StateCode = checkState(0);
	if(StateCode != CAR_OK){
		cout << "moveToSender : retrun error in method checkState after reach destnation" << endl;
		return StateCode;
	}
	state = 1;
	
	/*task
		send 新state資訊給gateway(主動)
	*/
	
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
	e = sx1272.receivePacketTimeout(10000);
	
	if(e != 0){
		cout << "beginTransport : error! receive data error occur" << endl;
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
	
	
	//判定使用者放入文件，目前使用enter做為判定
	getchar();
	
	cout << "beginTransport : recv longitude = " << *longitude << ", latitude = " << *latitude << endl;
	
	
	//確認state是否正確
	StateCode = checkState(1);
	if(StateCode != CAR_OK){
		cout << "recvSenderRequest : retrun error in method checkState after recv data" << endl;
		return StateCode;
	}
	
	state = 2;
	
	/*task
		send 新state資訊給gateway(主動)
	*/
}

int moveToReceiver(double dLon,double dLat,string *packetKey){
	double directionInfo, distanceInfo;	//方位與距離之回傳
	double reachDistance = 0.01;	//判定多少距離內算到達(單位公里)
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
		
		/*
			透過GPS函式取得GPS資訊
		*/
		
		isCarReach = isCarReachDestination(directionInfo, distanceInfo, reachDistance, sLon, sLat, dLon, dLat);
		
		if (isCarReach) {
			cout << "moveToReceiver : reach destnation!" << endl;
			break;
		}
		
		cout << "You should still go toward "<< directionInfo << " degree for " << distanceInfo << " kilometer." << endl;
		usleep(1000);
	} while ( -180 < sLon && sLon < 180);
	
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
    }
	
	/*task
		將recv的密碼分析成字串
		存入packetKey內
	*/
	//目前先用9487來代替
	*packetKey = "9487";
	
	
	//確認state是否正確
	StateCode = checkState(2);
	if(StateCode != CAR_OK){
		cout << "moveToReceiver : retrun error in method checkState after reach destnation" << endl;
		return StateCode;
	}
	state = 3;
	
	/*task
		send 新state資訊給gateway(主動)
	*/
	
	return CAR_OK;
}

int endTransport(string *packetKey){
	string input;
	bool isCorrect = false;
	
	//確認state是否正確
	StateCode = checkState(3);
	if(StateCode != CAR_OK){
		cout << "endTransport : retrun error in method checkState after reach destnation" << endl;
		return StateCode;
	}
	
	
	cout << "endTransport : Wait for password input" << endl << endl;
	do{
		cout << "Password :";
		cin.get(input,4);
		
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
