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
#include <unistd.h>
#include <iostream>
using namespace std;

int e;

char message1 [256];

void inputHeader();

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

void inputHeader(){
    int temp;
    printf("Input sendType: ");
    scanf("%d",&temp);
    message1[0] = temp;
    printf("Input carID: ");
    scanf("%d",&temp);
    message1[1] = temp;
    printf("Input PacNum: ");
    scanf("%d",&temp);
    message1[2] = temp;
    printf("Input EventNum: ");
    scanf("%d",&temp);
    message1[3] = temp;
}

void setHeaderFromRecv(){
    message1[0] = 2;
    message1[1] = 1;
    message1[2] = message1[2];
    message1[3] = 1;
}



void sendRequest(){
    char buffer[256];
    inputHeader();
    sprintf(message1+4, "1,1,2,9487,timeIsMoney,121.370889,24.943592,121.370522,24.944187,");
//    message1[0] = 1;
    e = sx1272.sendPacketTimeout(0, message1);
    printf("Packet sent, state %d\n",e);
}

void sendGPS(){
    char buffer[256];
    inputHeader();
    double slon,slat,dlon,dlat;
    int start,end;

    double gpsData[100]={
        24.943617,121.370906,
        24.943592,121.370889,  //1.接近原點
        24.943891,121.370717,  //2.圖書館前
        24.944171,121.370530,  //3.社科道路轉角
        24.943911,121.371423,  //4.公院前
        24.944224,121.371951,  //5.法商大道法院測
        24.944509,121.371739,  //6.法商大道中間
        24.944781,121.371573,  //7.法商大道商院測
        24.945151,121.372155,  //8.商院前
        24.945487,121.372747,  //9.商院正門轉角
        24.945202,121.372979,  //10.正門
        24.944927,121.373166,  //11.法院正門轉角
        24.944572,121.372549,  //12.法院前
        24.944502,121.371028,  //13.行政前

        24.944148,121.370493,  //14.圖書行政轉角
        24.943888,121.370672,  //15.圖書館
        24.943584,121.370883,  //16.圖書公院轉角
        24.943946,121.371511,  //17.公院
        24.944188,121.371931,  //18.法商大道法院測
        24.944460,121.371761,  //19.法商大道
        24.944757,121.371559,  //20.法商大道商院測
        24.944436,121.371015,  //21.行政
        24.945091,121.372150,  //22.商院
        24.945390,121.372683,  //23.正門商院轉角
        24.945161,121.372851,  //24.正門
        24.944835,121.373112,  //25.正門法院
        24.944498,121.372605   //26.法院
    };
    
    printf("start:");
    scanf("%d",&start);
    printf("end:");
    scanf("%d",&end);



    sprintf(message1+4, "1,1,2,9487,timeIsMoney,%lf,%lf,%lf,%lf,",gpsData[start*2+1],gpsData[start*2],gpsData[end*2+1],gpsData[end*2]);
    e = sx1272.sendPacketTimeout(0, message1);
    printf("Packet sent, state %d\n",e);

}

void sendACK(){
//    char buffer[256];
 //   inputHeader();
//    sprintf(message1, "0,123.121212,24.252525,123.121212,24.252525,1234,");
//    message1[0] = 1;
    setHeaderFromRecv();
    unistd::usleep(1300000);
    e = sx1272.sendPacketTimeout(0, message1);
    printf("Packet sent, state %d\n",e);
}

void recvACK(){
    e = sx1272.receivePacketTimeout(5000);
    if ( e == 0 )
    {
        printf("Receive packet, state %d\n",e);

        for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
        {
        message1[i] = (char)sx1272.packet_received.data[i];
        }
        int role = message1[0];
        int carID = message1[1];
        int packNum = message1[2];
        int eventNum = message1[3];
        printf("role = %d, carID = %d, packNum = %d, eventNum = %d\n",role,carID,packNum,eventNum);
    }
    else {
        printf("Receive packet, state %d\n",e);
    }
}

void recvState(){
    e = sx1272.receivePacketTimeout(5000);
    if ( e == 0 )
    {
        printf("Receive packet, state %d\n",e);

        for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
        {
        message1[i] = (char)sx1272.packet_received.data[i];
        }
        int role = message1[0];
        int carID = message1[1];
        int packNum = message1[2];
        int eventNum = message1[3];
        int state = message1[4];
        printf("role = %d, carID = %d, packNum = %d, eventNum = %d, state = %d\n",role,carID,packNum,eventNum,state);
    }
    else {
        printf("Receive packet, state %d\n",e);
    }
}

void loop(void)
{
    int mode;
    printf("Input : ");
    scanf("%d",&mode);

    switch(mode){
        case 1:
            sendRequest();
            recvACK();
            break;
        case 2:
            recvState();
            sendACK();
            break;
        case 3:
            sendGPS();
            recvACK();
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
