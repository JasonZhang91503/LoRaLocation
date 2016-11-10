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
#include <iostream>
using namespace std;

int e;
string LoRaSendNum;
int NodeAddress;
int num = 0;

char SendStr[5];

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
  
  
  NodeAddress = stoi(LoRaSendNum);
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
	num++;
 	// Send packet broadcast and print the result
	sprintf(SendStr,"%i",NodeAddress);
    e = sx1272.sendPacketTimeout(0, SendStr);
    printf("Packet sent, state %d\n",e);
}

int main (int argc, char **argv){
	//LoRaSendNum = argv[0];
	//LoRaSendNum = "1";
	cout << "NodeAddress(1=A,2=B,3=C):";
	cin >> LoRaSendNum;
	
	setup();
	while(num < 1){
		loop();
	}
	return (0);
}
