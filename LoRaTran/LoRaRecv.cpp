 /*
 *  LoRa 868 / 915MHz SX1272 LoRa module
 *  
 *  Copyright (C) Libelium Comunicaciones Distribuidas S.L. 
 *  http://www.libelium.com 
 *  
 *  This program is free software: you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation, either version 3 of the License, or 
 *  (at your option) any later version. 
 *  
 *  This program is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License 
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *  
 *  Version:           1.2
 *  Design:            David Gascón 
 *  Implementation:    Covadonga Albiñana, Victor Boria, Ruben Martin
 */
 
// Include the SX1272 and SPI library: 
#include "arduPiLoRa.h"
#include <string>
#include "FileTran.h"
#include "rssi_to_distance/LoRa_RSSIToDistance.h"
using namespace std;

int e;
string LoRaRecvNum;
int NodeAddress;
float rssi_value;

void setup()
{
  // Print a start message
  printf("SX1272 module configuration in Raspberry Pi\n");
  
  // Power ON the module
  e = sx1272.ON();
  printf("Setting power ON: state %d\n", e);
  
  // Set transmission mode
  e |= sx1272.setMode(4);
  printf("Setting Mode: state %d\n", e);
  
  // Set header
  e |= sx1272.setHeaderON();
  printf("Setting Header ON: state %d\n", e);
  
  // Select frequency channel
  e |= sx1272.setChannel(CH_10_868);
  printf("Setting Channel: state %d\n", e);
  
  // Set CRC
  e |= sx1272.setCRC_ON();
  printf("Setting CRC ON: state %d\n", e);
  
  // Select output power (Max, High or Low)
  e |= sx1272.setPower('H');
  printf("Setting Power: state %d\n", e);
  
  
  NodeAddress = stoi(LoRaRecvNum);
  // Set the node address
  e |= sx1272.setNodeAddress(NodeAddress);
  printf("Setting Node address: state %d\n", e);
  
  // Print a success message
  if (e == 0)
    printf("SX1272 successfully configured\n");
  else
    printf("SX1272 initialization failed\n");

  delay(1000);
}

void loop(void)
{    
  // Receive message
	e = sx1272.receivePacketTimeout(10000);
	if ( e == 0 )
	{
		printf("Receive packet, state %d\n",e);

		for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
		{
			my_packet[i] = (char)sx1272.packet_received.data[i];
		}
		printf("Message: %s\n", my_packet);
	}
	else {
		printf("Receive packet, state %d\n",e);
	}
}

int Recv(Rssi_info &rssi_info){
	printf("Begin Recv Packet\n");
	e = sx1272.receivePacketTimeout(10000);
	if ( e == 0 )
	{
		printf("Receive packet, state %d\n",e);

		for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
		{
			my_packet[i] = (char)sx1272.packet_received.data[i];
		}
		
		//display packetSender's address and record it
		char ReceiveMessage[100];
		sprintf(ReceiveMessage,"Packet send from address = %s\n",my_packet);
		printf("%s", ReceiveMessage);
		fileInput(ReceiveMessage);
		
		//getRSSI from last packet
		e=sx1272.getRSSIpacket();
		if(e==0){
			rssi_value=sx1272._RSSIpacket;
			
			//display RSSI record it
			char rssiValue[20];
			sprintf(rssiValue,"rssi = %f ",rssi_value);
			printf("%s\n",rssiValue);
			fileInput(rssiValue);
		}
		else{
			printf("rssi error !\n");
			fileInput("rssi error!\n");
			return e;
		}
		
		int address = stoi(my_packet);
		char* deviceID = findDeviceID(address);
		
		
		rssi_info.RSSI = rssi_value;
		rssi_info.deviceID = deviceID;
		rssi_info.number = 1;
	}
	else {
		printf("Receive packet, state %d\n",e);
		return e;
	}
}

char* findDeviceID(int address){
	if(address == 1){
		return "A"
	}
	else if(address == 2){
		return "B"
	}
	else if(address == 3){
		return "C"
	}
}


int main (int argc, char **argv){
	LoRaRecvNum = argv[0];
	fileOpen(argv[1]);
	
	Rssi_BufferManager bufferManger;
	Rssi_info new_rssi;
	Rssi_info* rssi_arr;
	Locate_info loca_info;
	
	setup();
	
	
	int count = 0;
	while(count < 3){
		rssi_info.RSSI = 0;
		rssi_info.deviceID = NULL;
		rssi_info.number = 0;
		e = Recv(new_rssi);
		if(e != 0){
			printf("recving data error!End Program");
			return 1;
		}
		
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
		
		count++;
	}
	
	
	return (0);
}
