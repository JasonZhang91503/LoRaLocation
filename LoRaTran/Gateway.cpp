//
//  main.cpp
//  gateway
//
//  Created by Keng on 15/01/2017.
//  Copyright © 2017 Keng. All rights reserved.
//

#include <iostream>
#include <mysql.h>
#include <string.h>
#include "arduPiLoRa.h"
#include "Lora_Setup.h"

using namespace std;
#define DATABASE_USERNAME "root"
#define DATABASE_PASSWORD "jizzbear"
#define DATABASE_NAME "postman"
#define IP "192.168.0.116"
#define PORT 3306
int e,NodeAddress=1;
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
void finish_with_error(MYSQL *con)
{
    cout<<mysql_error(con)<<endl;
    mysql_close(con);
    exit(1);
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
        cout<<lastPacketMessage<<endl;
        return messageStr;
    }
    else {
        printf("Receive packet, state %d\n", e);
        return NULL;
    }
}
int main(int argc, const char * argv[]) {
   
    MYSQL *mysql1;
    mysql1=database_initial(mysql1);
    mysql_library_init(0,NULL,NULL);
    string state;
    char *recvMsg;
    e=setup(NodeAddress);
    cout<<"[State 0] Query state about the database"<<endl;
    if (mysql_query(mysql1, "SELECT state FROM transport")){finish_with_error(mysql1);}
    else{
        MYSQL_RES *result = mysql_store_result(mysql1);
        if(result==NULL){finish_with_error(mysql1);}
        else{
            int num_fields = mysql_num_fields(result);
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(result)))
            {
                for(int i = 0; i < num_fields; i++)
                    {
                        state=row[i];
                        cout<<"Get State "<<row[i]<<endl;
                    }
            }
        }
        //Get longtitude latitude
    }
    if(state=="0"){
        send(state,e);//還要給經緯度
    }
    cout<<"[State 1] Wait for message 1 & update the database state to 2"<<endl;
    while(1){
        recvMsg = recvMessage();
        if(strcmp(recvMsg,"1")){
            send("2",e);
            if (mysql_query(mysql1, "UPDATE transport SET state = 2 where _id=2;"))
            {
                finish_with_error(mysql1);
            }else{
                cout<<"update database to 2"<<endl;
            }
            break;
        }
    }
    cout<<"[State 2] Wait for message 3"<<endl;
    while(1){
        recvMsg = recvMessage();
        if(strcmp(recvMsg,"3")){
            if (mysql_query(mysql1, "UPDATE transport SET state = 3 where _id=2;"))
            {
                finish_with_error(mysql1);
            }else{
                cout<<"update database to 3"<<endl;

            }
            break;
        }
    }
    cout<<"[State 3] Wait for message 4"<<endl;
    while(1){
        recvMsg = recvMessage();
        if(strcmp(recvMsg,"4")){
            if (mysql_query(mysql1, "UPDATE transport SET state = 4 where _id=2;"))
            {
                finish_with_error(mysql1);
            }else{
                cout<<"update database to 4"<<endl;
            }
            break;
        }
    }

    return 0;
}
