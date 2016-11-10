/*  
 *  GPRS+GPS Quadband Module (SIM908)
 *  
 *  Copyright (C) Libelium Comunicaciones Distribuidas S.L. 
 *  http://www.libelium.com 
 *  
 *  This program is free software: you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation, either version 3 of the License, or 
 *  (at your option) any later version. 
 *  a
 *  This program is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License 
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *  
 *  Version:           2.0
 *  Design:            David Gascón 
 *  Implementation:    Alejandro Gallego & Marcos Martinez
 */
 
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
 
 //Include arduPi library
 #include "arduPi.h"
#include "arduPiLoRa.h"

 int8_t sendATcommand(const char* ATcommand, const char* expected_answer1, unsigned int timeout);
 void power_on();
 

 int e;
int8_t answer;
int onModulePin= 2;
int counter;
long previous;


char Basic_str[100];
char GGA_str[100];
char GLL_str[100];
char RMC_str[100];
char VTG_str[100];
char ZDA_str[100];

void GPSsetup(){
    
    pinMode(onModulePin, OUTPUT);
    Serial.begin(115200);   
    
    printf("Starting...\n");
    power_on();
    
    delay(3000);
    
      
    sendATcommand("AT+CGPSPWR=1", "OK", 2000);
    sendATcommand("AT+CGPSRST=0", "OK", 2000);
    
    
    // waits for fix GPS
    while( (sendATcommand("AT+CGPSSTATUS?", "2D Fix", 5000) || 
            sendATcommand("AT+CGPSSTATUS?", "3D Fix", 5000)) == 0 );
    

}

void LoRasetup()
{
	// Print a start message
	printf("SX1272 module and Raspberry Pi: send packets without ACK\n");

	// Power ON the module
	e = sx1272.ON();
	printf("Setting power ON: state %d\n", e);

	// Set transmission mode
	e = sx1272.setMode(4);
	printf("Setting Mode: state %d\n", e);

	// Set header
	e = sx1272.setHeaderON();
	printf("Setting Header ON: state %d\n", e);
	
	// Select frequency channel
	e = sx1272.setChannel(CH_10_868);
	printf("Setting Channel: state %d\n", e);

	// Set CRC
	e = sx1272.setCRC_ON();
	printf("Setting CRC ON: state %d\n", e);

	// Select output power (Max, High or Low)
	e = sx1272.setPower('H');
	printf("Setting Power: state %d\n", e);

	// Set the node address
	e = sx1272.setNodeAddress(3);
	printf("Setting Node address: state %d\n", e);

	// Print a success message
	printf("SX1272 successfully configured\n\n");
	delay(1000);
}

void loop(){
    // Basic
    // Clean the input buffer
    while( Serial.available() > 0) Serial.read();    
    delay(100);
    // request Basic string
    sendATcommand("AT+CGPSINF=0", "AT+CGPSINF=0\r\n\r\n", 2000);
    
    counter = 0;
    answer = 0;
    memset(Basic_str, '\0', 100);    // Initialize the string
    previous = millis();
    // this loop waits for the NMEA string
    do{

        if(Serial.available() != 0){    
            Basic_str[counter] = Serial.read();
            counter++;
            // check if the desired answer is in the response of the module
            if (strstr(Basic_str, "OK") != NULL)    
            {
                answer = 1;
            }
        }
        // Waits for the asnwer with time out
    }while((answer == 0) && ((millis() - previous) < 2000));  
    
    Basic_str[counter-3] = '\0';
    
    printf("*************************************************\n");
    printf("Basic string: ");
    printf("%s\n",Basic_str);
   
    delay(1000);

}




void power_on(){

    uint8_t answer=0;
    
    // checks if the module is started
    answer = sendATcommand("AT", "OK", 2000);
    if (answer == 0)
    {
        // power on pulse
        digitalWrite(onModulePin,HIGH);
        delay(3000);
        digitalWrite(onModulePin,LOW);
    
        // waits for an answer from the module
        while(answer == 0){    
            // Send AT every two seconds and wait for the answer
            answer = sendATcommand("AT", "OK", 2000);    
        }
    }
    
}


int8_t sendATcommand(const char* ATcommand, const char* expected_answer, unsigned int timeout){

    uint8_t x=0,  answer=0;
    char response[100];
    unsigned long previous;

    memset(response, '\0', 100);    // Initialize the string

    delay(100);

    while( Serial.available() > 0) Serial.read();    // Clean the input buffer

    Serial.println(ATcommand);    // Send the AT command 


        x = 0;
    previous = millis();

    // this loop waits for the answer
    do{
        if(Serial.available() != 0){    
            // if there are data in the UART input buffer, reads it and checks for the asnwer
            response[x] = Serial.read();
            printf("%c",response[x]);
            x++;
            // check if the desired answer  is in the response of the module
            if (strstr(response, expected_answer) != NULL)    
            {
				printf("\n");
                answer = 1;
            }
        }
    }
    // Waits for the asnwer with time out
    while((answer == 0) && ((millis() - previous) < timeout));    

        return answer;
}

uint8_t SX1272ON()
{

  uint8_t state = 2;

  #if (SX1272_debug_mode > 1)
	  printf("\n");
	  printf("Starting 'ON'\n");
  #endif

  // Inital Reset Sequence

  // 1.- power ON embebed socket
  Utils.socketON();

  // 2.- reset pulse for LoRa module initialization
  pinMode(LORA_RESET_PIN, OUTPUT);
  digitalWrite(LORA_RESET_PIN, HIGH);
  delay(100);

  digitalWrite(LORA_RESET_PIN, LOW);
  delay(100);

  // 3.- SPI chip select
  pinMode(SX1272_SS,OUTPUT);
  digitalWrite(SX1272_SS,HIGH);
  delayMicroseconds(100);
 
  //Configure the MISO, MOSI, CS, SPCR.
  SPI.begin();
  //Set Most significant bit first
  SPI.setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
  //Divide the clock frequency
  SPI.setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64);
  //Set data mode
  SPI.setDataMode(BCM2835_SPI_MODE0);
  delayMicroseconds(100);
  setMaxCurrent(0x1B);
  #if (SX1272_debug_mode > 1)
	  printf("## Setting ON with maximum current supply ##\n");
	  printf("\n");
  #endif

  // set LoRa mode
  state = setLORA();

	//Set initialization values
	writeRegister(0x0,0x0);
	writeRegister(0x1,0x81);
	writeRegister(0x2,0x1A);
	writeRegister(0x3,0xB);
	writeRegister(0x4,0x0);
	writeRegister(0x5,0x52);
	writeRegister(0x6,0xD8);
	writeRegister(0x7,0x99);
	writeRegister(0x8,0x99);
	writeRegister(0x9,0x0);
	writeRegister(0xA,0x9);
	writeRegister(0xB,0x3B);
	writeRegister(0xC,0x23);
	writeRegister(0xD,0x1);
	writeRegister(0xE,0x80);
	writeRegister(0xF,0x0);
	writeRegister(0x10,0x0);
	writeRegister(0x11,0x0);
	writeRegister(0x12,0x0);
	writeRegister(0x13,0x0);
	writeRegister(0x14,0x0);
	writeRegister(0x15,0x0);
	writeRegister(0x16,0x0);
	writeRegister(0x17,0x0);
	writeRegister(0x18,0x10);
	writeRegister(0x19,0x0);
	writeRegister(0x1A,0x0);
	writeRegister(0x1B,0x0);
	writeRegister(0x1C,0x0);
	writeRegister(0x1D,0x4A);
	writeRegister(0x1E,0x97);
	writeRegister(0x1F,0xFF);
	writeRegister(0x20,0x0);
	writeRegister(0x21,0x8);
	writeRegister(0x22,0xFF);
	writeRegister(0x23,0xFF);
	writeRegister(0x24,0x0);
	writeRegister(0x25,0x0);
	writeRegister(0x26,0x0);
	writeRegister(0x27,0x0);
	writeRegister(0x28,0x0);
	writeRegister(0x29,0x0);
	writeRegister(0x2A,0x0);
	writeRegister(0x2B,0x0);
	writeRegister(0x2C,0x0);
	writeRegister(0x2D,0x50);
	writeRegister(0x2E,0x14);
	writeRegister(0x2F,0x40);
	writeRegister(0x30,0x0);
	writeRegister(0x31,0x3);
	writeRegister(0x32,0x5);
	writeRegister(0x33,0x27);
	writeRegister(0x34,0x1C);
	writeRegister(0x35,0xA);
	writeRegister(0x36,0x0);
	writeRegister(0x37,0xA);
	writeRegister(0x38,0x42);
	writeRegister(0x39,0x12);
	writeRegister(0x3A,0x65);
	writeRegister(0x3B,0x1D);
	writeRegister(0x3C,0x1);
	writeRegister(0x3D,0xA1);
	writeRegister(0x3E,0x0);
	writeRegister(0x3F,0x0);
	writeRegister(0x40,0x0);
	writeRegister(0x41,0x0);
	writeRegister(0x42,0x22);
	
  return state;
}

int main(){
	//LoRasetup();
	SX1272ON();
    GPSsetup();
	
    while(1){loop();}
    return 0;
}