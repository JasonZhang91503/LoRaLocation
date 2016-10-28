//
//  main.cpp
//  Lora
//
//  Created by Keng on 19/10/2016.
//  Copyright © 2016 Keng. All rights reserved.
//

#include "arduPiLoRa.h"
#include <string>
#include "FileTran.h"
#include "rssi_to_distance/LoRa_RSSIToDistance.h"
#include "Lora_Positioning.h"
#include <time.h>
using namespace std;

int e;
float rssi_value;
int NodeAddress;
char my_packet[100];
Rssi_info rssi_info;
int RSSI_A=34,RSSI_n=2;
void setup()
{
    // Print a start message
    //printf("SX1272 module configuration in Raspberry Pi\n");
    
    // Power ON the module
    e = sx1272.ON();
    //printf("Setting power ON: state %d\n", e);

    // Set transmission mode
    e |= sx1272.setMode(4);
    printf("Setting Mode: state %d\n", e);
    
    // Set header
    e |= sx1272.setHeaderON();
    //printf("Setting Header ON: state %d\n", e);
    
    // Select frequency channel
    e |= sx1272.setChannel(CH_10_868);
    printf("Setting Channel: state %d\n", e);
    
    // Set CRC
    e |= sx1272.setCRC_ON();
    //printf("Setting CRC ON: state %d\n", e);
    
    // Select output power (Max, High or Low)
    e |= sx1272.setPower('H');
    //printf("Setting Power: state %d\n", e);
    // Set the node address
    NodeAddress=1;
    e |= sx1272.setNodeAddress(NodeAddress);
    printf("Setting Node address: state %d\n", e);
    
    // Print a success message
    if (e == 0)
        printf("SX1272 successfully configured\n");
    else
        printf("SX1272 initialization failed\n");
    
    delay(1000);
}

char* findDeviceID(char address){
    if(address == '1'){
        return "A";
    }
    else if(address == '2'){
		return "B";
    }
    else if(address == '3'){
        return "C";
    }
}





Rssi_info* RecvRssi(){
	Rssi_info* rssi_info= (Rssi_info*)malloc(sizeof(Rssi_info));;
    bool is_RSSI;
    char rssiValue[20];
    char ReceiveMessage[100];
    char deviceID;
    
    // Receive message
    e = sx1272.receivePacketTimeout(10000);

	printf("Receive packet, state %d\n", e);//1
	if (e != 0) {//if fail to get,return
		return NULL;
	}

    for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
    {
        my_packet[i] = (char)sx1272.packet_received.data[i];
    }

    //get rssi_value;
    is_RSSI=sx1272.getRSSIpacket();
	if (is_RSSI){	//if fail to get,return
		filePrint("rssi error!\n");
		return NULL;
	}

	//set Rssi_info
    rssi_value=sx1272._RSSIpacket;
    rssi_info->RSSI = rssi_value;
	rssi_info->deviceID = findDeviceID(my_packet[0]);

	//display RSSI and record it
	sprintf(ReceiveMessage,"Packet send from address = %s\n",my_packet);
	sprintf(rssiValue,"rssi = %.2f ",rssi_value);
	filePrint(ReceiveMessage);
	filePrint(rssiValue);

	return rssi_info;
}

void RecvWithTimeout(clock_t Timeout,clock_t Timeout_Begin,Rssi_info* A_buffer, Rssi_info*B_buffer, Rssi_info*C_buffer) {
	clock_t Timeout_End;

	do {
		Rssi_info* temp_rssi = RecvRssi();
		if (temp_rssi == NULL)continue;

		switch (temp_rssi->deviceID[0]) {
		case 'A':
			if (A_buffer == NULL) {
				A_buffer = temp_rssi;
			}
			break;
		case 'B':
			if (B_buffer == NULL) {
				B_buffer = temp_rssi;
			}
			break;
		case 'C':
			if (C_buffer == NULL) {
				C_buffer = temp_rssi;
			}
			break;
		default:
			break;
		}
		Timeout_End = clock();
	} while (Timeout_End - Timeout_Begin < 10000 && !A_buffer && !B_buffer && !C_buffer);
}

void Rssi_to_point(Rssi_info* A_buffer, Rssi_info* B_buffer, Rssi_info* C_buffer) {
	Locate_info loca_info;
	Point finalPoint;
	Point BSpointA, BSpointB, BSpointC;
	BSpointA = set_Bspoint(4.0, 4.0, 'A');
	BSpointB = set_Bspoint(9.0, 7.0, 'B');
	BSpointC = set_Bspoint(9.0, 1.0, 'C');
	char output[50];

	if (!A_buffer || !B_buffer || !C_buffer) {
		printf("Reaching Timeout!\n");
	}
	else {
		loca_info.distances[0] = Rssi_to_distance(*A_buffer, RSSI_A, RSSI_n);
		printf("Point %s , Distance : %.2f \n", A_buffer->deviceID, loca_info.distances[0]);
		loca_info.distances[1] = Rssi_to_distance(*B_buffer, RSSI_A, RSSI_n);
		printf("Point %s , Distance : %.2f \n", B_buffer->deviceID, loca_info.distances[1]);
		loca_info.distances[2] = Rssi_to_distance(*C_buffer, RSSI_A, RSSI_n);
		printf("Point %s , Distance : %.2f \n", C_buffer->deviceID, loca_info.distances[2]);


		finalPoint = trilateration(BSpointA, BSpointB, BSpointC, loca_info.distances[0], loca_info.distances[1], loca_info.distances[2]);
		printf("finalPoint>> X: %.2f ,Y: %.2f \n", finalPoint.x, finalPoint.y);
		sprintf(output, "finalPoint>> X: %.2f ,Y: %.2f \n", finalPoint.x, finalPoint.y);
		fileInput(output);
	}
}


int main (int argc, char **argv){   
    Rssi_info* A_buffer = NULL ,*B_buffer = NULL,*C_buffer = NULL;
	clock_t Timeout_Begin;
	
	//file Operation
	if (argv[1] != NULL) {
		fileOpen(argv[1]);
	}

    setup();
    printf("Begin receiving message ! \n");
	//接收RSSI
	while(true){//新循環
		printf("Timeout Begin set = 0 ! \n");
		Timeout_Begin = clock();
		A_buffer = NULL;
		B_buffer = NULL;
		C_buffer = NULL;
		
		//10秒內判斷是否收到
		RecvWithTimeout(10 * 1000, Timeout_Begin, A_buffer, B_buffer, C_buffer);
		Rssi_to_point(A_buffer, B_buffer, C_buffer);
	}
    return (0);
}



