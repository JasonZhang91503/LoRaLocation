#include<iostream>
#include"LoRa_RSSIToDistance.h"
using namespace std;

int main() {
	Rssi_BufferManager bufferManger;
	Rssi_info new_rssi;
	Rssi_info* rssi_arr;
	Locate_info loca_info;


	while (1) {
		/*
		recv基地台的RSSI
		*/


		
		bufferManger.buf_push(new_rssi);

		rssi_arr = bufferManger.collectTripleRssi();

		//若有收集到三個rssi  則包成locate_info並繼續做下去
		if (rssi_arr != NULL) {
			float d_arr[3];
			for (int i = 0; i < 3; i++) {
				d_arr[i] = Rssi_to_distance(*(rssi_arr + i), -50, 2);
			}
			
			loca_info = FillIndoorLocate_info(d_arr[0], d_arr[1], d_arr[2]);
		}
		else {
			continue;
		}


		/*
			將loca_info丟給distance_to_point計算;
		*/

	}
	

}