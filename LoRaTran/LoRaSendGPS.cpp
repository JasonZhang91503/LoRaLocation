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
 
 //Include arduPi library
 #include "arduPi.h"

 int8_t sendATcommand(const char* ATcommand, const char* expected_answer1, unsigned int timeout);
 void power_on();
 
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

int main(){
    GPSsetup();
    while(1){loop();}
    return 0;
}