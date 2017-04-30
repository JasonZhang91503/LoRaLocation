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

int e;

char message1 [256];

void setup()
{
  // Print a start message
  printf("SX1272 module and Raspberry Pi: send packets without ACK\n");
  
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
  e |= sx1272.setNodeAddress(3);
  printf("Setting Node address: state %d\n", e);
  
  // Print a success message
  if (e == 0)
    printf("SX1272 successfully configured\n");
  else
    printf("SX1272 initialization failed\n");

  delay(1000);
}

void sendRequest(){
    char buffer[256];
    inputHeader();
    sprintf(message1+4, "0,123.121212,24.252525,123.121212,24.252525,1234,");
    message1[0] = 1;
    e = sx1272.sendPacketTimeout(0, message1);
    printf("Packet sent, state %d\n",e);
}

void inputHeader(){
    printf("Input sendType: ");
    scanf("%d",&(int*)message1[0]);
    printf("Input carID: ");
    scanf("%d",&(int*)message1[1]);
    printf("Input PacNum: ");
    scanf("%d",&(int*)message1[2]);
    printf("Input EventNum: ");
    scanf("%d",&(int*)message1[3]);
}

void loop(void)
{
    int mode;
    printf("Input : ");
    scanf("%d",&mode);

    switch(mode){
        case 1:
            sendRequest();
            break;
        case 2:
            break;
    }
    
}

int main (){
	setup();
	while(1){
		loop();
	}
	return (0);
}
