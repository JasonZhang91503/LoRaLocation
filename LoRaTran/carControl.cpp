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
 *  Design:            David Gascon 
 *  Implementation:    Alejandro Gallego & Marcos Martinez
 */
 
 //Include arduPi library
 #include "arduPi.h"
 #include <iostream>
 
 using namespace std;

 int8_t sendATcommand(const char* ATcommand, const char* expected_answer1, unsigned int timeout);
 void power_on();
 
int8_t answer;
int counter;
long previous;


char Basic_str[100];
char GGA_str[100];
char GLL_str[100];
char RMC_str[100];
char VTG_str[100];
char ZDA_str[100];

void setup(){
    Serial.begin(9600);   
    
    printf("Starting...\n");
}

void loop(){
    // Basic
    // Clean the input buffer
    while( Serial.available() > 0) Serial.read();    
    delay(100);
    // request Basic string
    //sendATcommand("AT+CGPSINF=0", "AT+CGPSINF=0\r\n\r\n", 2000);
    
	char send_str[100];
	
	cout<< "Input : ";
	cin >> send_str;
	
	Serial.println();
	
	/*
	
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
    
	
	
	
    Basic_str[counter] = '\0';
 
    printf("Basic string: ");
    printf("%s\n",Basic_str);
	
	*/
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

int main(){
    setup();
    while(1){loop();}
    return 0;
}

    