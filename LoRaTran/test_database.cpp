//
//  main.cpp
//  Lora_database
//
//  Created by Keng on 13/01/2017.
//  Copyright © 2017 Keng. All rights reserved.
//

#include <iostream>
#include <my_global.h>
#include <mysql.h>
#include <string.h>
#include "arduPiLoRa.h"
#include "Lora_Setup.h"
//define field
#define DATABASE_USERNAME "root"
#define DATABASE_PASSWORD "s410385007"
#define DATABASE_NAME "bonjour"
#define IP "192.168.100.8"
#define PORT 3306

using namespace std;

MYSQL *database_initial(MYSQL *sql){
    sql=mysql_init(NULL);
    
    if(sql == NULL)
    {
        fprintf(stderr, "ABB : %s\n", mysql_error(sql));
        return 0;
    }
    if(mysql_real_connect(sql, IP, DATABASE_USERNAME, DATABASE_PASSWORD, NULL, PORT,NULL ,0)==NULL){
        cout<<DATABASE_NAME<<" is connected sucessfully"<<endl;
    }else{
        int e = mysql_errno(sql);
        cout<<"error: "<<e<<endl;
    }
    return sql;
}
void send(string s,int e){
    char send_str[100];
    sprintf(send_str,"%s",s.c_str());
    e = sx1272.sendPacketTimeout(0,send_str);
    printf("Packet sent, state %d\n",e);
    
}
char* recvMessage() {
    // Receive message
    int e;
    char *messageStr;
    char lastPacketMessage[100];
    cout<<"Wait for sent..."<<endl;
    e = sx1272.receivePacketTimeout(10000);
    
    
    if (e == 0)
    {
        messageStr = (char*)malloc(sizeof(char)*sx1272.packet_received.length);
        printf("Receive packet, state %d\n", e);
        
        for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
        {
            lastPacketMessage[i] = (char)sx1272.packet_received.data[i];
            messageStr[i] = lastPacketMessage[i];
        }
        cout<<"message:"<<endl;
        //fileInput(lastPacketMessage);
        cout<<lastPacketMessage<<endl;
        return messageStr;
    }
    else {
        printf("Receive packet, state %d\n", e);
        return NULL;
    }
}
int main(int argc, const char * argv[]) {
    
    //database
    MYSQL *mysql1;
    MYSQL_RES *res;
    MYSQL_FIELD *col;
    int num_field,num_row;
    mysql1=database_initial(mysql1);
    mysql_library_init(0,NULL,NULL);
    int mode;
    //lora
    int state,NodeAddress=1,e;
    string str;
    string recvMsg;
    e=setup(NodeAddress);
    cout<<"Please input the state of LoRa postman :"<<endl;
    cout<<"[Mode 1] Query information about the database"<<endl;
    cout<<"[Mode 2] Modify the information about the database"<<endl;
    cout<<"[Mode 3] Send message to Car by LoRa"<<endl;
    cout<<"[Mode 4] Receive message to Car by LoRa"<<endl;
    //---------------------------------------------------//
    
    while(1){
        cin>>mode;
        switch(mode){
            case 1:
            {
                mysql_query(mysql1,"use bonjour");
                res=mysql_list_fields(mysql1, "ntpu", "%");
                num_field = mysql_num_fields(res);
                printf("Number of columns: %d\n", num_field);
                
                for (int i=0; i <num_field; ++i)
                {
                    /* col describes i-th column of the table */
                    col = mysql_fetch_field_direct(res, i);
                    printf ("Column %d: %s\n", i, col->name);
                }
                break;
            }
            case 2:
            {
                mysql_close(mysql1);
                mysql_library_end();
                return 0;
                break;
            }
            case 3:
                //query 資料->若query到資料，執行到send 指令
                str=to_string(0);
                send(str,e);
                
                break;
            case 4:
                recvMsg = recvMessage();
                if(recvMsg=="1"){
                    send("2",e);
                }else if(recvMsg=="3"){
                    //更新db_state->3
                     cout<<"update database state :3"<<endl;
                }else if(recvMsg=="4"){
                    //更新db_state->4
                    cout<<"update database state :4"<<endl;
                }else{
                    cout<<"wrong state"<<endl;
                }
                free(recvMsg);
                break;
            default:
                break;
                
        }
    }
    
    return 0;
}
