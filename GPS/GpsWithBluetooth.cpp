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
 
 #include <stdio.h>
 #include <unistd.h>
 #include <sys/socket.h>
 #include <bluetooth/bluetooth.h>
 #include <bluetooth/rfcomm.h>
bdaddr_t my_bdaddr_any = {0,0,0,0,0,0};
struct sockaddr_rc loc_addr = {0},rem_addr={0};
int s,client,bytes_read;
void createBluConn(){
     socklen_t opt = sizeof(rem_addr);
     s = socket(AF_BLUETOOTH,SOCK_STREAM,BTPROTO_RFCOMM);
     loc_addr.rc_family = AF_BLUETOOTH;
     loc_addr.rc_bdaddr = my_bdaddr_any;//any local Bluetooth adapter is acceptable
     loc_addr.rc_channel = (uint8_t) 22; //port number to listen
     bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
     printf("bind on 22 complete\n");
     // put socket into listening mode
     listen(s, 1);
     printf("listen on s 1\n");
    
     client = accept(s, (struct sockaddr *)&rem_addr, &opt);
     char buf[1024]={0};
     ba2str( &rem_addr.rc_bdaddr, buf );
     printf("accepted connection from %s\n", buf);
    
     memset(buf, 0, sizeof(buf));
}

void sendByBluetooth(char* str){
     unistd::write(client,str,sizeof(str));
     fprintf(stderr,"write %s to client\n",str);
}


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

void setup(){
    
    pinMode(onModulePin, OUTPUT);
    Serial.begin(115200);   
    
    printf("Starting...\n");
    power_on();
  	printf("power_on");  
 //   delay(3000);
    
      
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
   // delay(100);
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
    
    //**********************************************************
    // GGA
    // Clean the input buffer
    while( Serial.available() > 0) Serial.read();    
    //delay(100);
    // request GGA string
    sendATcommand("AT+CGPSINF=2", "AT+CGPSINF=2\r\n\r\n", 2000);
    
    counter = 0;
    answer = 0;
    memset(GGA_str, '\0', 100);    // Initialize the string
    previous = millis();
    // this loop waits for the NMEA string
    do{

        if(Serial.available() != 0){    
            GGA_str[counter] = Serial.read();
            counter++;
            // check if the desired answer is in the response of the module
            if (strstr(GGA_str, "OK") != NULL)    
            {
                answer = 1;
            }
        }
        // Waits for the asnwer with time out
    }while((answer == 0) && ((millis() - previous) < 2000));  
    
    GGA_str[counter-3] = '\0';
    
    //**********************************************************
    // GLL
    // Clean the input buffer
    while( Serial.available() > 0) Serial.read();    
       
    //delay(100);
    // request GLL string
    sendATcommand("AT+CGPSINF=4", "AT+CGPSINF=4\r\n\r\n", 2000);   
    
    counter = 0;
    answer = 0;
    memset(GLL_str, '\0', 100);    // Initialize the string
    previous = millis();
    // this loop waits for the NMEA string
    do{

        if(Serial.available() != 0){    
            GLL_str[counter] = Serial.read();
            counter++;
            // check if the desired answer is in the response of the module
            if (strstr(GLL_str, "OK") != NULL)    
            {
                answer = 1;
            }
        }
        // Waits for the asnwer with time out
    }while((answer == 0) && ((millis() - previous) < 2000)); 
    
    GLL_str[counter-3] = '\0';
        
    //**********************************************************
    // RMC
    // Clean the input buffer
    while( Serial.available() > 0) Serial.read();    
       
    //delay(100);
    // request RMC string 
    sendATcommand("AT+CGPSINF=32", "AT+CGPSINF=32\r\n\r\n", 2000);
    
    counter = 0;
    answer = 0;
    memset(RMC_str, '\0', 100);    // Initialize the string
    previous = millis();
    // this loop waits for the NMEA string
    do{

        if(Serial.available() != 0){    
            RMC_str[counter] = Serial.read();
            counter++;
            // check if the desired answer is in the response of the module
            if (strstr(RMC_str, "OK") != NULL)    
            {
                answer = 1;
            }
        }
        // Waits for the asnwer with time out
    }while((answer == 0) && ((millis() - previous) < 2000)); 
    
    RMC_str[counter-3] = '\0';
    
    //**********************************************************
    // VTG
    // Clean the input buffer
    while( Serial.available() > 0) Serial.read();    
       
    //delay(100);
    // request VTG string
    sendATcommand("AT+CGPSINF=64", "AT+CGPSINF=64\r\n\r\n", 2000); 
    
    counter = 0;
    answer = 0;
    memset(VTG_str, '\0', 100);    // Initialize the string
    previous = millis();
    // this loop waits for the NMEA string
    do{

        if(Serial.available() != 0){    
            VTG_str[counter] = Serial.read();
            counter++;
            // check if the desired answer is in the response of the module
            if (strstr(VTG_str, "OK") != NULL)    
            {
                answer = 1;
            }
        }
        // Waits for the asnwer with time out
    }while((answer == 0) && ((millis() - previous) < 2000)); 
    
    VTG_str[counter-3] = '\0';
    
    //**********************************************************
    // ZDA
    // Clean the input buffer
    while( Serial.available() > 0) Serial.read();    
       
    //delay(100);
    // request ZDA string
    sendATcommand("AT+CGPSINF=128", "AT+CGPSINF=128\r\n\r\n", 2000);
    
    counter = 0;
    answer = 0;
    memset(ZDA_str, '\0', 100);    // Initialize the string
    previous = millis();
    // this loop waits for the NMEA string
    do{

        if(Serial.available() != 0){    
            ZDA_str[counter] = Serial.read();
            counter++;
            // check if the desired answer is in the response of the module
            if (strstr(ZDA_str, "OK") != NULL)    
            {
                answer = 1;
            }
        }
        // Waits for the asnwer with time out
    }while((answer == 0) && ((millis() - previous) < 2000));
    
    ZDA_str[counter-3] = '\0';
    
    printf("*************************************************\n");
    printf("Basic string: ");
    printf("%s\n",Basic_str);
    printf("GGA string: ");
    printf("%s\n",GGA_str);
    printf("GLL string: ");
    printf("%s\n",GLL_str);
    sendByBluetooth(Basic_str);
    printf("RMC string: ");
    printf("%s\n",RMC_str);
    printf("VTG string: ");
    printf("%s\n",VTG_str);
    printf("ZDA string: ");
    printf("%s\n",ZDA_str);
   
   // delay(15000);

}

void power_on(){

    uint8_t answer=0;
    
    // checks if the module is started
    answer = sendATcommand("AT", "OK", 2000);
    if (answer == 0)
    {
        // power on pulse
        digitalWrite(onModulePin,HIGH);
     //   delay(3000);
        digitalWrite(onModulePin,LOW);
    
        // waits for an answer from the module
        while(answer == 0){    
            // Send AT every two seconds and wait for the answer
            answer = sendATcommand("AT", "OK", 2000);    
        }
    }
    
}


int8_t sendATcommand(const char* ATcommand, const char* expected_answer, unsigned int timeout){
	printf("send AT command %s \n",ATcommand);	

    uint8_t x=0,  answer=0;
    char response[100];
    unsigned long previous;

    memset(response, '\0', 100);    // Initialize the string

  //  delay(100);

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
    createBluConn();
	
    setup();
    while(1){loop();}
    return 0;
}

    
