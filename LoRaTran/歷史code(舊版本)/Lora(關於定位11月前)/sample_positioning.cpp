//
//  main.cpp
//  RSSI_to_distance+positioning
//
//  Created by Keng on 18/10/2016.
//  Copyright © 2016 Keng. All rights reserved.
//

#include <iostream>
#include "LoRa_RSSIToDistance.h"
#include "Lora_Positioning.h"
using namespace std;
int main(int argc, const char * argv[]) {
    
    Rssi_BufferManager bufferManger;
    Rssi_info new_rssi;
    Rssi_info* rssi_arr;
    Locate_info loca_info;
    Point BSpointA,BSpointB,BSpointC;
    Point finalPoint;
    
    BSpointA=set_Bspoint(4.0,4.0, 'A');
    BSpointB=set_Bspoint(9.0,7.0, 'B');
    BSpointC=set_Bspoint(9.0,1.0, 'C');
    
    while (1) {
        
        bufferManger.buf_push(new_rssi);
        rssi_arr = bufferManger.collectTripleRssi();
        
        if (rssi_arr != NULL) {
            float d_arr[3];
            for (int i = 0; i < 3; i++) {
                d_arr[i] = Rssi_to_distance(*(rssi_arr + i), -50, 2);
            }
            loca_info = FillIndoorLocate_info(d_arr[0], d_arr[1], d_arr[2]);
        }
        else {
            continue;
        }
        /*
         loca_info.distances[0]=4;
         loca_info.distances[1]=3;
         loca_info.distances[2]=3.25;
         */
        finalPoint=trilateration(BSpointA, BSpointB, BSpointC, loca_info.distances[0],loca_info.distances[1], loca_info.distances[2]);
        cout<<finalPoint.x<<" "<<finalPoint.y;
        
    }
    
    return 0;
}
