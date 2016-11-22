//
//  testBluet.cpp
//  
//
//  Created by 胡育旋 on 2016/11/22.
//
//


#include "arduPiLoRa.h"
#include "Lora_Setup.h"
#include "Bluetooth.h"
#include <iostream>
#include <string>

using namespace std;

Position GPSInfo;

int main (int argc, char **argv){
    
    BluetoothInitial();
    while(1){
         GPSInfo=readFromBluetooth();
    }
    return (0);
}


