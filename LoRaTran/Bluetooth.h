//
//  bluetooth.h
//  
//
//  Created by 胡育旋 on 2016/11/8.
//
//
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include<iostream>
#include<string>
using namespace std;

#ifndef bluetooth_h
#define bluetooth_h
bdaddr_t my_bdaddr_any = {0,0,0,0,0,0};
int s,client, bytes_read;
struct sockaddr_rc loc_addr = {0},rem_addr={0};

struct position{
    char *latitude;
    char *longitude;
};
typedef struct position Position;
void BluetoothInitial(){
    socklen_t opt = sizeof(rem_addr);
    s = socket(AF_BLUETOOTH,SOCK_STREAM,BTPROTO_RFCOMM);
    
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = *BDADDR_ANY;//any local Bluetooth adapter is acceptable
    loc_addr.rc_channel = (uint8_t) 22; //port number to listen
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
    cout<<"bind on 22 complete"<<endl;
    listen(s, 1);
    cout<<"listen on s 1"<<endl;
    
    client = accept(s, (struct sockaddr *)&rem_addr, &opt);
    char buf[1024]={0};
    ba2str( &rem_addr.rc_bdaddr, buf );
    cout<<"accepted connection from"<<buf<<endl;
    
    memset(buf, 0, sizeof(buf));
}

Position readFromBluetooth(){
    char buf[1024]={0};
    Position local;
    // read data from the client
    bytes_read =unistd:read(client, buf, sizeof(buf));
    
    if( bytes_read > 0 ) {
        std::size_t found = buf.find("%");
        std::size_t length = buf.copy(local.latitude,found-1,1);
        local.latitude[length]="\0";
        
        std::size_t foundEnd = buf.find("$");
        length = buf.copy(local.longitude,foundEnd-found-1,found+1);
        local.longitude[length]="\0";

        cout<<"latitude: "<<local.latitude<<" longitude: "<<local.longitude<<endl;
        return local;
    }
}

int closeBluetooth(){
    close(client);
    close(s);
    return 0;
}

#endif /* bluetooth_h */
