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
int s,client, bytes_read=0;
struct sockaddr_rc loc_addr = {0},rem_addr={0};
char mes[30]={"0"};

struct position{
    char latitude[10];
    char longitude[10];
};
typedef struct position Position;

int findLab(char tmp){
	for(int i=1;i<=sizeof(mes);i++){
		if(mes[i]==tmp){
			//cout<<"find "<<tmp<<" at "<<i<<endl;
			return i;
		}
	}
	cout<<"cannot find "<<tmp<<endl;
}

void copyToString(char *str,int start,int end){
	int j=0;	
	for(int i=start;i<=end;i++){
		str[j]=mes[i];
		j++;
	}
	str[j]=0;
	//cout<<"after copy:str = "<<str;
}

void BluetoothInitial(){
    socklen_t opt = sizeof(rem_addr);
    s = socket(AF_BLUETOOTH,SOCK_STREAM,BTPROTO_RFCOMM);
    
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = my_bdaddr_any;//any local Bluetooth adapter is acceptable
    loc_addr.rc_channel = (uint8_t) 22; //port number to listen
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
    cout<<"bind on 22 complete"<<endl;
    listen(s, 1);
    cout<<"listen on s 1"<<endl;
    
    client = accept(s, (struct sockaddr *)&rem_addr, &opt);
	cout<<"client: "<<client<<endl;
    char buf[1024]={0};
    ba2str( &rem_addr.rc_bdaddr, buf );
    cout<<"accepted connection from"<<buf<<endl;
    
    memset(buf, 0, sizeof(buf));
}

Position readFromBluetooth(){    
    Position local;
	
    // read data from the client
  
		bytes_read =unistd::read(client, mes, sizeof(mes));	
    	cout<<"read complete"<<endl;
    	if( bytes_read > 0 ) {	
			cout<<"bytes read = "<<bytes_read<<endl;
			cout<<"mes: "<<mes<<endl;			
			int t1=findLab(37);
			copyToString((local.latitude),1,t1-1);  
        	int t2=findLab(36);
			copyToString((local.longitude),t1+1,t2-1);	     
	        cout<<"latitude: "<<local.latitude<<" longitude: "<<local.longitude<<endl;
		}
		else{
			cout<<"bytes_read:"<<bytes_read<<" error: "<<errno<<endl;
			//	continue;
		}
		//}
		return local;
}

int closeBluetooth(){
    unistd::close(client);
    unistd::close(s);
    return 0;
}


#endif /* bluetooth_h */
