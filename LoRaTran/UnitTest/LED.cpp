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
 #include "iostream"
 using namespace std;

 void power_on();
 
int8_t answer;
int onModulePin0= 0;
int onModulePin1= 1;
int onModulePin2= 2;
int onModulePin3= 3;
int onModulePin4= 4;
int onModulePin5= 5;
int onModulePin6= 6;
int onModulePin7= 7;
int onModulePin8= 8;
int onModulePin9= 9;
int onModulePin10= 10;
int onModulePin11= 11;
int onModulePin12= 12;
int onModulePin13= 13;
int onModulePin14= 14;
int onModulePin15= 15;
int counter;
long previous;


char Basic_str[100];
char GGA_str[100];
char GLL_str[100];
char RMC_str[100];
char VTG_str[100];
char ZDA_str[100];

void setup(){
    int a;
    cout << "input:"
    cin >> a;
    
    bool b = false;
    
    pinMode(a, OUTPUT);      
    printf("Starting...\n");


               digitalWrite(onModulePin,LOW);

    while(cin.get()!= 'x'){
        if(!b){
               b = true;
               digitalWrite(onModulePin,HIGH);
        } 
        else{
             b = false;
               digitalWrite(onModulePin,LOW);
        }
    }

}



int main(){
    setup();
    return 0;
}

    
