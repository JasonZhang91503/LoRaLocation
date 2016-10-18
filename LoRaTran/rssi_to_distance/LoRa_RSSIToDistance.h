#pragma once

#include<queue>
#include <math.h>
using namespace std;
/*-------------------------------------------------
struct Locate_info:
描述:
	LoRa最終傳給手機所用的封包格式,mode = 0為室內模式,
	mode = 1則為室外模式				
成員:
	mode = 傳送的模式,0則傳distances,1則傳座標x,y
	distance = 由RSSI轉換後得知的強度距離,共三組
				分別存如A,B,C所算出來的距離
	x,y = 經過三角定位後的座標
-------------------------------------------------*/
struct Locate_info
{
	unsigned int mode;
	float distances[3];
	double x, y;
};

/*-------------------------------------------------
struct Rssi_info:
描述:
	一個LoRa定位點所傳給接收端的RSSI與deviceID配對
成員:
	RSSI = RSSI訊號強度值
	deviceID = 指向char型態的字串陣列,用以表示一個deviceID
	number = 此次Rssi在系統中的編號
-------------------------------------------------*/
struct Rssi_info
{
	float RSSI;
	char* deviceID = NULL;
	int number;
};

/*
class Rssi_BufferManager {
public:

private:
	Rssi_info *rssiBuffer;
	int buf_MaxSize = 20;
	char* deviceID_a;
	char* deviceID_b;
	char* deviceID_c;
};
*/

class Rssi_BufferManager {
public:
	Rssi_BufferManager() {
		MAX_packetNumber = 10000;
		deviceID_a = "A";
		deviceID_b = "B";
		deviceID_c = "C";
		currentNumber = 0;
		nextNumber = 0;
	}

	void buf_push(Rssi_info new_rssi) {
		queue<Rssi_info>* queue = getQueue(new_rssi.deviceID);
		if (new_rssi.number < currentNumber) {
			return;
		}
		else if (new_rssi.number > currentNumber) {
			if (new_rssi.number > nextNumber) {
				currentNumber = new_rssi.number;
				nextNumber = new_rssi.number;
				popToCurrentNumber();
			}
		}
		queue->push(new_rssi);
		
		changeBufStateAfterPush();
	}

	void buf_pop(char* deviceID) {
		popFrontByNumber(1,deviceID);
	}

	Rssi_info* collectTripleRssi() {
		if (RssiInfo_queue_A.size() > 0 && RssiInfo_queue_B.size() > 0 && RssiInfo_queue_C.size() > 0) {
			Rssi_info info[3];
			info[0] = RssiInfo_queue_A.front();
			info[1] = RssiInfo_queue_B.front();
			info[2] = RssiInfo_queue_C.front();

			RssiInfo_queue_A.pop();
			RssiInfo_queue_B.pop();
			RssiInfo_queue_C.pop();
			currentNumber++;
			return info;
		}
		else {
			return NULL;
		}
	}

	void popFrontByNumber(unsigned number,char* deviceID) {
		queue<Rssi_info>* queue = getQueue(deviceID);
		for (int i = 0; i < number; i++) {
			if (queue->size() == 0) {
				break;
			}
			queue->pop();
		}
	}
	void popToCurrentNumber() {
		while (RssiInfo_queue_A.front().number < currentNumber) {
			RssiInfo_queue_A.pop();
		}
		while (RssiInfo_queue_B.front().number < currentNumber) {
			RssiInfo_queue_B.pop();
		}
		while (RssiInfo_queue_C.front().number < currentNumber) {
			RssiInfo_queue_C.pop();
		}
	}
	
private:
	queue<Rssi_info>* getQueue(char* deviceID) {
        if (deviceID == deviceID_a) {
			return &RssiInfo_queue_A;
		}
		if (deviceID == deviceID_b) {
			return &RssiInfo_queue_B;
		}
		if (deviceID == deviceID_c) {
			return &RssiInfo_queue_C;
		}
        //new
        return &RssiInfo_queue_C;
	}
	bool changeBufStateAfterPush() {
		bool IsChanged = false;

		if (currentNumber == nextNumber) {
			//除了原本buf存入的rssi三組值外，湊齊了下一組的rssi三組值
			if (RssiInfo_queue_A.size() == 2 && RssiInfo_queue_B.size() == 2 && RssiInfo_queue_C.size() == 2) {
				RssiInfo_queue_A.pop();
				RssiInfo_queue_B.pop();
				RssiInfo_queue_C.pop();
				currentNumber = currentNumber + 1;
				nextNumber = currentNumber + 1;
				
				IsChanged = true;
			}
		}
		return IsChanged;
	}
	queue<Rssi_info> RssiInfo_queue_A;
	queue<Rssi_info> RssiInfo_queue_B;
	queue<Rssi_info> RssiInfo_queue_C;
	char* deviceID_a;
	char* deviceID_b;
	char* deviceID_c;
	int MAX_packetNumber;
	int currentNumber;
	int nextNumber;
};



/*-------------------------------------------------
Rssi_to_distance(Rssi_info rssi_info):
描述:
	將Rssi值轉換為距離(distance)的公式
參數:
	rssi_info = 結構為Rssi_info,用以儲存RSSI與deviceID
	A = 發射端和接收端相隔1米時的信號強度
	N = 環境衰減因子
計算公式：
    D =10 ^（（ABS（RSSI） - A）/（10* N））
	其中：
	D - 計算所得距離
	RSSI - 接收信號強度（負值）
	A - 發射端和接收端相隔1米時的信號強度
	N - 環境衰減因子
-------------------------------------------------*/
float Rssi_to_distance(Rssi_info rssi_info,float A,float N) {
	float rssi = rssi_info.RSSI;
	float power;
	float distance;
	char* deviceID = rssi_info.deviceID;
	
	//D =10 ^（（ABS（RSSI） - A）/（10* N））
	rssi = fabsf(rssi);
	power = (rssi - A) / (10 * N);
	distance = pow(10, power);
	return distance;
}


/*-------------------------------------------------
FillIndoorLocate_info(float d_a,float d_b,float d_c):
描述:
	將三個distance值轉包入Locate_info結構
參數:
	d_a = 測點到A基地台的距離
	d_b = 測點到B基地台的距離
	d_c = 測點到C基地台的距離
-------------------------------------------------*/
Locate_info FillIndoorLocate_info(float d_a,float d_b,float d_c) {
	Locate_info indoor_info;

	indoor_info.mode = 0;
	indoor_info.distances[0] = d_a;
	indoor_info.distances[1] = d_b;
	indoor_info.distances[2] = d_c;

	return indoor_info;
}


/*-------------------------------------------------
FillOutdoorLocate_info(double x, double y):
描述:
	將測量點的x,y值轉包入Locate_info結構
參數:
	x = 測點的x值
	y = 測點的y值
-------------------------------------------------*/
Locate_info FillOutdoorLocate_info(double x, double y) {
	Locate_info indoor_info;

	indoor_info.mode = 1;
	indoor_info.x = x;
	indoor_info.y = y;
	
	return indoor_info;
}