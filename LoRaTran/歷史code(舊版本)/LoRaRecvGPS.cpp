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
#include <stdio.h>
#include<stdlib.h>
int e;
char lastPacketMessage[100];

void setup()
{
  // Print a start message
  printf("SX1272 module and Raspberry Pi: receive packets without ACK\n");
  
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
  
  // Set the node address
  e |= sx1272.setNodeAddress(8);
  printf("Setting Node address: state %d\n", e);
  
  // Print a success message
  if (e == 0)
    printf("SX1272 successfully configured\n");
  else
    printf("SX1272 initialization failed\n");

  delay(1000);
}


//用他產生得message字串記得要free掉，不然程式可能會爆掉
char* recvMessage() {
	// Receive message
	e = sx1272.receivePacketTimeout(10000);

	char* messageStr;

	if (e == 0)
	{
		messageStr = (char*)malloc(sizeof(char)*sx1272.packet_received.length);

		printf("Receive packet, state %d\n", e);

		for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
		{
			lastPacketMessage[i] = (char)sx1272.packet_received.data[i];
			messageStr[i] = lastPacketMessage[i];
		}
		printf("Message: %s\n", lastPacketMessage);

		return messageStr;
	}
	else {
		printf("Receive packet, state %d\n", e);

		return NULL;
	}
}

void loop(void)
{
	char* recvGPSMsg = recvMessage();	//利用LoRa函式接收傳來的資料，並用指標存
	


	free(recvGPSMsg);//最後記得free掉
}

int main (){
	setup();
	while(1){
		loop();
	}
	return (0);
}
